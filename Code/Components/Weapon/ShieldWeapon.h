#pragma once
#include "ICustomWeapon.h"

class CShieldWeaponComponent :public ICustomWeapon
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CShieldWeaponComponent>& desc)
    {
        desc.SetGUID("{5FC0D1FC-6BE6-4904-9F05-3C5F4AB60F25}"_cry_guid);
        desc.AddBase<ICustomWeapon>();

        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Shield");

        desc.AddMember(&CShieldWeaponComponent::m_weaponHandType, 'whnd', "WeaponHandType", "Weapon Hand Type", "Weapon Hand Type", EWeaponHandType::LeftHand);

    }

};
