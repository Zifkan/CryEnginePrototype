#pragma once
#include <CryEntitySystem/IEntitySystem.h>
#include <CryEntitySystem/IEntityComponent.h>
#include "RxCpp/rx.hpp"
#include <CryPhysics/physinterface.h>

class CMeleeWeaponComponent :public IEntityComponent
{
      
public:
    virtual uint64 GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& event) override;
    // ~IEntityComponent

    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<CMeleeWeaponComponent>& desc)
    {
        desc.SetGUID("{0BF0A12F-E727-4238-9E62-57FACA3F8231}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.AddMember(&CMeleeWeaponComponent::m_rayLength, 'rlen', "WeaponRayLength", "Weapon Ray length", "Weapon Ray length", 1.0f);
        desc.AddMember(&CMeleeWeaponComponent::m_rayOffset, 'roff', "WeaponRayOffset", "Weapon Ray offset", "Weapon Ray offset", Vec3(0,0,0));
        desc.AddMember(&CMeleeWeaponComponent::m_rayAngleRotation, 'rrot', "WeaponRayRotation", "Weapon Ray rotation", "Weapon Ray rotation", Vec3(0, 0, 0));
        desc.AddMember(&CMeleeWeaponComponent::m_attackDetectionTimeLimit, 'tdet', "AttackDetectionTime", "Attack Time", "Weapon Detection Attack Time", 1);
        desc.AddMember(&CMeleeWeaponComponent::m_damage, 'dama', "WeaponDamage", "Weapon Damage", "Weapon Damage", 10);

                      
    }

    void Attack();

    float GetWeaponDamage()
    {
        return m_damage;
    }

    rxcpp::subjects::subject<ray_hit> RayHitSubject = rxcpp::subjects::subject<ray_hit>();

private:
    float m_rayLength;
    Vec3 m_rayOffset;
    Vec3 m_rayAngleRotation;

    void Update(float fFrameTime);
    void DetectHit(const ray_hit rayhit, const int hits);

    float m_damage = 50.0f;
    float m_attackDetectionTimer = 0.0f;
    float m_attackDetectionTimeLimit = 1;;
    bool m_isAttack = false;
};
