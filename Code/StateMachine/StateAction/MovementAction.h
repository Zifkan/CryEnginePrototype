#pragma once
#include <CryAnimation/ICryMannequin.h>
#include "BaseAction.h"
#include "Components/LifeResources/BaseLifeResource.h"
#include "Components/Camera/CameraController.h"

class MovementAction : public BaseAction
{
public:

    MovementAction(IBaseLifeResource* lifeResource,IEntity* characterEntity,Cry::DefaultComponents::CAdvancedAnimationComponent* animationComponent,
        Cry::DefaultComponents::CCharacterControllerComponent* characterController,
        IEntity* mainCamera,
        ICharacterActions* characterAction, 
        int priority, 
        FragmentID fragmentID = FRAGMENT_ID_INVALID,
        const TagState& fragTags = TAG_STATE_EMPTY, 
        uint32 flags = 0, 
        ActionScopes scopeMask = 0,
        uint32 userToken = 0);

    ~MovementAction();

    EStatus Update(float timePassed) override;

  
    void Enter();
    void Exit();
private:

    Vec3 GetLookDirNormalized(Vec3 target,Vec3 location);


    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
    CCameraController* m_pMainCameraComponent = nullptr;
    IEntity* m_pMainCameraEntity;
    IEntity* m_pCharacterEntity;
    IBaseLifeResource* m_pLifeResource;

    Vec2 m_moveDirection;
    MovementType m_movementType;

    float m_sprintRatio;
    float m_sprintAnimRatio;
    rxcpp::composite_subscription m_movementCompositeSubscribtion;
    rxcpp::composite_subscription m_movementTypeSubscribtion;
};
