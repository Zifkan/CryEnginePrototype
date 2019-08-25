#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "BaseAction.h"
#include "Components/LifeResources/BaseLifeResource.h"
#include "Components/Weapon/WeaponSystem.h"

class AttackAction : public BaseAction
{
public:
    AttackAction(IEntity* pCharacterEntity, WeaponSystemComponent* weaponSystem,IBaseLifeResource* lifeResource,
                 ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID,
                 const TagState& fragTags = TAG_STATE_FULL, uint32 flags = 0, ActionScopes scopeMask = 0,
                 uint32 userToken = 0);


    EStatus Update(float timePassed) override;

    void Enter() override;
    void Exit() override;

private:

    IBaseLifeResource* m_pLifeResource;
    WeaponSystemComponent* m_pWeaponSystem;
    TagID m_simpleTagId = TAG_ID_INVALID;
    TagID m_forceTagId = TAG_ID_INVALID;
    AttackType m_attackType;
    float timeLeft;
    int m_attackId;
};
