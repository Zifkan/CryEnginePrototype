#include "StdAfx.h"
#include "EnemyAttackAction.h"

EnemyAttackAction::EnemyAttackAction(WeaponSystemComponent* weaponSystem, 
    IEntity* characterEntity,    
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
    ActionScopes scopeMask, uint32 userToken) : BaseAction(characterEntity, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    , m_lastCallTime(0.0f)
    , m_attackId(0)
    , m_pWeaponSystem(weaponSystem)
{
    m_simpleTagId = m_pAnimationComponent->GetTagId("Simple");
    m_forceTagId = m_pAnimationComponent->GetTagId("Force");


  
}


void EnemyAttackAction::Enter()
{
    pPlayerEntity = gEnv->pEntitySystem->FindEntityByName("Player");

    if (m_attackId >= 4 || gEnv->pTimer->GetCurrTime() - m_lastCallTime >= 1)
        m_attackId = 0;

    
    m_pAnimationComponent->SetAnimationDrivenMotion(false);
   
    m_pCharacterController->SetVelocity(ZERO);
    m_attackId++;
    BaseAction::Enter();
}

void EnemyAttackAction::Exit()
{
   
    m_pWeaponSystem->StopAttack();
    m_lastCallTime = gEnv->pTimer->GetCurrTime();
    BaseAction::Exit();
}



IAction::EStatus EnemyAttackAction::Update(float timePassed)
{
    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, 1);

    
    if (GetNormalizedTime(0) > 0.75f)
        m_pWeaponSystem->StopAttack();

    Quat newRotation = Quat::CreateRotationVDir(pPlayerEntity->GetWorldPos()- m_pCharacterController->GetEntity()->GetWorldPos());

    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(newRotation));
    ypr.y = 0;
    ypr.z = 0;
    newRotation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_pCharacterEntity->SetRotation(newRotation);
    SetAnimationControlMovement();

    return IAction::Update(timePassed);
}