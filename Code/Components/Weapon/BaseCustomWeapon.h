#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/RigidBodyComponent.h>
#include "WeaponExtensionData.h"


class BaseCustomWeapon : public IEntityComponent
{
public:

    static void ReflectType(Schematyc::CTypeDesc<BaseCustomWeapon>& desc)
    {
        desc.SetGUID("{A61885BA-9C35-4D2C-844B-DE3036D36851}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Weapon Interface");
        // desc.AddMember(&BaseCustomWeapon::m_physType, 'phys', "PhysType", "Phys Type", "Phys Type", PE_RIGID);

    }

    Cry::Entity::EntityEventMask GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& event);

    EWeaponHandType GetWeaponHand() const { return m_weaponHandType; }

    void DetachPhysicalize();
    pe_type m_physType;

protected:

    virtual void StartGame();
    virtual void Update(float fFrameTime);

    virtual void Initialize();
    EWeaponHandType m_weaponHandType;
    Cry::DefaultComponents::CRigidBodyComponent* m_pRigidBodyComponent = nullptr;

};

