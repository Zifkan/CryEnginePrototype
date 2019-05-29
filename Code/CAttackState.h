#pragma once
#include  "StdAfx.h"
#include "IBaseState.h"
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include "AttackAction.h"

class CAttackState: IBaseState
{
public:
    CAttackState(IEntity* entity);

    void OnEnterState() override;
    void Update(float frameTime) override;
    void OnExitState() override;
private:
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

    IEntity* m_pEntity;
    
    FragmentID m_attackFragmentId;
    TagID m_forceAttackTagId;

    uint8  m_attackId;
    СAttackAction* pAttackAction;

    float m_lastActiontime;
};
