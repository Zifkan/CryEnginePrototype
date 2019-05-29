#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "StdAfx.h"

class IBaseState
{
public:       
    virtual ~IBaseState(){}

    virtual void OnEnterState() = 0;
    virtual void OnExitState() = 0;
    virtual void Update(float frameTime) = 0;

    virtual void FinishAction();

protected:
    FragmentID m_fragmentId;
};
