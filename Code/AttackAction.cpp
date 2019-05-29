#include "AttackAction.h"

void СAttackAction::OnActionFinished()
{
    m_pState->FinishAction();
}
