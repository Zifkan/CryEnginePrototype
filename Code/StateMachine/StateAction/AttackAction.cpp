#include "StdAfx.h"
#include "AttackAction.h"

AttackAction::AttackAction(WeaponSystemComponent* weaponSystem,IBaseLifeResource* lifeResource, IEntity* characterEntity,
    Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
    Cry::DefaultComponents::CCharacterControllerComponent* characterController,
    ICharacterActions* characterAction,
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
    ActionScopes scopeMask, uint32 userToken) : BaseAction(characterAction, priority, fragmentID, fragTags, flags, scopeMask, userToken)
    , m_pLifeResource(lifeResource)
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

        if(type==FORCE_ATTACK)
            m_pAnimationComponent->SetTag("Force", true);      
    });
}


void AttackAction::Enter()
{
    if (m_attackId >=4)
        m_attackId = 0;

   
    m_pAnimationComponent->SetAnimationDrivenMotion(false);
    m_pLifeResource->ChangeValue(0);
    SetOptionIdx(m_attackId);
    m_pCharacterController->SetVelocity(ZERO);
    BaseAction::Enter();
}

void AttackAction::Exit()
{
    m_attackId++;
    m_pWeaponSystem->StopAttack();
    BaseAction::Exit();
}



IAction::EStatus AttackAction::Update(float timePassed)
{   
    m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, 1);

    auto timeLeft = GetNormalizedTime(0);

    if (timeLeft <= 0.75f)
    {
        m_pCharacterController->AddVelocity(5 * timePassed * m_pCharacterEntity->GetForwardDir());
        m_pWeaponSystem->StopAttack();
    }

    return IAction::Update(timePassed);
}
