#include "StdAfx.h"
#include "Player.h"


#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "Camera/CameraController.h"

static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent)


CPlayerComponent::CPlayerComponent(): m_characterEntityName("Player"), m_sprintRatio(1.5f), m_sprintAnimRatio(4.0f)
{
}

void CPlayerComponent::Initialize()
{   	
	// The character controller is responsible for maintaining player physics
	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	// Offset the default character controller up by one unit
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	// Create the advanced animation component, responsible for updating Mannequin and animating the player
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	
	// Set the player geometry, this also triggers physics proxy creation
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/LegionerThirdpersonDefenition.adb");
	//m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");

	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/LegionFirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	// Queue the idle fragment to start playing immediately on next update
	m_pAnimationComponent->SetDefaultFragmentName("Idle");

	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
    

	// Load the character and Mannequin data from file
	m_pAnimationComponent->LoadFromDisk();

	// Acquire fragment and tag identifiers to avoid doing so each update
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");
    m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    m_forceAttackTagId = m_pAnimationComponent->GetTagId("ForceAttack");

    m_pPlayerInput = m_pEntity->GetOrCreateComponent<CPlayerInputComponent>();
    m_pEntity->SetName(m_characterEntityName.c_str());

	Revive();
}

uint64 CPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_START_GAME:
	{      
		// Revive the entity when gameplay starts
		Revive();
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		// Start by updating the movement request we want to send to the character controller
		// This results in the physical representation of the character moving
		UpdateMovementRequest(pCtx->fFrameTime);

		// Process mouse input to update look orientation.
		UpdateLookDirectionRequest(pCtx->fFrameTime);

		// Update the animation state of the character
		UpdateAnimation(pCtx->fFrameTime);

	
        m_FrameTick.get_subscriber().on_next(pCtx->fFrameTime);
	}
    break;
    case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
    {
        Initialize();
    }
	break;
	}
}

void CPlayerComponent::UpdateMovementRequest(float frameTime)
{  
    Vec3 velocity = ZERO;

    if (!m_pCharacterController->IsOnGround()) return;

	const float moveSpeed = 20.5f;

	velocity.x += (m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * frameTime * m_moveDirection.x);
	velocity.y += (m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * frameTime * m_moveDirection.y);
    
	m_pCharacterController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
}

void CPlayerComponent::UpdateLookDirectionRequest(float frameTime)
{
	const float rotationSpeed = 0.002f;
	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	// Apply smoothing filter to the mouse input
	m_mouseDeltaRotation = m_mouseDeltaSmoothingFilter.Push(m_mouseDeltaRotation).Get();

	// Update angular velocity metrics
	m_horizontalAngularVelocity = (m_mouseDeltaRotation.x * rotationSpeed) / frameTime;
	m_averagedHorizontalAngularVelocity.Push(m_horizontalAngularVelocity);

	// Start with updating look orientation from the latest input
	//Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	//// Yaw
	//ypr.x += m_mouseDeltaRotation.x * rotationSpeed;

	//// Pitch
	//// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	//ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);

	//// Roll (skip)
	//ypr.z = 0;

	//m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	//// Reset the mouse delta accumulator every frame
	//m_mouseDeltaRotation = ZERO;
}

void CPlayerComponent::UpdateAnimation(float frameTime)
{
	const float angularVelocityTurningThreshold = 0.174; // [rad/s]

	// Update tags and motion parameters used for turning
	const bool isTurning = std::abs(m_averagedHorizontalAngularVelocity.Get()) > angularVelocityTurningThreshold;
	m_pAnimationComponent->SetTagWithId(m_rotateTagId, isTurning);
	if (isTurning)
	{
		// TODO: This is a very rough predictive estimation of eMotionParamID_TurnAngle that could easily be replaced with accurate reactive motion 
		// if we introduced IK look/aim setup to the character's model and decoupled entity's orientation from the look direction derived from mouse input.

		const float turnDuration = 1.0f; // Expect the turning motion to take approximately one second.
		m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, m_horizontalAngularVelocity * turnDuration);
	}

	// Update active fragment
	const auto& desiredFragmentId = m_pCharacterController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId)
	{
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}

	// Update entity rotation as the player turns
	// We only want to affect Z-axis rotation, zero pitch and roll
	/*Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));
	ypr.y = 0;
	ypr.z = 0;
	const Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(ypr));*/

	// Send updated transform to the entity, only orientation changes
