#include "StdAfx.h"
#include "MovementState.h"


void CMovementState::OnEnterState()
{
    m_pAnimationComponent->QueueFragmentWithId(m_fragmentId);
}

void CMovementState::OnExitState()
{
}

void CMovementState::Update(float frameTime)
{/*

    if (m_fragmentId != desiredFragmentId)
    {
        desiredFragmentId = m_fragmentId;
        m_pAnimationComponent->QueueFragmentWithId(m_fragmentId);
    }
   */

    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, *m_movementType != WALK ? m_sprintAnimRatio : 1);

    if (!m_pCharacterController->IsOnGround() || !m_pMainCamera) return;

    const float moveSpeed = 20.5f;
    Vec3 camForward = m_pMainCamera->GetForwardDir();
    camForward.z = 0;

    Vec3 m_Move = m_moveDirection->y * camForward.normalized() + m_moveDirection->x * m_pMainCamera->GetRightDir();

    if (m_moveDirection->GetLength2() > 0)
    {
        m_pEntity->SetRotation(quaternion::CreateRotationVDir(m_Move));
        m_pCharacterController->AddVelocity((*m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * frameTime * m_pEntity->GetForwardDir()));
    }
}
