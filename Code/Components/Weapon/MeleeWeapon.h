#pragma once
#include <CryPhysics/physinterface.h>
#include "RxCpp/rx.hpp"
#include "WeaponExtensionData.h"
#include "BaseCustomWeapon.h"

class WeaponSystemComponent;

class CMeleeWeaponComponent :public BaseCustomWeapon
{    
 
public:  
 
    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<CMeleeWeaponComponent>& desc)
    {
        desc.SetGUID("{0BF0A12F-E727-4238-9E62-57FACA3F8231}"_cry_guid);
        desc.AddBase<BaseCustomWeapon>();

        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Melee Weapon");
        desc.AddMember(&CMeleeWeaponComponent::m_rayLength, 'rlen', "WeaponRayLength", "Weapon Ray length", "Weapon Ray length", 1.0f);
        desc.AddMember(&CMeleeWeaponComponent::m_rayOffset, 'roff', "WeaponRayOffset", "Weapon Ray offset", "Weapon Ray offset", Vec3(0,0,0));
        desc.AddMember(&CMeleeWeaponComponent::m_rayAngleRotation, 'rrot', "WeaponRayRotation", "Weapon Ray rotation", "Weapon Ray rotation", Vec3(0, 0, 0));
        desc.AddMember(&CMeleeWeaponComponent::m_damage, 'dama', "WeaponDamage", "Weapon Damage", "Weapon Damage", 10);
        desc.AddMember(&CMeleeWeaponComponent::m_weaponHandType, 'whnd', "WeaponHandType", "Weapon Hand Type", "Weapon Hand Type", EWeaponHandType::RightHand);

    }

    void Init(WeaponSystemComponent* weaponSystem,IEntity* secondWeapon);

    void Attack();
    void StopAttack();

    float GetWeaponDamage(){ return m_damage; }

    rxcpp::subjects::subject<ray_hit[3]> RayHitSubject = rxcpp::subjects::subject<ray_hit[3]>();


    void ProcessEvent(const SEntityEvent& event) override;

protected:
    void Update(float fFrameTime) override;

private:

    float m_rayLength{};
    Vec3 m_rayOffset;
    Vec3 m_rayAngleRotation;  
    float m_damage = 50.0f;
    bool m_isAttack = false;
    IPhysicalEntity* m_pSkipEnts[10] = {};

};