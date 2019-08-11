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

    if (secondWeapon!=nullptr)
        m_pSkipEnts[2] = secondWeapon->GetPhysicalEntity();
}

void CMeleeWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
    BaseCustomWeapon::ProcessEvent(event);
   
}

void CMeleeWeaponComponent::Update(float fFrameTime)
{    
   
    if (!m_isAttack) return;
    ray_hit rayhit[RAY_HIT_COUNT];
    
    // Perform the ray cast.
    int hits = gEnv->pPhysicalWorld->RayWorldIntersection(m_pEntity->GetWorldPos()+m_rayOffset,  Quat(m_rayAngleRotation)* m_pEntity->GetForwardDir()*m_rayLength,
        ent_static | ent_sleeping_rigid | ent_rigid | ent_independent , rwi_max_piercing,
        rayhit, 3, m_pSkipEnts, 10);  


 /* 
    primitives::box box;
    box.center = GetEntity()->GetWorldPos();
    box.size = Vec3(1.f, 1.f, 1.f);
    box.Basis.SetRotationXYZ(GetEntity()->GetWorldAngles());

    geom_contact *pContact = 0;
    float hitDist = gEnv->pPhysicalWorld->PrimitiveWorldIntersection(box.type, &box, (Vec3(.0f, .0f, -1.f) * 10.f), ent_all, &pContact, 0, rwi_colltype_any | rwi_stop_at_pierceable, 0, 0, 0, m_pSkipEnts,0);

*/

    if (hits > 1)
    {
        std::array<ray_hit, RAY_HIT_COUNT> sendRayHit;
        std::copy(std::begin(rayhit), std::end(rayhit), std::begin(sendRayHit));

        RayHitSubject.get_subscriber().on_next(sendRayHit);
    }


    return;

    auto center = GetEntity()->GetWorldPos() + m_boxOffset.x* GetEntity()->GetRightDir() + m_boxOffset.y* GetEntity()->GetForwardDir() + m_boxOffset.z* GetEntity()->GetUpDir();
    primitives::box box;
    box.size = m_boxSize;
    box.Basis.SetRotationVDir(GetEntity()->GetWorldPos()- center);
    box.bOriented = 1;
    box.center = center;

    float n = 0.0f;
    geom_contact *contacts;
    intersection_params params;
    WriteLockCond lockContacts;
    params.bStopAtFirstTri = false;
    params.bNoBorder = true;
    params.bNoAreaContacts = true;
    n = gEnv->pPhysicalWorld->PrimitiveWorldIntersection(primitives::box::type, &box, Vec3(ZERO),
        ent_rigid | ent_sleeping_rigid | ent_independent | ent_static | ent_terrain | ent_water, &contacts, 0,
        geom_colltype0 | geom_colltype_foliage | geom_colltype_player, &params, 0, 0, m_pSkipEnts, 3, &lockContacts);

    if (!m_isAttack) return;

    int ret = (int)n;
    CryLog("hit count: %i", ret);
    float closestdSq = 9999.0f;
    geom_contact *closestc = 0;
    geom_contact *currentc = contacts;
    for (int i = 0; i < ret; i++)
    {
        geom_contact *contact = currentc;
        if (contact)
        {
            IPhysicalEntity *pCollider = gEnv->pPhysicalWorld->GetPhysicalEntityById(contact->iPrim[0]);
            if (pCollider)
            {
                IEntity *pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pCollider);
                if (pEntity)
                {
                    if (pEntity == GetEntity())
                    {
                        ++currentc;
                        continue;
                    }
                }

                float distSq = (GetEntity()->GetWorldPos() - currentc->pt).len2();
                if (distSq < closestdSq)
                {
                    closestdSq = distSq;
                    closestc = contact;
                }
            }
        }
        ++currentc;
    }

    if (closestc)
    {
        IPhysicalEntity *pCollider = gEnv->pPhysicalWorld->GetPhysicalEntityById(closestc->iPrim[0]);
        IEntity *pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pCollider);
        if (pEntity)
        {
     //       CryLog("EntityName: %s", pEntity->GetName());
        }
    }

}
