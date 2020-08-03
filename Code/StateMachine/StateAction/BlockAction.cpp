#include "StdAfx.h"
#include "BlockAction.h"
#include "StateMachine/StateMachine.h"


BlockAction::BlockAction(IEntity* pCharacterEntity, 
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags, ActionScopes scopeMask,
    uint32 userToken) : BaseAction(pCharacterEntity,priority, fragmentID, fragTags, flags, scopeMask, userToken)
{
    ScopeLayer = 1;

  
}

BlockAction::~BlockAction()
{
   
}

