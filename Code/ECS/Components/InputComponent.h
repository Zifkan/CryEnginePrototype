#pragma once

#include <CryInput/IInput.h>
#include <CryMath/Cry_Vector2.h>

struct CombatMapData
{
    EInputState FirstAttack;
    EInputState SecondAttack;
    EInputState FirstAbility;
    EInputState SecondAbility;
};

struct InputComponent
{
    InputComponent(){}

    Vec2 MovementAxis;
    Vec2 AimAxis;

    EInputState Sprint;

    CombatMapData Combat;
};


