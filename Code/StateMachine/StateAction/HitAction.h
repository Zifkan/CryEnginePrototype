#pragma once
#include "BaseAction.h"
#include "Components/Damage/HitDamageComponent.h"

class HitAction : public BaseAction
{
public:
    HitAction(IEntity* pCharacterEntity, CHitDamageComponent*  hitDamageComponent, ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID,
              const TagState& fragTags = TAG_STATE_FULL, uint32 flags = 0, ActionScopes scopeMask = 0,
              uint32 userToken = 0);


    EStatus Update(float timePassed) override;

private:
    CHitDamageComponent*  m_pHitDamageComponent = nullptr;
    TagID m_leftSideTagId;
    TagID m_rightSideTagId;
};
