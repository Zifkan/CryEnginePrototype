#pragma once
#include "BaseAction.h"

class IdleAction :public BaseAction
{
public:
    IdleAction(ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
        : BaseAction(characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    {
    }


    void Enter();
    void Exit();
};
