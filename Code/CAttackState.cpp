#include "CAttackState.h"


CAttackState::CAttackState(IEntity* entity)
: m_pEntity(entity)
, m_lastActiontime(0)
{
    m_pCharacterController = entity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pAnimationComponent = entity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

    m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    m_forceAttackTagId = m_pAnimationComponent->GetTagId("ForceAttack");
}

void CAttackState::OnEnterState()
{
    if (gEnv->pTimer->GetCurrTime() - m_lastActiontime<1.0f || m_attackId>5)
    {
        m_attackId = 0;
    }

   /* m_attackId = m_attackId > 5 || attackData.TypeValue ? 0 : m_attackId;
    pAttackAction = new СAttackAction(this,1, m_attackFragmentId);
    pAttackAction->SetOptionIdx(m_attackId);
    m_pAnimationComponent->SetTagWithId(m_forceAttackTagId, attackData.TypeValue);
    m_pAnimationComponent->QueueCustomFragment(*pAttackAction);*/
   

}

void CAttackState::Update(float frameTime)
{

}

void CAttackState::OnExitState()
{
    m_attackId++;
    m_lastActiontime = gEnv->pTimer->GetCurrTime();
}
