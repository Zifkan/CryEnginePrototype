#include "StdAfx.h"
#include "WeaponSystem.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "StateMachine/StateAction/PushBackAction.h"
#include "Components/Characters/PlayerComponent.h"
#include "BaseCustomWeapon.h"

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

    m_pCharacterActions->BlockSubject.get_observable().distinct_until_changed().subscribe([this](bool isBlock)
    {
        m_pLeftHandWeapon->SetBlock(isBlock);
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
            BaseCustomWeapon* weaponType = pEntity->GetComponent<BaseCustomWeapon>();

           
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

    m_pRightHandWeapon->Init(this, m_pLeftHandWeapon==nullptr?nullptr: m_pLeftHandWeapon->GetEntity());
}


void WeaponSystemComponent::AttachToRightHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pRightHandWeapon->GetEntityId());

    m_pRightHandAttachment = m_pAttachmentManager->GetInterfaceByName(m_rightWeaponSlotName.c_str());
   
    m_pRightHandAttachment->AddBinding(attachmentItem);
}

void WeaponSystemComponent::AttachToLeftHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pLeftHandWeapon->GetEntityId());

    m_pLeftHandAttachment = m_pAttachmentManager->GetInterfaceByName(m_leftWeaponSlotName.c_str());

    m_pLeftHandAttachment->AddBinding(attachmentItem);
}

void WeaponSystemComponent::HitDetection()
{
    


    m_pRightHandWeapon->RayHitSubject.get_observable().subscribe([this](std::array<ray_hit, RAY_HIT_COUNT> hits)
    {
        bool isHitted = false;    
        bool isBlocked = false;
        SWeaponHitStruct hitStruct;
        CHitDamageComponent* pHitDamageComponent;
        for (auto& i : hits)
        {
            ray_hit* hit = &i;
            IPhysicalEntity* pHitEntity = hit->pCollider;

            IEntity* pHitedEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pHitEntity);
                       
            if (pHitedEntity != nullptr && pHitedEntity->GetId() != m_pEntity->GetId())
            {
                pHitDamageComponent = pHitedEntity->GetComponent<CHitDamageComponent>();
                              
                if (pHitDamageComponent != nullptr)
                {
                    //Check is Character was already hitted by weapon - insta skip 
                    if (pHitDamageComponent->IsHitted())
                    {
                        return;
                    }
                    else
                    {
                        isHitted = true;

                        hitStruct.Damage = m_pRightHandWeapon->GetWeaponDamage();
                        hitStruct.Hitpoint = hit->pt;
                        hitStruct.HitDirection = pHitedEntity->GetWorldPos() - m_pRightHandWeapon->GetEntity()->GetWorldPos();
                        hitStruct.PartId = hit->partid;
                    }                    
                }

                CShieldWeaponComponent* pShieldComponent = pHitedEntity->GetComponent<CShieldWeaponComponent>();

                if (pShieldComponent != nullptr && pShieldComponent->IsInBlock())
                {
                    isBlocked = true;
                }

                
            }
        }

        if (isHitted && !isBlocked)
        {
            pHitDamageComponent->OnHit(hitStruct);
            return;
        }


      //  CryLog("hitted entity: %s", pHitedEntity->GetName());
        m_pRightHandWeapon->StopAttack();
        m_pEntity->GetComponent<CCharacterComponent>()->m_stateMachine->SetCurrentState(typeid(PushBackAction));
    });
}

void WeaponSystemComponent::DetachWeapons() const
{
    m_pRightHandAttachment->ClearBinding();
    m_pLeftHandAttachment->ClearBinding();
}

void WeaponSystemComponent::StopAttack()
{
    m_pRightHandWeapon->StopAttack();
}
