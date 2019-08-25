#include "HitAction.h"

HitAction::HitAction(IEntity* pCharacterEntity,CHitDamageComponent*  hitDamageComponent, ICharacterActions* characterAction, int priority, FragmentID fragmentID, const TagState& fragTags,
                     uint32 flags, ActionScopes scopeMask, uint32 userToken): BaseAction(pCharacterEntity,
    characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
,m_pHitDamageComponent(hitDamageComponent)
{
    m_leftSideTagId = m_pAnimationComponent->GetTagId("LeftSide");
    m_rightSideTagId = m_pAnimationComponent->GetTagId("RightSide");

    hitDamageComponent->HitSubject.get_observable().subscribe([this](SideHitEnum sideHit)
    {
        switch (sideHit)
        {
        case LeftSide:
        {
            m_pAnimationComponent->SetTagWithId(m_leftSideTagId, true);
        }
        break;
        case RightSide:
        {
            m_pAnimationComponent->SetTagWithId(m_rightSideTagId, true);
        }
        break;
        }
    });
}

IAction::EStatus HitAction::Update(float timePassed)
{
    if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
        SetFragment(m_fragmentID, m_fragTags);

    SetAnimationControlMovement();

    return m_eStatus;;
}
