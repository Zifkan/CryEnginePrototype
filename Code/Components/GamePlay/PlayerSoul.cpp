#include "StdAfx.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "PlayerSoul.h"

static void RegisterCPlayerSoul(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerSoul));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCPlayerSoul)

void CPlayerSoul::Initialize()
{
    m_lifeTimeLimit = 10.f;
    m_pPoinLight = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CPointLightComponent>();
}

Cry::Entity::EntityEventMask CPlayerSoul::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED) | ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
}

void CPlayerSoul::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {
        LifeTimeReaction();
    }
    break;
    case ENTITY_EVENT_UPDATE:
    {
        SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
        m_lifeTimeCycle.get_subscriber().on_next(pCtx->fFrameTime);
    }
    break;
    case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
    {
      
    }
    break;
    case ENTITY_EVENT_RESET:
    {
        if (event.nParam[0]==0)
            m_lifeTimeCycleSubscription.unsubscribe();
    }
    break;
    }
}

void CPlayerSoul::LifeTimeReaction()
{   
     m_lifeTimeCycleSubscription = m_lifeTimeCycle.get_observable().scan(m_lifeTimeLimit,[](float v, float time){return v - time;})
    .take_while([](float v) {return v > 0.01; })
    .subscribe([this](float time)
    {    
         auto temp = static_cast<float>(m_pPoinLight->GetColorParameters().m_diffuseMultiplier.value)/ time;       
        m_pPoinLight->GetColorParameters().m_diffuseMultiplier.value -= temp;
        m_pPoinLight->SetOptics("");
    });

}
