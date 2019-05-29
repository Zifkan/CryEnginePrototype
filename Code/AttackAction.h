#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "IBaseState.h"
#include "CAttackState.h"

class СAttackAction : public IAction
{
public:
    СAttackAction(IBaseState* state, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
        : IAction(priority, fragmentID, fragTags, flags, scopeMask, userToken)
        , m_pState(state)
    {
    }

    void OnActionFinished() override;

    virtual const char* GetName() const;
    virtual void DoDelete();
    virtual IAction* CreateSlaveAction(FragmentID slaveFragID, const TagState& fragTags, SAnimationContext& context);
private:
    IBaseState* m_pState;
};
