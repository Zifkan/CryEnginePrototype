#include "HitAction.h"

HitAction::HitAction(CHitDamageComponent*  hitDamageComponent, ICharacterActions* characterAction, int priority, FragmentID fragmentID, const TagState& fragTags,
                     uint32 flags, ActionScopes scopeMask, uint32 userToken): BaseAction(
    characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
,m_pHitDamageComponent(hitDamageComponent)
{
    hitDamageComponent->HitSubject.get_observable().subscribe([this](SideHitEnum sideHit)
    {
        switch (sideHit)
        {
        case LeftSide:
        {
            m_pStateMachine->GetAnimationComponent()->SetTagWithId(m_leftSideTagId, true);
        }
        break;
        case RightSide:
        {
            m_pStateMachine->GetAnimationComponent()->SetTagWithId(m_rightSideTagId, true);
        }
        break;
        }
    });
}

IAction::EStatus HitAction::Update(float timePassed)
{
    if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
        SetFragment(m_fragmentID, m_fragTags);

    return m_eStatus;;
}

void HitAction::Enter()
{
    if (m_leftSideTagId == TAG_ID_INVALID)
        m_leftSideTagId = m_pStateMachine->GetAnimationComponent()->GetTagId("LeftSide");

    if (m_rightSideTagId == TAG_ID_INVALID)
        m_rightSideTagId = m_pStateMachine->GetAnimationComponent()->GetTagId("RightSide");
}
