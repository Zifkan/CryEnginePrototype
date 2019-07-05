#include "StdAfx.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include "AIAction.h"

static void RegisterAIActionComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(AIActionComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIActionComponent)


void AIActionComponent::SetMovementForward(float y)
{
}

void AIActionComponent::SetMovementSide(float x)
{
}

void AIActionComponent::SetAttack(bool isAttack)
{
}

void AIActionComponent::SetRotateYaw(float x)
{
}

void AIActionComponent::SetRotatePitch(float y)
{
}

void AIActionComponent::SetMoveSignal(bool signal)
{
}

EntityEventMask AIActionComponent::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void AIActionComponent::ProcessEvent(const SEntityEvent& event)
{

}
