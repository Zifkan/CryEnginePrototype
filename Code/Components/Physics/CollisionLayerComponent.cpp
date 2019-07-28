#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

#include <CryPhysics/physinterface.h>
#include "CollisionLayerComponent.h"


static void RegisterCollisionLayerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCollisionLayerComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCollisionLayerComponent)

Cry::Entity::EntityEventMask CCollisionLayerComponent::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME);
}

void CCollisionLayerComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {
        IPhysicalEntity* pPhysicalEntity = m_pEntity->GetPhysicalEntity();

        if (!pPhysicalEntity)
            return;

        pe_params_collision_class paramsCollision;
        paramsCollision.collisionClassAND.type = 0;
        paramsCollision.collisionClassAND.ignore = 0;
        for (int i = 0; i < m_type.Size(); ++i)
        {
            paramsCollision.collisionClassAND.type |= static_cast<uint32>(m_type.At(i));
            paramsCollision.collisionClassOR.type  |= static_cast<uint32>(m_type.At(i));
        }
        for (int i = 0; i < m_ignore.Size(); ++i)
        {
            paramsCollision.collisionClassAND.ignore |= static_cast<uint32>(m_type.At(i));
            paramsCollision.collisionClassOR.ignore  |= static_cast<uint32>(m_type.At(i));
        }
        pPhysicalEntity->SetParams(&paramsCollision);
    }
    break;
    }
  
}