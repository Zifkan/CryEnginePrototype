#pragma once
#include "BaseAction.h"

class EnemyAttackAction : public BaseAction
{
public:
    EnemyAttackAction(WeaponSystemComponent* weaponSystem,
        IEntity* characterEntity,
        ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID,
        const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0,
        uint32 userToken = 0);


    EStatus Update(float timePassed) override;

    void Enter() override;
    void Exit() override;

private:

    float m_lastCallTime;

    uint8 m_attackId;
    IEntity* pPlayerEntity;
    WeaponSystemComponent* m_pWeaponSystem;
    TagID m_simpleTagId;
    TagID m_forceTagId;
};
