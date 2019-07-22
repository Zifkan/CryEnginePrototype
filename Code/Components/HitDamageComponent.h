#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "WeaponHitStruct.h"
#include "LifeResources/LifeResourceManager.h"
#include "CharacterComponent.h"

enum SideHitEnum
{
    LeftSide,
    RightSide,
    PushBack,
};

class CHitDamageComponent final :  public IEntityComponent
{
public:

    virtual Cry::Entity::EntityEventMask GetEventMask() const;

    static void ReflectType(Schematyc::CTypeDesc<CHitDamageComponent>& desc)
    {
        desc.SetGUID("{AA6D3914-86A9-4FEB-B89A-CA8E589C657A}"_cry_guid);
        desc.SetLabel("Hit Damage Controller");
        desc.SetEditorCategory("Damage");
        desc.SetComponentFlags({ EFlags::Singleton });
    }

    rxcpp::subjects::subject<SideHitEnum> HitSubject = rxcpp::subjects::subject<SideHitEnum>();


    void OnHit(SWeaponHitStruct hitStruct);

protected:
    virtual void ProcessEvent(const SEntityEvent& event);

    CLifeResourceManagerComponent* m_pLifeResourceManager = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAdvancedAnimationComponent = nullptr;
};
