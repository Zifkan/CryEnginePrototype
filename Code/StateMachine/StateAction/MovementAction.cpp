#include "StdAfx.h"
#include "MovementAction.h"
#include "StateMachine/StateMachine.h"


MovementAction::MovementAction(IBaseLifeResource* lifeResource,
    IEntity* characterEntity,
    IEntity* mainCamera,
    ICharacterActions* characterAction, 
    int priority, 
    FragmentID fragmentID,
    const TagState& fragTags, 
    uint32 flags,
    ActionScopes scopeMask,
    uint32 userToken):
    BaseAction(characterEntity,characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    ,m_pLifeResource(lifeResource)
    ,m_pMainCameraEntity(mainCamera)
    ,m_pCharacterEntity(characterEntity)   
    ,m_moveDirection(0.0)
    ,m_sprintRatio(1.5f)
    ,m_sprintAnimRatio(3.5)
{
    m_movementCompositeSubscribtion = m_pCharacterAction->MovementSubject.get_observable().start_with(Vec2(0.1f,0.0f)).subscribe([this](Vec2 vec2)
    {       
        if (vec2.GetLength2() == 0)
        {
            m_pStateMachine->SetActionFinish(this);
            ForceFinish();
        }
        m_moveDirection = vec2;

        m_pMainCameraComponent = m_pMainCameraEntity->GetComponent<CCameraController>();
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

    if (!m_pCharacterController->IsOnGround() || !m_pMainCameraEntity) return IAction::Update(timePassed);

    const float moveSpeed = 20.5f;
    Vec3 camForward = m_pMainCameraEntity->GetForwardDir();
    camForward.z = 0;
          
    const Vec3 moveDirection = m_moveDirection.y * camForward.normalized() + m_moveDirection.x * m_pMainCameraEntity->GetRightDir();

    SetRotation(moveDirection);

    m_pCharacterController->AddVelocity((m_movementType == (SPRINT | DODGE) ? m_sprintRatio : 1.0f) * (moveSpeed * timePassed * moveDirection));

    return IAction::Update(timePassed);
}

void MovementAction::Enter()
{
}

void MovementAction::Exit()
{
}

Vec3 MovementAction::GetLookDirNormalized(Vec3 target, Vec3 location)
{   
    auto x = target.x - location.x;
    auto y = target.y - location.y;
    auto z = target.z - location.z;

    return Vec3(x, y, z).normalize();
}

void MovementAction::SetRotation(Vec3 moveDirection)
{
    if (m_pMainCameraComponent->IsCameraFocus())
    {
        const auto lookDir = GetLookDirNormalized(m_pCharacterEntity->GetPos(), m_pMainCameraEntity->GetPos());
        m_pCharacterEntity->SetRotation(Quat::CreateRotationVDir(Vec3(lookDir.x, lookDir.y, 0)));
    }

    if (m_moveDirection.GetLength2() > 0 && !m_pMainCameraComponent->IsCameraFocus())
    {
        m_pCharacterEntity->SetRotation(quaternion::CreateRotationVDir(moveDirection));
    }
}
