#include "StdAfx.h"
#include "BlockAction.h"
#include "StateMachine/StateMachine.h"


BlockAction::BlockAction(IEntity* pCharacterEntity, ICharacterActions* characterAction,
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags, ActionScopes scopeMask,
    uint32 userToken) : BaseAction(pCharacterEntity,characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
{
    ScopeLayer = 1;

    m_blocActionSubsciption = characterAction->BlockSubject.get_observable().distinct_until_changed().subscribe([this](bool isBlock)
    {
        if (!isBlock)
        {
            m_pStateMachine->SetActionFinish(this);
            ForceFinish();
        }
    });
}

BlockAction::~BlockAction()
{
    m_blocActionSubsciption.unsubscribe();
}

