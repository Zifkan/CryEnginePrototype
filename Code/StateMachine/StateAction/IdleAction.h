#pragma once
#include "BaseAction.h"
#include "Components/Camera/CameraController.h"

class IdleAction :public BaseAction
{
public:
    IdleAction(IEntity* characterEntity, IEntity* mainCamera, ICharacterActions* characterAction, int priority,
               FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState& fragTags = TAG_STATE_EMPTY,
               uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0);

private:
    EStatus Update(float timePassed) override;
   
    Vec3 GetLookDirNormalized(Vec3 target, Vec3 location);
    void SetRotation();

    CCameraController* m_pMainCameraComponent = nullptr;
    IEntity* m_pMainCameraEntity = nullptr;

    float timer;
    TagID m_rotateTagId = TAG_ID_INVALID;
    Vec3 m_lastDirection;
};
