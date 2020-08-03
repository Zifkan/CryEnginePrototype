#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryAnimation/IAttachment.h>
#include <CrySchematyc/Utils/SharedString.h>
#include "Components/Weapon/MeleeWeapon.h"
#include "Components/Weapon/ShieldWeapon.h"
#include "Components/Weapon/WeaponExtensionData.h"


class WeaponSystemComponent : public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<WeaponSystemComponent>& desc)
    {
        desc.SetGUID("{A1E52698-3DBE-473D-B632-2930FF84E673}"_cry_guid);
        desc.SetEditorCategory("Weapon");
        desc.SetLabel("Weapon System");
        desc.SetDescription("Controlls weapons");

        desc.AddMember(&WeaponSystemComponent::m_rightWeaponSlotName, 'rslt', "RightWeaponSlotName", "Right hand weapon slot Name", "Right hand weapon slot Name", "r_weapon");
        desc.AddMember(&WeaponSystemComponent::m_leftWeaponSlotName,  'lslt', "LeftWeaponSlotName", "Left hand weapon slot Name", "Left hand weapon slot Name", "l_weapon");
       // desc.AddMember(&WeaponSystemComponent::m_explicitEntityId, 'enid', "ExplicitEntityId", "ExplicitEntityId", "ExplicitEntityId", Schematyc::ExplicitEntityId::Invalid);
    }

    void Init(IAttachmentManager* attachmentManager);
    void DetachWeapons() const;

  

    void StopAttack();

private:
    CMeleeWeaponComponent* m_pRightHandWeapon = nullptr; 
    CShieldWeaponComponent* m_pLeftHandWeapon = nullptr;
  
    IAttachmentManager* m_pAttachmentManager = nullptr;
    Schematyc::CSharedString m_rightWeaponSlotName = "r_weapon";
    Schematyc::CSharedString m_leftWeaponSlotName  = "l_weapon";


    IAttachment* m_pRightHandAttachment;
    IAttachment* m_pLeftHandAttachment;

    void SetAttach(IEntityLink* pLink);
    void AttachToRightHand();
    void AttachToLeftHand();
    void HitDetection();

    //Schematyc::ExplicitEntityId m_explicitEntityId;
};
