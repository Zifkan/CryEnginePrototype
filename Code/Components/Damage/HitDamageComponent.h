#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "Components/Weapon/WeaponExtensionData.h"
#include "Components/LifeResources/LifeResourceManager.h"
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include "RxCpp/rx.hpp"


enum SideHitEnum
{
    LeftSide,
    RightSide,
    PushBack,
};

class CHitDamageComponent final :  public IEntityComponent
{
public:

    virtual Cry::Entity::EventFlags GetEventMask() const;

    static void ReflectType(Schematyc::CTypeDesc<CHitDamageComponent>& desc)
    {
        desc.SetGUID("{AA6D3914-86A9-4FEB-B89A-CA8E589C657A}"_cry_guid);
        desc.SetLabel("Hit Damage Controller");
        desc.SetEditorCategory("Damage");
        desc.SetComponentFlags({ EFlags::Singleton });

        desc.AddMember(&CHitDamageComponent::m_hittedStatusDuration, 'hsts', "HitDuration", "Hitted Status Duration", "Duration of Character been hitted", 1.5f);
    }

    rxcpp::subjects::subject<SideHitEnum> HitSubject = rxcpp::subjects::subject<SideHitEnum>();


    void OnHit(SWeaponHitStruct hitStruct);



protected:
    virtual void ProcessEvent(const SEntityEvent& event);

    CLifeResourceManagerComponent* m_pLifeResourceManager = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAdvancedAnimationComponent = nullptr;

  
    float m_hittedStatusDuration;
    float m_hittedTimer;
};
