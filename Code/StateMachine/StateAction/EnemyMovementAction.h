﻿#pragma once
#include "BaseAction.h"

class IBaseLifeResource;

class EnemyMovementAction : public BaseAction
{
public:

    EnemyMovementAction(IEntity* characterEntity, Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
        Cry::DefaultComponents::CCharacterControllerComponent* characterController,      
        ICharacterActions* characterAction,
        int priority,
        FragmentID fragmentID = FRAGMENT_ID_INVALID,
        const TagState& fragTags = TAG_STATE_EMPTY,
        uint32 flags = 0,
        ActionScopes scopeMask = 0,
        uint32 userToken = 0);

    ~EnemyMovementAction();

    EStatus Update(float timePassed) override;
private:

    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
    IEntity* m_pCharacterEntity;

    Vec2 m_movePoint;
    MovementType m_movementType;

    float m_sprintRatio;
    float m_sprintAnimRatio;
    rxcpp::composite_subscription m_movementCompositeSubscribtion;
    rxcpp::composite_subscription m_movementTypeSubscribtion;
};
