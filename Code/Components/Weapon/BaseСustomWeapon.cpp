#include "StdAfx.h"
#include "BaseCustomWeapon.h"


void BaseCustomWeapon::Initialize()
{
    m_pRigidBodyComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CRigidBodyComponent>();
}

void BaseCustomWeapon::StartGame()
{
    Physicalize(false);
}

void BaseCustomWeapon::Update(float fFrameTime)
{
}


uint64 BaseCustomWeapon::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}


void BaseCustomWeapon::ProcessEvent(const SEntityEvent& event)
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
        Update(pCtx->fFrameTime);
    }
    break;
    }
}

void BaseCustomWeapon::Physicalize(bool isPhysicalize)
{
    m_pRigidBodyComponent->Enable(isPhysicalize);
    m_pRigidBodyComponent->ApplyImpulse(Vec3(0, 0, 20));

    
}