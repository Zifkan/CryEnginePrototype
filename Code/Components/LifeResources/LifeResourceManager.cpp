#include "StdAfx.h"
#include "LifeResourceManager.h"
#include <CryEntitySystem/IEntity.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


static void RegisterCLifeResourceManager(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CLifeResourceManagerComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCLifeResourceManager)

void CLifeResourceManagerComponent::RegisterResource(std::type_index index,IBaseLifeResource* resource)
{
    m_lifeResources.insert(std::pair<std::type_index, IBaseLifeResource*>(index, resource));
}

Cry::Entity::EventFlags CLifeResourceManagerComponent::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void CLifeResourceManagerComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_UPDATE:
    {
        SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
        Update(pCtx->fFrameTime);
    }
    break;
    }
}

void CLifeResourceManagerComponent::Update(float timePassed)
{
    for (auto const& resource : m_lifeResources)
    {
        resource.second->Update(timePassed);
    }
}
