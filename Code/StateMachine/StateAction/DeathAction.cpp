#include "StdAfx.h"
#include "DeathAction.h"
#include "Components/Weapon/WeaponSystem.h"

DeathAction::DeathAction(IEntity* pCharacterEntity, 
                         WeaponSystemComponent* weaponSystemComponent,                         
                         int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
                         ActionScopes scopeMask,
                         uint32 userToken): BaseAction(pCharacterEntity, priority, fragmentID, fragTags, flags,scopeMask, userToken)                        
                         , m_pWeaponSystemComponent(weaponSystemComponent)
{
    IsDying = true;


}

void DeathAction::Enter()
{  
 
    if (IsDying)
    {
        m_pAnimationComponent->SetTag("Dying", true);
    }

    

    m_pWeaponSystemComponent->DetachWeapons();
    BaseAction::Enter();
}

void DeathAction::Exit()
{   
    if (IsDying)
    {      
        IsDying = false;
        m_pAnimationComponent->SetTag("Dying", false);
        m_pAnimationComponent->SetTag("Dead", true);
    }   
  
   // m_pStateMachine->SetCurrentState(typeid(DeathAction)); 
    BaseAction::Exit();
}

IAction::EStatus DeathAction::Update(float timePassed)
{
   

    //// Update the fragments and tags if they are different.
    //if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
    //    SetFragment(m_fragmentID, m_fragTags);

    return  BaseAction::Update(timePassed);
}