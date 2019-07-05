#include "StdAfx.h"


#include "Camera/CameraController.h"
#include "StateMachine/StateAction/IdleAction.h"
#include "CharacterComponent.h"




CCharacterComponent::CCharacterComponent(): 
    m_characterEntityName("Player")
{    
}

void CCharacterComponent::Initialize()
{       
   // gEnv->pEntitySystem->RemoveEntity(m_pEntity->GetComponent<Cry::DefaultComponents::CCameraComponent>()->GetEntity()->GetId(),true);
	// The character controller is responsible for maintaining player physics
	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	// Offset the default character controller up by one unit
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	// Create the advanced animation component, responsible for updating Mannequin and animating the player
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	
	// Set the player geometry, this also triggers physics proxy creation
	//m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/LegionerThirdpersonDefenition.adb");

	//m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/LegionFirstPersonControllerDefinition.xml");
//	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");


	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	//m_pAnimationComponent->SetAnimationDrivenMotion(false);
    

	// Load the character and Mannequin data from file
	//m_pAnimationComponent->LoadFromDisk();
  	

    m_pWeaponSystem = m_pEntity->GetOrCreateComponent<CWeaponSystem>();

	Revive();
}

uint64 CCharacterComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void CCharacterComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_START_GAME:
	{
        StartGame();
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

        if (gEnv->IsEditor())
            EditorUpdate(pCtx->fFrameTime);

        if (gEnv->IsGameOrSimulation())
            GameUpdate(pCtx->fFrameTime);
        
	}
    break;
    case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
    {
        PropertyChanged();
        Initialize();
    }
	break;
	}
}


void CCharacterComponent::Revive()
{
	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);

	// Make sure that the player spawns upright
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	// Apply character to the entity
	m_pAnimationComponent->ResetCharacter();
	m_pCharacterController->Physicalize();
}

void CCharacterComponent::EditorUpdate(float fFrameTime)
{
}

void CCharacterComponent::GameUpdate(float fFrameTime)
{    
    m_lifeResourceManager.Update(fFrameTime);
}

void CCharacterComponent::InitWeaponSystem()
{
    m_pWeaponSystem->Init(m_pCharacterActions, m_pAnimationComponent->GetCharacter()->GetIAttachmentManager());
}
