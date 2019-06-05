#include "StdAfx.h"
#include "AttackAction.h"

void AttackAction::Enter()
{
    if (m_attackId >= 5 || gEnv->pTimer->GetCurrTime() - m_lastCallTime >= 1)
        m_attackId = 0;

    
    SetOptionIdx(m_attackId);
    BaseAction::Enter();
}

void AttackAction::Exit()
{
    m_attackId++;
    m_lastCallTime = gEnv->pTimer->GetCurrTime();
    BaseAction::Exit();
}

IAction::EStatus AttackAction::Update(float timePassed)
{
   
    return IAction::Update(timePassed);
}
