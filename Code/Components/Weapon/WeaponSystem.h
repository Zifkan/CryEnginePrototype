#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "CharacterActions.h"
#include <CryAnimation/IAttachment.h>
#include <CrySchematyc/Utils/SharedString.h>
#include "Components/WeaponHitStruct.h"


class CMeleeWeaponComponent;

class CWeaponSystemComponent : public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CWeaponSystemComponent>& desc)
    {
        desc.SetGUID("{A1E52698-3DBE-473D-B632-2930FF84E673}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Weapon System");
        desc.SetDescription("Controlls weapons");

        desc.AddMember(&CWeaponSystemComponent::m_weaponSlotName, 'wslt', "WeaponSlotName", "Weapon Slot Name", "Weapon Slot Name", "r_weapon");
    }

    void Init(ICharacterActions* characterActions, IAttachmentManager* attachmentManager);

    rxcpp::subjects::subject<SWeaponHitStruct> HitReactionSubject = rxcpp::subjects::subject<SWeaponHitStruct>();

private:
    CMeleeWeaponComponent* m_pMeleeWeapon = nullptr;
    ICharacterActions* m_pCharacterActions = nullptr;
    IAttachmentManager* m_pAttachmentManager = nullptr;
    Schematyc::CSharedString m_weaponSlotName = "r_weapon";

    void AttachToHand();

    void HitDetection();
};
