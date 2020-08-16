#include "ConvertToEntity.h"
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "Core/CryEcsWorld.h"

static void RegisterCameraController(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CConvertToEntity));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCameraController)


Cry::Entity::EventFlags CConvertToEntity::GetEventMask() const
{
    return ENTITY_EVENT_START_GAME  | ENTITY_EVENT_EDITOR_PROPERTY_CHANGED;
}

void CConvertToEntity::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {    
        auto* world = CryEcsWorld::instance(); 
        auto e = world->EntityManager->CreateEntity(entityName);
        Convert(e, *world->EntityManager);
    }
    break;
    }
}
