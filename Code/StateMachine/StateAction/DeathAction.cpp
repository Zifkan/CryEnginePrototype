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
    BaseAction::Enter();
 /*
    if (IsDying)
    {
        m_pAnimationComponent->SetTag("Simple", true);
    }
    else
    {
        m_pAnimationComponent->SetTag("Simple", false);
        m_pAnimationComponent->SetTag("Force", true);

    }  */

}

void DeathAction::Exit()
{
    //BaseAction::Exit();

    //if (IsDying)
    //{
    //  
    //    IsDying = false;
    //}
    //m_pStateMachine->SetCurrentState(typeid(DeathAction));
}

IAction::EStatus DeathAction::Update(float timePassed)
{
  //  m_pAnimationComponent->SetTag("Dying", true);

    return BaseAction::Update(timePassed);
}
