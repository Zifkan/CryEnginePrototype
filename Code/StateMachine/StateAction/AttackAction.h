﻿#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "BaseAction.h"
#include "Components/LifeResources/BaseLifeResource.h"

class AttackAction : public BaseAction
{
public:
    AttackAction(IBaseLifeResource* lifeResource, IEntity* characterEntity,
                 Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
                 Cry::DefaultComponents::CCharacterControllerComponent* characterController,
                 ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID,
                 const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0,
                 uint32 userToken = 0);


    EStatus Update(float timePassed) override;

    void Enter() override;
    void Exit() override;

private:

    float m_lastCallTime;

    uint8 m_attackId;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent;
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
    IEntity* m_pCharacterEntity;
    IBaseLifeResource* m_pLifeResource;
};