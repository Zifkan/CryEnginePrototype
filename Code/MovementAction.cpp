#include "StdAfx.h"
#include "MovementAction.h"
#include "StateMachine.h"


MovementAction::MovementAction(IEntity* characterEntity,Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
    Cry::DefaultComponents::CCharacterControllerComponent* characterController,
    IEntity* mainCamera,
    ICharacterActions* characterAction, 
    int priority, 
    FragmentID fragmentID,
    const TagState& fragTags, 
    uint32 flags,
    ActionScopes scopeMask,
    uint32 userToken):
    BaseAction(characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    ,m_pCharacterController(characterController)
    ,m_pAnimationComponent(animationComponent)
    ,m_pMainCamera(mainCamera)
    ,m_pCharacterEntity(characterEntity)   
    ,m_moveDirection(0.0)
    ,m_sprintRatio(1.5f)
    ,m_sprintAnimRatio(3.5)
{
    m_movementCompositeSubscribtion = m_pCharacterAction->MovementSubject.get_observable().start_with(Vec2(0.1f,0.0f)).subscribe([this](Vec2 vec2)
    {       
        if (vec2.GetLength2() == 0)
        {
            m_pStateMachine->SetActionFinish();
            ForceFinish();
        }
        m_moveDirection = vec2;
    });


    m_movementTypeSubscribtion = m_pCharacterAction->MovementTypeSubject.get_observable().subscribe([this](MovementType type)
    {
        m_movementType = type;
    });

    m_movementType = WALK;
}

MovementAction::~MovementAction()
{
    m_movementCompositeSubscribtion.unsubscribe();
    m_movementTypeSubscribtion.unsubscribe();
}


IAction::EStatus MovementAction::Update(float timePassed)
{

    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, m_movementType != WALK ? m_sprintAnimRatio : 1);

    if (!m_pCharacterController->IsOnGround() || !m_pMainCamera) return IAction::Update(timePassed);

    const float moveSpeed = 20.5f;
    Vec3 camForward = m_pMainCamera->GetForwardDir();
    camForward.z = 0;

    Vec3 m_Move = m_moveDirection.y * camForward.normalized() + m_moveDirection.x * m_pMainCamera->GetRightDir();

    if (m_moveDirection.GetLength2() > 0)
    {
        m_pCharacterEntity->SetRotation(quaternion::CreateRotationVDir(m_Move));
        m_pCharacterController->AddVelocity((m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * timePassed * m_pCharacterEntity->GetForwardDir()));
    }


    return IAction::Update(timePassed);
}

void MovementAction::Enter()
{
}

void MovementAction::Exit()
{
}
