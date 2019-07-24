#pragma once
#include <CryEntitySystem/IEntityComponent.h>

class ICustomWeapon : public IEntityComponent
{
public:
    
    static void ReflectType(Schematyc::CTypeDesc<ICustomWeapon>& desc)
    {
        desc.SetGUID("{A61885BA-9C35-4D2C-844B-DE3036D36851}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Weapon Interface");

    }

    EWeaponHandType GetWeaponHand() const { return m_weaponHandType; }
protected:
    EWeaponHandType m_weaponHandType;
};
