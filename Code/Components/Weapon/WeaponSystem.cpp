#include "StdAfx.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "WeaponSystem.h"
#include "Components/Player.h"

static void RegisterWeaponSystem(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponSystem));
        // Functions
        {
        }
    }
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeaponSystem)

void CWeaponSystem::Init(ICharacterActions* characterActions, IAttachmentManager* attachmentManager)
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
        }
        else
        {
            CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "There is no weapon linked to character");
        }
    }
}

void CWeaponSystem::AttachToHand()
{
    auto *attachmentItem = new CEntityAttachment();
    attachmentItem->SetEntityId(m_pMeleeWeapon->GetEntityId());

    auto *attachment = m_pAttachmentManager->GetInterfaceByName(m_weaponSlotName.c_str());
   
    attachment->AddBinding(attachmentItem);   
}
