#pragma once
#include <CryEntitySystem/IEntity.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include "IdleAction.h"
#include <typeindex>


class BaseAction;

class CStateMachine
{
public:
    
    CStateMachine(Cry::DefaultComponents::CCharacterControllerComponent* character_controller_component,
        Cry::DefaultComponents::CAdvancedAnimationComponent* advanced_animation_component, 
        IEntity* entity)
        : m_pCharacterController(character_controller_component),
          m_pAnimationComponent(advanced_animation_component),
          m_pEntity(entity),
          m_isLastActionPlaying(false),
          m_lastActionPriority(-1)
    {       
    }

    void RegisterState(const type_info& stateId, BaseAction* action)
    {
        m_actionsMap.insert(std::pair<std::type_index, BaseAction*>(stateId, action));
        action->RegisterStateMachine(this);
    }

    void SetCurrentState(const type_info& stateId/*BaseAction* action*/)
    {       
        BaseAction* newState = m_actionsMap[stateId];
        const auto newActionPriority = newState->GetPriority();
        if (m_lastActionPriority<0 || m_isLastActionPlaying && newActionPriority > m_lastActionPriority || !m_isLastActionPlaying)
        {       
            m_lastActionPriority = newActionPriority;
            m_isLastActionPlaying = true;
            m_pAnimationComponent->QueueCustomFragment(*newState);
        }
    }

    void SetActionFinish()
    {
        m_isLastActionPlaying = false;
    }

    void Update()
    {
       /* if (m_pCurrentState != nullptr)
            CryLog("Is playing: %i", m_pCurrentState->IsPlayingFragment());*/
    }
   


private:
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

    IEntity* m_pEntity;

    std::map<std::type_index, _smart_ptr<BaseAction>> m_actionsMap;
    bool m_isLastActionPlaying;
    int8 m_lastActionPriority;

    _smart_ptr<BaseAction> m_pCurrentAction;
};



