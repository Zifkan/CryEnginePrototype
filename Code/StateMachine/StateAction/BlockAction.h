#pragma once
#include "BaseAction.h"

class BlockAction :  public BaseAction
{
public:
    BlockAction(IEntity* pCharacterEntity,
        int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID,
        const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0,
        uint32 userToken = 0);

    ~BlockAction();
};
