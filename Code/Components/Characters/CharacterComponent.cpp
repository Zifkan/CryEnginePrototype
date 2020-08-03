#include "StdAfx.h"
#include "CharacterComponent.h"

CCharacterComponent::CCharacterComponent()
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
  	
 
    m_pWeaponSystem = m_pEntity->GetOrCreateComponent<WeaponSystemComponent>();

    m_pHitDamageComponent = m_pEntity->GetOrCreateComponent<CHitDamageComponent>();

    m_pAnimationComponent->SetAnimationDrivenMotion(true);
    m_pAnimationComponent->LoadFromDisk();
	Revive();
}

Cry::Entity::EventFlags CCharacterComponent::GetEventMask() const
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

void CCharacterComponent::SetStatus(CharecterStatusFlag flag)
{
    m_currentStatus |= (uint32)flag;
}

void CCharacterComponent::UnSetStatus(CharecterStatusFlag flag)
{
    m_currentStatus &= ~(uint32)flag;
}

void CCharacterComponent::EditorUpdate(float fFrameTime)
{
}

void CCharacterComponent::GameUpdate(float fFrameTime)
{
}

void CCharacterComponent::InitWeaponSystem()
{
   
}
