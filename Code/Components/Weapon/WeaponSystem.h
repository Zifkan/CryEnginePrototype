#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "Components/Inputs/Actions/CharacterActions.h"
#include <CryAnimation/IAttachment.h>
#include <CrySchematyc/Utils/SharedString.h>
#include "WeaponHitStruct.h"
#include "MeleeWeapon.h"
#include "RxCpp/rx.hpp"

class CWeaponSystemComponent : public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CWeaponSystemComponent>& desc)
    {
        desc.SetGUID("{A1E52698-3DBE-473D-B632-2930FF84E673}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Weapon System");
        desc.SetDescription("Controlls weapons");

        desc.AddMember(&CWeaponSystemComponent::m_rightWeaponSlotName, 'rslt', "RightWeaponSlotName", "Right hand weapon slot Name", "Right hand weapon slot Name", "r_weapon");
        desc.AddMember(&CWeaponSystemComponent::m_leftWeaponSlotName,  'lslt', "LeftWeaponSlotName", "Left hand weapon slot Name", "Left hand weapon slot Name", "l_weapon");
    }

    void Init(ICharacterActions* characterActions, IAttachmentManager* attachmentManager);

    rxcpp::subjects::subject<SWeaponHitStruct> HitReactionSubject = rxcpp::subjects::subject<SWeaponHitStruct>();

private:
    CMeleeWeaponComponent* m_pRightHandWeapon = nullptr; 
    CMeleeWeaponComponent* m_pLeftHandWeapon = nullptr;
    ICharacterActions* m_pCharacterActions = nullptr;
    IAttachmentManager* m_pAttachmentManager = nullptr;
    Schematyc::CSharedString m_rightWeaponSlotName = "r_weapon";
    Schematyc::CSharedString m_leftWeaponSlotName  = "l_weapon";

    void AttachToRightHand();

    void HitDetection();
};
