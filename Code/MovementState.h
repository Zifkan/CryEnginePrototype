#pragma once
#include "IBaseState.h"
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include "Components/Player.h"

class CMovementState :public IBaseState
{
public:

    CMovementState(IEntity* entity, float sprintAnimRatio, float sprintRatio,IEntity* pMainCamera, Vec2* moveDirection, MovementType* movementType):
    m_pEntity(entity),
    m_pMainCamera(pMainCamera),
    m_sprintAnimRatio(sprintAnimRatio),   
    m_movementType(movementType), 
    m_sprintRatio(sprintRatio),
    m_moveDirection(moveDirection)
    {    
        m_pCharacterController = entity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
        m_pAnimationComponent = entity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

        m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");
        m_fragmentId = m_pAnimationComponent->GetFragmentId("Walk");
    }

   
    void OnEnterState() override;
    void OnExitState() override;
    void Update(float frameTime) override;

   
private:
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

    IEntity* m_pEntity;
    IEntity* m_pMainCamera = nullptr;

    float m_sprintAnimRatio;

    TagID m_rotateTagId;
    MovementType* m_movementType;
    float m_sprintRatio;
    Vec2* m_moveDirection;
    FragmentID desiredFragmentId;
};
