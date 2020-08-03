#include "StdAfx.h"
#include "AttackAction.h"
#include "StateMachine/StateMachine.h"


AttackAction::AttackAction(IEntity* pCharacterEntity,
    WeaponSystemComponent* weaponSystem,
    IBaseLifeResource* lifeResource,
    
    int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
    ActionScopes scopeMask, uint32 userToken) : BaseAction(pCharacterEntity,priority, fragmentID, fragTags, flags, scopeMask, userToken)
    , m_pLifeResource(lifeResource)
    , m_attackId(0)
    , m_pWeaponSystem(weaponSystem)
{
    m_simpleTagId = m_pAnimationComponent->GetTagId("Simple");
    m_forceTagId = m_pAnimationComponent->GetTagId("Force");


    timeLeft = 0.0f;
   
}


void AttackAction::Enter()
{
    CryLog("Id: %i", GetOptionIdx());  


    if (m_attackId >=4)
        m_attackId = 0;  

   
  //  m_pAnimationComponent->SetAnimationDrivenMotion(false);
  
    m_pCharacterController->SetVelocity(ZERO);    

    m_attackId++;

    BaseAction::Enter();
}

void AttackAction::Exit()
{
  
    m_pWeaponSystem->StopAttack();

    m_pAnimationComponent->SetTagWithId(m_simpleTagId, false);
    m_pAnimationComponent->SetTagWithId(m_forceTagId, false);
    timeLeft = 0.0f;

   // m_pAnimationComponent->GetCharacter()->GetISkeletonAnim()->SetAnimationDrivenMotion(false);
    BaseAction::Exit();
}



IAction::EStatus AttackAction::Update(float timePassed)
{
   /* if (m_attackType == ATTACK)
    {
        m_pAnimationComponent->SetTagWithId(m_simpleTagId, true);
    }


    if (m_attackType == FORCE_ATTACK)
    {       
        m_pAnimationComponent->SetTagWithId(m_forceTagId, true);
    }
       
    timeLeft = GetNormalizedTime(0);

   
    if (timeLeft >= 0.75f)
        m_pWeaponSystem->StopAttack();
   
    if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
        SetFragment(m_fragmentID, m_fragTags);



    SetAnimationControlMovement();*/

    return m_eStatus;
}
