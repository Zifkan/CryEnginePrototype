#include  "StdAfx.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include "BaseLifeComponent.h"



uint64 IBaseLifeResource::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void IBaseLifeResource::ProcessEvent(const SEntityEvent& event)
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
        Update();
    }
    break;
    case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
    {
        PropertyChange();
    }
    break;
    }

}
void IBaseLifeResource::SetValue(float value)
{
    Value.get_subscriber().on_next(value);
}

void IBaseLifeResource::StartGame()
{
    SetValue(m_maxValue);
}

void IBaseLifeResource::Update()
{
}

void IBaseLifeResource::PropertyChange()
{

}
