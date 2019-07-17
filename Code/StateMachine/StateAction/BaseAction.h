#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "CharacterActions.h"
#include <DefaultComponents/Physics/CharacterControllerComponent.h>


class CStateMachine;

class BaseAction :public IAction
{
public:
    BaseAction(ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
        : IAction(priority, fragmentID, fragTags, flags, scopeMask, userToken)
        , m_pCharacterAction(characterAction)
    {        
        ScopeLayer = 0;
    }

    DEFINE_ACTION(typeid(this).name())   
    
    IAction* CreateSlaveAction(FragmentID slaveFragID, const TagState& fragTags, SAnimationContext& context) override;

   
    void RegisterStateMachine(CStateMachine* stateMachine);

    virtual void Enter() override;
    virtual void Exit() override;

    virtual void OnActionFinished();

    uint8 ScopeLayer;

protected:
    ICharacterActions* m_pCharacterAction;
    CStateMachine* m_pStateMachine;

    float GetNormalizedtime();
};
