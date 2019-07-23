#include "StdAfx.h"
#include "WeaponSystem.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "StateMachine/StateAction/PushBackAction.h"
#include "Components/Characters/PlayerComponent.h"

static void RegisterWeaponSystem(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponSystemComponent));
        // Functions
        {
        }
    }
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeaponSystem)

void CWeaponSystemComponent::Init(ICharacterActions* characterActions, IAttachmentManager* attachmentManager)
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
    if (pLink)
    {
        IEntity* pEntity = gEnv->pEntitySystem->GetEntity(pLink->entityId);
        if (pEntity)
        {
            m_pRightHandWeapon = pEntity->GetComponent<CMeleeWeaponComponent>();

            CRY_ASSERT(m_pRightHandWeapon != nullptr);

            AttachToRightHand();
            HitDetection();
        }
        else
        {
            CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "There is no weapon linked to character");
        }
    }
}

void CWeaponSystemComponent::AttachToRightHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pRightHandWeapon->GetEntityId());

    auto *attachment = m_pAttachmentManager->GetInterfaceByName(m_rightWeaponSlotName.c_str());
   
    attachment->AddBinding(attachmentItem);

  // m_pRightHandWeapon->Init(this);
}

void CWeaponSystemComponent::HitDetection()
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
