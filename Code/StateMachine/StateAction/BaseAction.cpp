#include "StdAfx.h"
#include "BaseAction.h"
#include "StateMachine/StateMachine.h"

IAction* BaseAction::CreateSlaveAction(FragmentID slaveFragID, const TagState& fragTags, SAnimationContext& context)
{
    ActionScopes forceScopeMask = (slaveFragID == FRAGMENT_ID_INVALID) ? ACTION_SCOPES_ALL : ACTION_SCOPES_NONE;
    return new TAction<CONTEXT>(GetPriority(), slaveFragID, fragTags, 0, forceScopeMask);
}

void BaseAction::RegisterStateMachine(CStateMachine* stateMachine)
{
    m_pStateMachine = stateMachine;
    m_pISkeletonAnim = m_pStateMachine->GetAnimationComponent()->GetCharacter()->GetISkeletonAnim();
}

void BaseAction::Enter()
{
    IAction::Enter();
}

void BaseAction::Exit()
{
    OnActionFinished();
    IAction::Exit();
}

void BaseAction::OnActionFinished()
{
    m_pStateMachine->SetActionFinish(this);
}

float BaseAction::GetNormalizedTime(uint32 layer)
{
    return m_pISkeletonAnim->GetLayerNormalizedTime(layer);
}
