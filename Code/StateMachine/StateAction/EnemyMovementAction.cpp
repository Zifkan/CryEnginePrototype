#include "StdAfx.h"
#include "EnemyMovementAction.h"
#include "StateMachine/StateMachine.h"

EnemyMovementAction::EnemyMovementAction(IEntity* characterEntity,    
    ICharacterActions* characterAction,
    int priority,
    FragmentID fragmentID,
    const TagState& fragTags,
    uint32 flags,
    ActionScopes scopeMask,
    uint32 userToken) :
    BaseAction(characterEntity,characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    , m_movePoint(0.0)
    , m_sprintRatio(1.5f)
    , m_sprintAnimRatio(3.5)
{
    m_movementCompositeSubscribtion = m_pCharacterAction->MovementSubject.get_observable().start_with(Vec2(0.1f, 0.0f)).subscribe([this](Vec2 vec2)
    {
        if (vec2.GetLength2() == 0)
        {
            m_pStateMachine->SetActionFinish(this);
            ForceFinish();
        }
        m_movePoint = vec2;
    });


    m_movementTypeSubscribtion = m_pCharacterAction->MovementTypeSubject.get_observable().subscribe([this](MovementType type)
    {
        m_movementType = type;
    });

    m_movementType = WALK;
}

EnemyMovementAction::~EnemyMovementAction()
{
    m_movementCompositeSubscribtion.unsubscribe();
    m_movementTypeSubscribtion.unsubscribe();
}


IAction::EStatus EnemyMovementAction::Update(float timePassed)
{

    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, m_movementType != WALK ? m_sprintAnimRatio : 1);

    if (!m_pCharacterController->IsOnGround()) return IAction::Update(timePassed);

 

    const float moveSpeed = 20.5f;


    if (m_movePoint.GetLength2() > 0)
    {
        //   m_pCharacterController->AddVelocity((m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * timePassed * m_movePoint));

        m_pCharacterController->ChangeVelocity((m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) *m_movePoint, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);

        Quat newRotation = Quat::CreateRotationVDir(m_pCharacterController->GetMoveDirection());

        Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(newRotation));
        ypr.y = 0;
        ypr.z = 0;
        newRotation = Quat(CCamera::CreateOrientationYPR(ypr));
        m_pCharacterEntity->SetRotation(newRotation);
    }

    return IAction::Update(timePassed);
}
