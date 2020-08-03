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

void WeaponSystemComponent::Init(IAttachmentManager* attachmentManager)
{
   

    if (m_pAttachmentManager == nullptr)
        m_pAttachmentManager = attachmentManager;

  

  
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
    
/*

    m_pRightHandWeapon->RayHitSubject.get_observable().subscribe([this](std::array<ray_hit, RAY_HIT_COUNT> hits)
    {        
        SWeaponHitStruct hitStruct;
        CHitDamageComponent* pHitDamageComponent = nullptr;
        CCharacterComponent* mEnemyCharacter = nullptr;
        CShieldWeaponComponent* pShieldComponent = nullptr;

        for (auto& i : hits)
        {
            ray_hit* hit = &i;
            IPhysicalEntity* pHitEntity = hit->pCollider;

            IEntity* pHitedEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pHitEntity);

            if (pHitedEntity != nullptr && pHitedEntity->GetId() != m_pEntity->GetId())
            {
                if (mEnemyCharacter == nullptr)
                {
                    mEnemyCharacter = pHitedEntity->GetComponent<CCharacterComponent>();
                    pHitDamageComponent = pHitedEntity->GetComponent<CHitDamageComponent>();
                    hitStruct.Damage = m_pRightHandWeapon->GetWeaponDamage();
                    hitStruct.Hitpoint = hit->pt;
                    hitStruct.HitDirection = pHitedEntity->GetWorldPos() - m_pRightHandWeapon->GetEntity()->GetWorldPos();
                    hitStruct.PartId = hit->partid;
                }

                if (pShieldComponent == nullptr)
                {
                    pShieldComponent = pHitedEntity->GetComponent<CShieldWeaponComponent>();
                }
            }
        }

        if (mEnemyCharacter != nullptr)
        {
            if (!(mEnemyCharacter->GetStatus() & (uint32)CharecterStatusFlag::Hit))
            {
                mEnemyCharacter->SetStatus(CharecterStatusFlag::Hit);
                if (!(mEnemyCharacter->GetStatus() & (uint32)CharecterStatusFlag::Dodge))
                {
                    pHitDamageComponent->OnHit(hitStruct);
                    return;
                }

                if (pShieldComponent != nullptr)
                {
                    m_pRightHandWeapon->StopAttack();
                    m_pEntity->GetComponent<CCharacterComponent>()->m_stateMachine->SetCurrentState(typeid(PushBackAction));
                }
            }
        }
        else
        {
            if (hits.size() > 0)
            {
                m_pRightHandWeapon->StopAttack();
                m_pEntity->GetComponent<CCharacterComponent>()->m_stateMachine->SetCurrentState(typeid(PushBackAction));
            }
        } 
    });*/
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
