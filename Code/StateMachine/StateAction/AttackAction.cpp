#include "StdAfx.h"
#include "AttackAction.h"
#include "StateMachine/StateMachine.h"


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

    timeLeft = 0.0f;
    characterAction->AttackSubject.get_observable().subscribe([this](AttackType type)
    {
        if(timeLeft==0.0f)
        {
            m_attackType = type;
           // CryLog("m_attackType %i", m_attackType);
        }
      
    });
}


void AttackAction::Enter()
{
    CryLog("Id: %i", GetOptionIdx());

    if (m_simpleTagId == TAG_ID_INVALID)
        m_simpleTagId = m_pStateMachine->GetAnimationComponent()->GetTagId("Simple");

    if (m_forceTagId == TAG_ID_INVALID)
        m_forceTagId = m_pStateMachine->GetAnimationComponent()->GetTagId("Force");



    if (m_attackId >=4)
        m_attackId = 0;  

   
    m_pAnimationComponent->SetAnimationDrivenMotion(false);
    m_pLifeResource->ChangeValue(0);
    m_pCharacterController->SetVelocity(ZERO);    

    m_attackId++;
    BaseAction::Enter();
}

void AttackAction::Exit()
{
  
    m_pWeaponSystem->StopAttack();

    auto animComponent = m_pStateMachine->GetAnimationComponent();
    animComponent->SetTagWithId(m_simpleTagId, false);
    animComponent->SetTagWithId(m_forceTagId, false);
    timeLeft = 0.0f;
    BaseAction::Exit();
}



IAction::EStatus AttackAction::Update(float timePassed)
{   
 
    auto animComponent = m_pStateMachine->GetAnimationComponent();

    if (m_attackType == ATTACK)
    {
        animComponent->SetTagWithId(m_simpleTagId, true);
    }


    if (m_attackType == FORCE_ATTACK)
    {       
        animComponent->SetTagWithId(m_forceTagId, true);
    }
       
    timeLeft = GetNormalizedTime(0);

    if (timeLeft <= 0.75f)
    {
        m_pCharacterController->AddVelocity(5 * timePassed * m_pCharacterEntity->GetForwardDir());
    }

    if (timeLeft >= 0.75f)
        m_pWeaponSystem->StopAttack();


   
    if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
        SetFragment(m_fragmentID, m_fragTags);

  
   


    return m_eStatus;
}
