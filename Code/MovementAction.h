#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "IBaseState.h"

class CMovementAction : public IAction
{
public:
    CMovementAction(IBaseState* state,int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags, ActionScopes scopeMask, uint32 userToken)
        : IAction(priority, fragmentID, fragTags, flags, scopeMask, userToken)
        , m_pState(state)
    {
    }

    void OnActionFinished() override;

private:
    IBaseState* m_pState;
};
