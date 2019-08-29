#include "StdAfx.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "PlayerSoul.h"
#include <CryCore/StaticInstanceList.h>

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

Cry::Entity::EventFlags CPlayerSoul::GetEventMask() const
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
    float diffuseMultiplier = m_pPoinLight->GetColorParameters().m_diffuseMultiplier.value;

     m_lifeTimeCycleSubscription = m_lifeTimeCycle.get_observable()
    .scan(m_lifeTimeLimit,[](float timeLimit, float frameTime){return timeLimit - frameTime;})
    .take_while([](float v) {return v > 0.01; })
    .subscribe([this, diffuseMultiplier](float time)
    { 
         CryLog("Result: %f", diffuseMultiplier * (time / m_lifeTimeLimit));
         m_pPoinLight->GetColorParameters().m_diffuseMultiplier.value = diffuseMultiplier * (time / m_lifeTimeLimit);
         

        /* SRenderLight light;

      

         light.SetPosition(ZERO);
         light.m_Flags = DLF_DEFERRED_LIGHT | DLF_POINT;

         light.SetLightColor(m_pPoinLight->GetColorParameters().m_color * diffuseMultiplier * (time / m_lifeTimeLimit));
         light.SetSpecularMult(diffuseMultiplier * (time / m_lifeTimeLimit));

         const int slot = m_pEntity->LoadLight(GetOrMakeEntitySlotId(), &light);*/
    });

}
