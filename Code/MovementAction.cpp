#include "MovementAction.h"

void CMovementAction::OnActionFinished()
{
    m_pState->FinishAction();
}
