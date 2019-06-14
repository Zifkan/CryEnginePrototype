#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "MeleeWeapon.h"
#include "CharacterActions.h"
#include <CryAnimation/IAttachment.h>
#include <CrySchematyc/Utils/SharedString.h>

class CWeaponSystem : public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CWeaponSystem>& desc)
    {
        desc.SetGUID("{A1E52698-3DBE-473D-B632-2930FF84E673}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Weapon System");
        desc.SetDescription("Controlls weapons");

        desc.AddMember(&CWeaponSystem::m_weaponSlotName, 'wslt', "WeaponSlotName", "Weapon Slot Name", "Weapon Slot Name", "r_weapon");
    }

    void Init(ICharacterActions* characterActions, IAttachmentManager* attachmentManager);
   
private:
    CMeleeWeaponComponent* m_pMeleeWeapon = nullptr;
    ICharacterActions* m_pCharacterActions = nullptr;
    IAttachmentManager* m_pAttachmentManager = nullptr;
    Schematyc::CSharedString m_weaponSlotName = "r_weapon";

    //   rxcpp::composite_subscription m_rayHitSubjectSubscription;

    void AttachToHand();
};
