#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "BaseAction.h"

class AttackAction : public BaseAction
{
public:
    AttackAction(IEntity* characterEntity, Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
        Cry::DefaultComponents::CCharacterControllerComponent* characterController,
        ICharacterActions* characterAction, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
        : BaseAction(characterAction,priority, fragmentID, fragTags, flags, scopeMask, userToken)
        , m_lastCallTime(0.0f)
        , m_attackId(0)
        , m_pAnimationComponent(animationComponent)
        , m_pCharacterController(characterController)
        , m_pCharacterEntity(characterEntity)
    {
    }
           

    EStatus Update(float timePassed) override;

    void Enter() override;
    void Exit() override;

private:

    float m_lastCallTime;

    uint8 m_attackId;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent;
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
    IEntity* m_pCharacterEntity;
};
