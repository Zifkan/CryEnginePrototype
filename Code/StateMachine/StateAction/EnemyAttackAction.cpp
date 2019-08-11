#include "StdAfx.h"
#include "EnemyAttackAction.h"

EnemyAttackAction::EnemyAttackAction(WeaponSystemComponent* weaponSystem, IEntity* characterEntity,
    Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
    Cry::DefaultComponents::CCharacterControllerComponent* characterController,
    ICharacterActions* characterAction,
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
    ActionScopes scopeMask, uint32 userToken) : BaseAction(characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    , m_lastCallTime(0.0f)
    , m_attackId(0)
    , m_pAnimationComponent(animationComponent)
    , m_pCharacterController(characterController)
    , m_pCharacterEntity(characterEntity)
    , m_pWeaponSystem(weaponSystem)
{
    characterAction->AttackSubject.get_observable().distinct_until_changed().subscribe([this](AttackType type)
    {
        if (type == ATTACK)
            m_pAnimationComponent->SetTag("Simple", true);

        if (type == FORCE_ATTACK)
            m_pAnimationComponent->SetTag("Force", true);
    });
}


void EnemyAttackAction::Enter()
{
    pPlayerEntity = gEnv->pEntitySystem->FindEntityByName("Player");

    if (m_attackId >= 4 || gEnv->pTimer->GetCurrTime() - m_lastCallTime >= 1)
        m_attackId = 0;

    
    m_pAnimationComponent->SetAnimationDrivenMotion(false);
    SetOptionIdx(m_attackId);
    m_pCharacterController->SetVelocity(ZERO);
   
    BaseAction::Enter();
}

void EnemyAttackAction::Exit()
{
    m_attackId++;
    m_pWeaponSystem->StopAttack();
    m_lastCallTime = gEnv->pTimer->GetCurrTime();
    BaseAction::Exit();
}



IAction::EStatus EnemyAttackAction::Update(float timePassed)
{
    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, 1);

    if (GetNormalizedTime(0)<=0.75f)
        m_pCharacterController->AddVelocity(5 * timePassed * m_pCharacterEntity->GetForwardDir());

    if (GetNormalizedTime(0) > 0.75f)
        m_pWeaponSystem->StopAttack();

    Quat newRotation = Quat::CreateRotationVDir(pPlayerEntity->GetWorldPos()- m_pCharacterController->GetEntity()->GetWorldPos());

    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(newRotation));
    ypr.y = 0;
    ypr.z = 0;
    newRotation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_pCharacterEntity->SetRotation(newRotation);

    return IAction::Update(timePassed);
}