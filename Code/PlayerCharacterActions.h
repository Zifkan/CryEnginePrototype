﻿#pragma once
#include "CharacterActions.h"

class PlayerCharacterActions : public ICharacterActions
{
public:

    PlayerCharacterActions();

    void SetMovementForward(float y) override;
    void SetMovementSide(float x) override;
    void Attack(bool isAttack) override;
    void SetRotateYaw(float x) override;
    void SetRotatePitch(float y) override;
    void SetSprint(bool isSprint) override;

private:
    Vec2 m_movement = ZERO;
};
