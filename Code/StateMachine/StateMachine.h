#pragma once
#include <CryEntitySystem/IEntity.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <typeindex>
#include "StateAction/BaseAction.h"


class BaseAction;

class CStateMachine
{
public:
    
    CStateMachine(Cry::DefaultComponents::CAdvancedAnimationComponent* advanced_animation_component)
        : m_pAnimationComponent(advanced_animation_component)
    {       
    }

    void RegisterState(const type_info& stateId, BaseAction* action)
    {
        m_actionsMap.insert(std::pair<std::type_index, BaseAction*>(stateId, action));
        action->RegisterStateMachine(this);
    }

    void SetCurrentState(const type_info& stateId)
    {       
        BaseAction* newState = m_actionsMap[stateId];
        const auto newActionPriority = newState->GetPriority();
        const uint8 scopeLayer = newState->ScopeLayer;
        const auto it = m_pCurrentActionMap.find(scopeLayer);
        if (it != m_pCurrentActionMap.end())
        {
            if(newActionPriority > m_pCurrentActionMap[scopeLayer]->GetPriority())
            {
                m_pCurrentActionMap[scopeLayer] = newState;
                m_pAnimationComponent->QueueCustomFragment(*newState);
            }
        }
        else
        {
            m_pCurrentActionMap.insert(std::pair<uint8, BaseAction*>(scopeLayer, newState));
            m_pAnimationComponent->QueueCustomFragment(*newState);
        }
    }

    void SetActionFinish(BaseAction* action)
    {
        m_pCurrentActionMap.erase(action->ScopeLayer);
    } 


private:
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

    std::map<uint8, _smart_ptr<BaseAction>> m_pCurrentActionMap;

    std::map<std::type_index, _smart_ptr<BaseAction>> m_actionsMap;

   
};



