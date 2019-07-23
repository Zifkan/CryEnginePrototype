#pragma once
#include <CryEntitySystem/IEntityComponent.h>

class CAICharacterActions : public IEntityComponent,  ICharacterActions
{
public:
    void SetMovementForward(float y) override;
    void SetMovementSide(float x) override;
    void SetAttack(bool isAttack) override;
    void SetRotateYaw(float x) override;
    void SetRotatePitch(float y) override;
    void SetMoveSignal(bool signal) override;
};
