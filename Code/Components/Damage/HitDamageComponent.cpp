﻿#include "StdAfx.h"
#include "HitDamageComponent.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include "Components/LifeResources/HealthLifeResource.h"
#include <Components/Weapon/WeaponExtensionData.h>

static void RegisterHitDamageComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHitDamageComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHitDamageComponent)


uint64 CHitDamageComponent::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}


void CHitDamageComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {
        m_pLifeResourceManager = m_pEntity->GetComponent<CLifeResourceManagerComponent>();
        m_pAdvancedAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
    }
    break;
    }
}


void CHitDamageComponent::OnHit(SWeaponHitStruct hitStruct)
{  
    auto pHealthLifeResource = m_pLifeResourceManager->GetResource<CHealthLifeResource>();
    
    pHealthLifeResource->ChangeValue(-hitStruct.Damage);

    if (pHealthLifeResource->Value.get_value() <= 0.0f) return;

    const auto dotProduct = hitStruct.HitDirection.dot(m_pEntity->GetForwardDir());

    auto pCharacter = m_pAdvancedAnimationComponent->GetCharacter();
    ISkeletonPose* pSkelPose = pCharacter->GetISkeletonPose();
    IDefaultSkeleton& rIDefaultSkeleton = pCharacter->GetIDefaultSkeleton();
   // CryLog("Bone: %s", rIDefaultSkeleton.GetJointNameByID(hitStruct.PartId));
   // pSkelPose->GetAbsJointByID(hitStruct.PartId);



    SideHitEnum hitSide;
    if (dotProduct > 0)
    {
       //"from front"
        if (hitStruct.Hitpoint.x > m_pEntity->GetWorldPos().x)
        {
            hitSide = LeftSide;
            //left Anim
        }
        else
        {
            hitSide = RightSide;
            //right Anim
        }
    }
    else
    {
        //"from back"
        if (hitStruct.Hitpoint.x > m_pEntity->GetWorldPos().x)
        {
            hitSide = RightSide;
            //right Anim
        }
        else
        {
            hitSide = LeftSide;
            //left Anim          
            
        }       
    }    

    HitSubject.get_subscriber().on_next(hitSide);
}
