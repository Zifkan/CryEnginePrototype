﻿#include "StdAfx.h"
#include "WeaponSystem.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "StateMachine/StateAction/PushBackAction.h"
#include "Components/Characters/PlayerComponent.h"

static void RegisterWeaponSystem(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(WeaponSystemComponent));
        // Functions
        {
        }
    }
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeaponSystem)

void WeaponSystemComponent::Init(ICharacterActions* characterActions, IAttachmentManager* attachmentManager)
{
    if (m_pCharacterActions == nullptr)
        m_pCharacterActions = characterActions;

    if (m_pAttachmentManager == nullptr)
        m_pAttachmentManager = attachmentManager;

    m_pCharacterActions->AttackSubject.get_observable().subscribe([this](AttackType type)
    {
        m_pRightHandWeapon->Attack();
    });

    IEntityLink* pLink = m_pEntity->GetEntityLinks();
    SetAttach(pLink);
}



void WeaponSystemComponent::SetAttach(IEntityLink* pLink)
{    
    if (pLink)
    {
        IEntity* pEntity = gEnv->pEntitySystem->GetEntity(pLink->entityId);
        if (pEntity)
        {
            ICustomWeapon* weaponType = pEntity->GetComponent<ICustomWeapon>();

           
            switch (weaponType->GetWeaponHand())
            {
            case EWeaponHandType::LeftHand:
            {
                m_pLeftHandWeapon = pEntity->GetComponent <CShieldWeaponComponent>();
                CRY_ASSERT(m_pLeftHandWeapon != nullptr);
                AttachToLeftHand();
                SetAttach(pLink->next);
            }
            break;
            case EWeaponHandType::RightHand:
            {
                m_pRightHandWeapon = pEntity->GetComponent <CMeleeWeaponComponent>();
                CRY_ASSERT(m_pRightHandWeapon != nullptr);
                AttachToRightHand();
                HitDetection();
                SetAttach(pLink->next);
            }
            break;
            }          
        }
        else
        {
            CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "There is no weapon linked to character");
        }
    }

    m_pRightHandWeapon->Init(this, m_pLeftHandWeapon->GetEntity());
}


void WeaponSystemComponent::AttachToRightHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pRightHandWeapon->GetEntityId());

    auto *attachment = m_pAttachmentManager->GetInterfaceByName(m_rightWeaponSlotName.c_str());
   
    attachment->AddBinding(attachmentItem);   
}

void WeaponSystemComponent::AttachToLeftHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pLeftHandWeapon->GetEntityId());

    auto *attachment = m_pAttachmentManager->GetInterfaceByName(m_leftWeaponSlotName.c_str());

    attachment->AddBinding(attachmentItem);
}

void WeaponSystemComponent::HitDetection()
{
    m_pRightHandWeapon->RayHitSubject.get_observable().subscribe([this](ray_hit hit)
    {
        IPhysicalEntity* pHitEntity = hit.pCollider;
        IEntity* pHitedEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pHitEntity);
       
        if (pHitedEntity!=nullptr)
        {
            CHitDamageComponent* pHitDamageComponent = pHitedEntity->GetComponent<CHitDamageComponent>();

            if (pHitedEntity->GetGuid() != m_pEntity->GetGuid() && pHitDamageComponent != nullptr)
            {
                SWeaponHitStruct hitStruct;
                hitStruct.Damage = m_pRightHandWeapon->GetWeaponDamage();
                hitStruct.Hitpoint = hit.pt;
                hitStruct.HitDirection = pHitedEntity->GetWorldPos() - m_pRightHandWeapon->GetEntity()->GetWorldPos();
                hitStruct.PartId = hit.partid;

                pHitDamageComponent->OnHit(hitStruct);
                return;
            }
        }

        m_pRightHandWeapon->StopAttack();
        m_pEntity->GetComponent<CPlayerComponent>()->m_stateMachine->SetCurrentState(typeid(PushBackAction));
    });
}