//	GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));

   /* float testValue = 0.0f;
    m_pAnimationComponent->GetCharacter()->GetISkeletonAnim()->GetDesiredMotionParam(eMotionParamID_TravelSpeed, testValue);
   
*/
    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, m_movementType != WALK? m_sprintAnimRatio:1);
}


void CPlayerComponent::Revive()
{
	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);

	// Make sure that the player spawns upright
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	// Apply character to the entity
	m_pAnimationComponent->ResetCharacter();
	m_pCharacterController->Physicalize();

	// Reset input now that the player respawned
	m_inputFlags = 0;
	m_mouseDeltaRotation = ZERO;
	m_mouseDeltaSmoothingFilter.Reset();

	m_activeFragmentId = FRAGMENT_ID_INVALID;

	m_lookOrientation = IDENTITY;
	m_horizontalAngularVelocity = 0.0f;
	m_averagedHorizontalAngularVelocity.Reset();
}

void CPlayerComponent::InitInput(ICharacterActions* playerCharacterActions)
{    
    m_pCharacterActions = playerCharacterActions;
    m_pPlayerInput->RegisterInputs(playerCharacterActions);
    SetupActions();
}

void CPlayerComponent::SetupActions()
{
    m_pCharacterActions->MovementSubject.get_observable().subscribe([this](Vec2 Vector2) {  m_moveDirection = Vector2; });

    m_pCharacterActions->RotateYawSubject.get_observable().subscribe([this](float x) {  m_mouseDeltaRotation.x -= x; });
    m_pCharacterActions->RotatePitchSubject.get_observable().subscribe([this](float y) {  m_mouseDeltaRotation.y -= y; });

   
    m_pCharacterActions->MovementTypeSubject.get_observable()
    .buffer_with_time(std::chrono::milliseconds(500), rxcpp::observe_on_new_thread())
    .subscribe([this](std::vector<MovementType> v)
    {
        if (v.empty())
        {
            m_movementType = WALK;
            //CryLog("Type of Movement = Walk");
            return;
        }

        if (!v.empty())
        {
            if (v[v.size() - 1] == WALK)
            {
                m_movementType = DODGE;
                //CryLog("Type of Movement = Dodge");
                return;
            }

            if (v[v.size() - 1]==SPRINT)
            {
                m_movementType = SPRINT;
                //CryLog("Type of Movement = Sprint");
            }
        }
    });



    m_pCharacterActions->AttackSubject.get_observable().subscribe([this](bool isAttack)
    {          

        if (!pAction || (pAction && pAction->GetActiveTime()<0.1f))
        {
            pAction = new TAction< SAnimationContext >(1, m_attackFragmentId);
            // pAction->SetOptionIdx(0);        
            m_pAnimationComponent->QueueCustomFragment(*pAction);
           
        }      
      
    });
}

bool CPlayerComponent::IsAnimationPlaying(FragmentID fragmentId, int animLayer)
{   
   /* ICharacterInstance* pCharacter = m_pAnimationComponent->GetCharacter();
  //  pCharacter->fra
    if (pCharacter && !animName.empty())
    {       
        ISkeletonAnim* pISkeletonAnim = pCharacter->GetISkeletonAnim();

        int animID = pCharacter->GetIAnimationSet()->GetAnimIDByName(animName.c_str());

        int numAnims = pISkeletonAnim->GetNumAnimsInFIFO(animLayer);
        for (int i = 0; i < numAnims; ++i)
        {
            const CAnimation& anim = pISkeletonAnim->GetAnimFromFIFO(animLayer, i);
            int32 animInLayerID = anim.GetAnimationId();
            if (animInLayerID == animID)
            {                

                // animation is playing, but is it also the top of the stack? (FIFO - last one in)
                // this is done this way because in theory the animation could be in the FIFO several times
                // so DO NOT optimize this to just check whether i == numAnims - 1 please
                const CAnimation& topAnim = pISkeletonAnim->GetAnimFromFIFO(animLayer, numAnims - 1);
                int32 topAnimId = topAnim.GetAnimationId();
                if (topAnimId == animID)
                {
                    
                }

                return true;
            }
        }

    }

    // else it's apparently all false*/
    return false;
}
