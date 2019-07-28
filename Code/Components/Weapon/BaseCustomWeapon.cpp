#include "StdAfx.h"
#include "BaseCustomWeapon.h"


void BaseCustomWeapon::Initialize()
{
    
}

void BaseCustomWeapon::StartGame()
{
   
}

void BaseCustomWeapon::Update(float fFrameTime)
{
}


uint64 BaseCustomWeapon::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | 
           ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | 
           ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED) | 
           ENTITY_EVENT_BIT(ENTITY_EVENT_COLLISION);
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
    case ENTITY_EVENT_EDITOR_PROPERTY_CHANGED:
    {
       
    }
    }
}