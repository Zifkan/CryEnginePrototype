#pragma once
#include "CharacterActions.h"

class PlayerCharacterActions : public ICharacterActions
{
public:

    void SetMovement(Vec2 moveMent) override;
    void Attack(bool isAttack) override;
    void SetRotateYaw(float x) override;
    void SetRotatePitch(float y) override;
    void SetSprint(bool isSprint) override;
};
