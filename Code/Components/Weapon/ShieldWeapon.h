#pragma once
#include <CryEntitySystem/IEntityComponent.h>

class CShieldWeaponComponent :public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CShieldWeaponComponent>& desc)
    {
        desc.SetGUID("{5FC0D1FC-6BE6-4904-9F05-3C5F4AB60F25}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Shield");
    }
};
