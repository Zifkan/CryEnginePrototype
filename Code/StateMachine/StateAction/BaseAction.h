#pragma once
#include <CryAnimation/ICryMannequin.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>


class CStateMachine;

class BaseAction :public IAction
{
public:

    BaseAction(IEntity* pCharacterEntity, int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState& fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
        : IAction(priority, fragmentID, fragTags, flags, scopeMask, userToken)
        ,m_pCharacterEntity(pCharacterEntity)
    {        
        ScopeLayer = 0;
        m_pCharacterController = pCharacterEntity->GetComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
        m_pAnimationComponent = pCharacterEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
        m_pISkeletonAnim = m_pAnimationComponent->GetCharacter()->GetISkeletonAnim();
    }

    DEFINE_ACTION(typeid(this).name())   
    
    IAction* CreateSlaveAction(FragmentID slaveFragID, const TagState& fragTags, SAnimationContext& context) override;

   
    void RegisterStateMachine(CStateMachine* stateMachine);

    virtual void Enter() override;
    virtual void Exit() override;

    virtual void OnActionFinished();

    uint8 ScopeLayer;

protected:
    ISkeletonAnim* m_pISkeletonAnim;
    CStateMachine* m_pStateMachine;
    IEntity* m_pCharacterEntity;
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent;


    float GetNormalizedTime(uint32 layer);

    void SetAnimationControlMovement();
};
