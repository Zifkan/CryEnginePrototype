#include "StdAfx.h"
#include "Player.h"


#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "Camera/CameraController.h"
#include "AttackAction.h"
#include "MovementAction.h"
#include "IdleAction.h"



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


CPlayerComponent::CPlayerComponent(): 
    m_characterEntityName("Player"),
    m_sprintRatio(1.5f),
    m_sprintAnimRatio(4.0f)
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
	m_pAnimationComponent->SetAnimationDrivenMotion(true);
    

	// Load the character and Mannequin data from file
	m_pAnimationComponent->LoadFromDisk();
  

	// Acquire fragment and tag identifiers to avoid doing so each update
	

   
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
      
        CreateStateMachine();
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
       // CryLog("IsPlayingFragment time: %f", m_pAnimationComponent->GetCharacter()->GetISkeletonAnim()->GetAnimFromFIFO(1, 0).GetCurrentSegmentNormalizedTime());
        m_stateMachine->Update();
        m_stateMachine->SetCurrentState(typeid(IdleAction));
	}
    break;
    case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
    {
        Initialize();
    }
	break;
	}
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


	m_lookOrientation = IDENTITY;
	m_horizontalAngularVelocity = 0.0f;
}

void CPlayerComponent::InitInput(ICharacterActions* playerCharacterActions)
{    
    m_pCharacterActions = playerCharacterActions;
    m_pPlayerInput->RegisterInputs(playerCharacterActions);
    SetupActions();
}

void CPlayerComponent::SetupActions()
{    
    m_pCharacterActions->MovementSubject.get_observable().subscribe([this](Vec2 Vector2)
    {
        if (Vector2.GetLength2() > 0)
        {
            m_stateMachine->SetCurrentState(typeid(MovementAction));
        }
    });

    m_pCharacterActions->AttackSubject.get_observable().subscribe([this](AttackType type)
    {
        m_stateMachine->SetCurrentState(typeid(AttackAction));
    });

    m_pCharacterActions->MovementTypeSubject.get_observable().subscribe([this](MovementType type)
    {
       
    });
}

void CPlayerComponent::CreateStateMachine()
{
    m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
    m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
    m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");
    m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    m_forceAttackTagId = m_pAnimationComponent->GetTagId("ForceAttack");


    m_stateMachine = new CStateMachine(m_pCharacterController, m_pAnimationComponent, m_pEntity);

    m_stateMachine->RegisterState(typeid(IdleAction), new IdleAction(m_pCharacterActions,0,m_idleFragmentId));
    m_stateMachine->RegisterState(typeid(MovementAction), new MovementAction(m_pEntity, m_pAnimationComponent, m_pCharacterController, m_pMainCamera, m_pCharacterActions, 1, m_walkFragmentId));
    m_stateMachine->RegisterState(typeid(AttackAction), new AttackAction(m_pEntity, m_pAnimationComponent, m_pCharacterController, m_pCharacterActions, 2, m_attackFragmentId));
}
