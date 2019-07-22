#include "StdAfx.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "WeaponSystem.h"
#include "Components/LifeResources/LifeResourceManager.h"
#include "Components/LifeResources/HealthLifeResource.h"
#include "Components/CharacterComponent.h"
#include "Components/HitDamageComponent.h"
#include "Components/PlayerComponent.h"
#include "StateMachine/StateAction/PushBackAction.h"

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
        m_pMeleeWeapon->Attack();
    });


    IEntityLink* pLink = m_pEntity->GetEntityLinks();
    if (pLink)
    {
        IEntity* pEntity = gEnv->pEntitySystem->GetEntity(pLink->entityId);
        if (pEntity)
        {
            m_pMeleeWeapon = pEntity->GetComponent<CMeleeWeaponComponent>();

            CRY_ASSERT(m_pMeleeWeapon != nullptr);

            AttachToHand();
            HitDetection();
        }
        else
        {
            CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "There is no weapon linked to character");
        }
    }
}

void CWeaponSystemComponent::AttachToHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pMeleeWeapon->GetEntityId());

    auto *attachment = m_pAttachmentManager->GetInterfaceByName(m_weaponSlotName.c_str());
   
    attachment->AddBinding(attachmentItem);

    m_pMeleeWeapon->Init(this);
}

void CWeaponSystemComponent::HitDetection()
{
    m_pMeleeWeapon->RayHitSubject.get_observable().subscribe([this](ray_hit hit)
    {
        IPhysicalEntity* pHitEntity = hit.pCollider;
        IEntity* pHitedEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pHitEntity);
       
        if (pHitedEntity!=nullptr)
        {
            CHitDamageComponent* pHitDamageComponent = pHitedEntity->GetComponent<CHitDamageComponent>();

            if (pHitedEntity->GetGuid() != m_pEntity->GetGuid() && pHitDamageComponent != nullptr)
            {
                SWeaponHitStruct hitStruct;
                hitStruct.Damage = m_pMeleeWeapon->GetWeaponDamage();
                hitStruct.Hitpoint = hit.pt;
                hitStruct.HitDirection = pHitedEntity->GetWorldPos() - m_pMeleeWeapon->GetEntity()->GetWorldPos();
                hitStruct.PartId = hit.partid;

                pHitDamageComponent->OnHit(hitStruct);
            }
        }            
        else
        {
            m_pMeleeWeapon->StopAttack();
            m_pEntity->GetComponent<CPlayerComponent>()->m_stateMachine->SetCurrentState(typeid(PushBackAction));
        }
    });
}
