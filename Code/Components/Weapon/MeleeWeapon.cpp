#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "MeleeWeapon.h"
#include "WeaponSystem.h"

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


void CMeleeWeaponComponent::Attack()
{
    m_isAttack = true;
}

void CMeleeWeaponComponent::StopAttack()
{
    m_isAttack = false;
}

void CMeleeWeaponComponent::Init(WeaponSystemComponent* weaponSystem, IEntity* secondWeapon)
{   
    m_pSkipEnts[0] = weaponSystem->GetEntity()->GetPhysicalEntity();
    m_pSkipEnts[1] = m_pEntity->GetPhysicalEntity();
    m_pSkipEnts[3] = secondWeapon->GetPhysicalEntity();
}

void CMeleeWeaponComponent::Update(float fFrameTime)
{
    if (!m_isAttack ) return;

    ray_hit rayhit;
    
    // Perform the ray cast.
    int hits = gEnv->pPhysicalWorld->RayWorldIntersection(m_pEntity->GetWorldPos()+m_rayOffset,  Quat(m_rayAngleRotation)* m_pEntity->GetForwardDir()*m_rayLength,
        ent_static | ent_sleeping_rigid | ent_rigid | ent_independent | ent_terrain, rwi_stop_at_pierceable | rwi_colltype_any,
        &rayhit, 1, m_pSkipEnts, 2);


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
