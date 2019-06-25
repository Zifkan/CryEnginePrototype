#include "StdAfx.h"
#include "AttackAction.h"

AttackAction::AttackAction(IBaseLifeResource* lifeResource, IEntity* characterEntity,
    Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
    Cry::DefaultComponents::CCharacterControllerComponent* characterController,
    ICharacterActions* characterAction,
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
    ActionScopes scopeMask, uint32 userToken) : BaseAction(characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    , m_pLifeResource(lifeResource)
    , m_lastCallTime(0.0f)
    , m_attackId(0)
    , m_pAnimationComponent(animationComponent)
    , m_pCharacterController(characterController)
    , m_pCharacterEntity(characterEntity)
{
    characterAction->AttackSubject.get_observable().distinct_until_changed().subscribe([this](AttackType type)
    {
        if (type == ATTACK)
            m_pAnimationComponent->SetTag("Simple", true);

        if(type==FORCE_ATTACK)
            m_pAnimationComponent->SetTag("Force", true);
        
        CryLog("Attack Type: %i",type );
    });
}


void AttackAction::Enter()
{
    if (m_attackId >= 5 || gEnv->pTimer->GetCurrTime() - m_lastCallTime >= 1)
        m_attackId = 0;

   
    m_pAnimationComponent->SetAnimationDrivenMotion(false);
    m_pLifeResource->ChangeValue(-15);
    SetOptionIdx(m_attackId);
    BaseAction::Enter();
}

void AttackAction::Exit()
{
    m_attackId++;
    m_lastCallTime = gEnv->pTimer->GetCurrTime();
    BaseAction::Exit();
}



IAction::EStatus AttackAction::Update(float timePassed)
{   
    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, 1);
    m_pCharacterController->AddVelocity(10* timePassed * m_pCharacterEntity->GetForwardDir());
    return IAction::Update(timePassed);
}
