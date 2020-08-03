#pragma once
#include "BaseAction.h"
#include "Components/Weapon/MeleeWeapon.h"

class DeathAction : public BaseAction
{
public:
    DeathAction(IEntity* pCharacterEntity, WeaponSystemComponent* weaponSystemComponent,
                int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID,
                const TagState& fragTags = TAG_STATE_FULL, uint32 flags = 0, ActionScopes scopeMask = 0,
                uint32 userToken = 0);

    virtual void Enter();
    virtual void Exit();
    IAction::EStatus Update(float timePassed);
private:
    bool IsDying;
    WeaponSystemComponent* m_pWeaponSystemComponent;
};
