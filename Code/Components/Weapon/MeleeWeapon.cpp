﻿#include "StdAfx.h"
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
        m_pSkipEnts[3] = secondWeapon->GetPhysicalEntity();
}

void CMeleeWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
    BaseCustomWeapon::ProcessEvent(event);
    switch (event.event)
    {
    case ENTITY_EVENT_COLLISION:
    {
        return;
        if (!m_isAttack) return;

        // Get the EventPhysCollision structure describing the collision that occurred
         const EventPhysCollision* pPhysCollision = reinterpret_cast<const EventPhysCollision*>(event.nParam[0]);
        // The EventPhysCollision provides information for both the source and target entities
        // Therefore, we store the indices of this entity (and the other collider)
        // This can for example be used to get the surface identifier (and thus the material) of the part of our entity that collided
        const int thisEntityIndex = static_cast<int>(event.nParam[1]);
        // Calculate the index of the other entity
        const int otherEntityIndex = (thisEntityIndex + 1) % 2;
        if (otherEntityIndex <= 0) return;
        // Get the contact point (in world coordinates) of the two entities
        const Vec3& contactPoint = pPhysCollision->pt;
        // Get the collision normal vector
        const Vec3& contactNormal = pPhysCollision->n;
        // Get properties for our entity, starting with the local velocity of our entity at the contact point
        const Vec3& relativeContactVelocity = pPhysCollision->vloc[thisEntityIndex];
        // Get the mass of our entity
        const float contactMass = pPhysCollision->mass[thisEntityIndex];
        // Get the identifier of the part of our entity that collided
        // This is the same identifier that is added with IPhysicalEntity::AddGeometry
        const int contactPartId = pPhysCollision->partid[thisEntityIndex];
        // Get the surface on our entity that collided
        const int contactSurfaceId = pPhysCollision->idmat[thisEntityIndex];

        const int primId = pPhysCollision->iPrim[otherEntityIndex];
        // Get the ISurfaceType representation of the surface that collided
        if (const ISurfaceType* pContactSurface = gEnv->p3DEngine->GetMaterialManager()->GetSurfaceType(contactSurfaceId))
        {
            /* Interact with pContactSurface here*/
        }
        auto physEntity = gEnv->pPhysicalWorld->GetPhysicalEntityById(contactPartId);
        auto temp = gEnv->pEntitySystem->GetEntity(primId);
        if (temp!=nullptr)
            CryLog("Sword Collision detection: %s", temp->GetName());
    }
    break;
    }
}

void CMeleeWeaponComponent::Update(float fFrameTime)
{
    if (!m_isAttack ) return;

    ray_hit rayhit[3];
    
    // Perform the ray cast.
    int hits = gEnv->pPhysicalWorld->RayWorldIntersection(m_pEntity->GetWorldPos()+m_rayOffset,  Quat(m_rayAngleRotation)* m_pEntity->GetForwardDir()*m_rayLength,
        ent_static | ent_sleeping_rigid | ent_rigid | ent_independent , rwi_max_piercing,
        rayhit, 3, m_pSkipEnts, 10);


    //primitives::box box;
    //box.center = worldPos;
    //box.size = wallSafeDistance;

    //geom_contact *pContact = 0;
    //float hitDist = gEnv->pPhysicalWorld->PrimitiveWorldIntersection(box.type, &box, Quat(m_rayAngleRotation)* m_pEntity->GetForwardDir(), 
    //    ent_static | ent_terrain | ent_rigid | ent_sleeping_rigid,  &pContact, 0, geom_colltype_player, 0, 0, 0, pSkipEnts, 2);

    if (hits >1)
    {     

        for (int i = 0; i < 3; ++i)
        {
            auto hit = rayhit[i];
            IPhysicalEntity* pHitEntity = hit.pCollider;

            if (pHitEntity != nullptr)
            {
                IEntity* pHitedEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pHitEntity);
                if (pHitedEntity != nullptr)
                {
                    CryLog("Hitted Entity: %s", pHitedEntity->GetName());
                }            
            }
        }
        CryLog("*************************");
    }
       
  //  RayHitSubject.get_subscriber().on_next(rayhit);

   
}
