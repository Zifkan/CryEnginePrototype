#include "StdAfx.h"
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>

#include "MeleeWeapon.h"

static void RegisterCMeleeWeaponComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CMeleeWeaponComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCMeleeWeaponComponent)

uint64 CMeleeWeaponComponent::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void CMeleeWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {

    }
    break;
    case ENTITY_EVENT_UPDATE:
    {
        SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
        Update(pCtx->fFrameTime);
    }
    break;
    case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
    {
    
    }
    break;
    }
}

void CMeleeWeaponComponent::Attack()
{
    m_isAttack = true;
}

void CMeleeWeaponComponent::Update(float fFrameTime)
{
  //  if (!m_isAttack ) return;

    ray_hit rayhit;
    static IPhysicalEntity* pSkipEnts[10];
    
    // Perform the ray cast.
    int hits = gEnv->pPhysicalWorld->RayWorldIntersection(m_pEntity->GetWorldPos()+m_rayOffset,  Quat(m_rayAngleRotation)* m_pEntity->GetForwardDir()*m_rayLength,
        ent_static | ent_sleeping_rigid | ent_rigid | ent_independent | ent_terrain, rwi_stop_at_pierceable | rwi_colltype_any,
        &rayhit, 1, pSkipEnts, 2);


    //primitives::box box;
    //box.center = worldPos;
    //box.size = wallSafeDistance;

    //geom_contact *pContact = 0;
    //float hitDist = gEnv->pPhysicalWorld->PrimitiveWorldIntersection(box.type, &box, Quat(m_rayAngleRotation)* m_pEntity->GetForwardDir(), 
    //    ent_static | ent_terrain | ent_rigid | ent_sleeping_rigid,  &pContact, 0, geom_colltype_player, 0, 0, 0, pSkipEnts, 2);

    DetectHit(rayhit, hits);
    


    m_attackDetectionTimer += fFrameTime;

    if(m_attackDetectionTimer>= m_attackDetectionTimeLimit)
    {
        m_attackDetectionTimer = 0.0f;
        m_isAttack = false;
    }
}

void CMeleeWeaponComponent::DetectHit(const ray_hit rayhit,const int hits)
{
    if (hits == 0) return;

    RayHitSubject.get_subscriber().on_next(rayhit);
}
