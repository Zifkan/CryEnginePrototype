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
   
    gEnv->pEntitySystem->RemoveEntity(m_pEntity->GetComponent<Cry::DefaultComponents::CCameraComponent>()->GetEntity()->GetId(),true);
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
  //  m_pEntity->SetName(m_characterEntityName.c_str());
    m_pEntity->SetName("Player");  
   
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
        m_pMainCamera = gEnv->pEntitySystem->FindEntityByName("GameCamera");
        CRY_ASSERT(m_pMainCamera != nullptr);
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

		// Update the animation state of the character
		UpdateAnimation(pCtx->fFrameTime);

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
    if (!m_pCharacterController->IsOnGround() || !m_pMainCamera) return;
    
	const float moveSpeed = 20.5f;
    Vec3 camForward = m_pMainCamera->GetForwardDir();
    camForward.z = 0;

    Vec3 m_Move = m_moveDirection.y * camForward.normalized() + m_moveDirection.x * m_pMainCamera->GetRightDir();   
    
    if (m_moveDirection.GetLength2() > 0)
    {
        m_pEntity->SetRotation(quaternion::CreateRotationVDir(m_Move));
        m_pCharacterController->AddVelocity((m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * frameTime * m_pEntity->GetForwardDir()));
    }
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
   
    m_pCharacterActions->SprintSubject.get_observable()
    .map([](bool isPressed) { return HoldDetectionStruct<MovementType>(isPressed, gEnv->pTimer->GetCurrTime(),WALK); })
    .scan(HoldDetectionStruct<MovementType>(false, 0.0f,WALK),[](HoldDetectionStruct<MovementType> last, HoldDetectionStruct<MovementType> current)
    {    
        if (current.IsSignal && !last.IsSignal)
        {
            return HoldDetectionStruct<MovementType>(current.IsSignal, current.Time, DODGE);
        }

        if (current.IsSignal && last.IsSignal)
        {          
            return HoldDetectionStruct<MovementType>(last.IsSignal, last.Time, current.Time - last.Time > 0.25f ? SPRINT : DODGE);
        }
        return current;       
    })
    .map([](HoldDetectionStruct<MovementType> value) {return value.TypeValue; })
    .distinct_until_changed()
    .subscribe([this](MovementType value)
    {
        m_movementType = value;
        CryLog("Type of Movement = %i", m_movementType);
    });

    m_pCharacterActions->AttackSubject.get_observable()
    .map([](bool isPressed) { return HoldDetectionStruct<bool>(isPressed, 0, false); })
    .scan(HoldDetectionStruct<bool>(false, 0,false),[](HoldDetectionStruct<bool> last, HoldDetectionStruct<bool> current)
    {        
        float time = 0.0f;
       
        if (last.Time >= 0.25f)
        {
            return HoldDetectionStruct<bool>(true, 0, true);
        }
        
        if (current.IsSignal )
        {
            time += last.Time + gEnv->pTimer->GetFrameTime();
            return HoldDetectionStruct<bool>(false, time, false);
        }

        if (last.Time > 0.1f && last.Time < 0.25f)
        {                    
            return HoldDetectionStruct<bool>(true, 0, false);
        }      

        return HoldDetectionStruct<bool>(false, time, false);
    })
   // .skip_while([](HoldDetectionStruct<std::tuple<bool, int>> data) {return !data.IsSignal; })        
    .subscribe([this](HoldDetectionStruct<bool> attackData)
    {          

        if (!pAction || (pAction && pAction->GetActiveTime()<0.1f))
        {

            if (!attackData.IsSignal) return;

            pAction = new TAction< SAnimationContext >(1, m_attackFragmentId); 
            pAction->SetOptionIdx(0);
            m_pAnimationComponent->QueueCustomFragment(*pAction);

            CryLog("Is simple: %i", attackData.TypeValue);
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
