#pragma once
#include "BaseCustomWeapon.h"
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>


class CShieldWeaponComponent :public BaseCustomWeapon
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CShieldWeaponComponent>& desc)
    {
        desc.SetGUID("{5FC0D1FC-6BE6-4904-9F05-3C5F4AB60F25}"_cry_guid);
        desc.AddBase<BaseCustomWeapon>();

        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Shield");

        desc.AddMember(&CShieldWeaponComponent::m_weaponHandType, 'whnd', "WeaponHandType", "Weapon Hand Type", "Weapon Hand Type", EWeaponHandType::LeftHand);

    }

   
    void SetBlock(bool isEnable);
protected:
    virtual void StartGame();

    Vec3 ColliderSize = Vec3(1,1,1);
    Cry::DefaultComponents::CBoxPrimitiveComponent* m_pCollider;
    pe_params params;
};
