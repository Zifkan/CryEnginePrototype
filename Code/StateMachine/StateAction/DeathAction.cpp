#include "StdAfx.h"
#include "DeathAction.h"

DeathAction::DeathAction(Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
                         ICharacterActions* characterAction,
                         int priority, FragmentID fragmentID, const TagState& fragTags, uint32 flags,
                         ActionScopes scopeMask,
                         uint32 userToken): BaseAction(characterAction, priority, fragmentID, fragTags, flags,scopeMask, userToken)
                         , m_pAnimationComponent(animationComponent)
{
    IsDying = true;


}

void DeathAction::Enter()
{  
 
   /* if (IsDying)
    {
        m_pAnimationComponent->SetTag("Dying", true);
    }*/
    BaseAction::Enter();
}

void DeathAction::Exit()
{   
    if (IsDying)
    {      
        IsDying = false;
        m_pAnimationComponent->SetTag("Dead", true);
    }
   
    m_pStateMachine->SetCurrentState(typeid(DeathAction)); 
    BaseAction::Exit();
}

IAction::EStatus DeathAction::Update(float timePassed)
{
   

    //// Update the fragments and tags if they are different.
    //if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
    //    SetFragment(m_fragmentID, m_fragTags);

    return  BaseAction::Update(timePassed);
}