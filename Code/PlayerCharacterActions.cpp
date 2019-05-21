#include "PlayerCharacterActions.h"

PlayerCharacterActions::PlayerCharacterActions()
{
    MovementSide.get_observable().start_with(0.0f).combine_latest(MovementForward.get_observable().start_with(0.0f))
    .subscribe([this](std::tuple<float, float> v) {MovementSubject.get_subscriber().on_next(Vec2(v._Myfirst._Val,v._Get_rest()._Myfirst._Val)); });
}

void PlayerCharacterActions::SetMovementForward(float y)
{
    MovementForward.get_subscriber().on_next(y);
}

void PlayerCharacterActions::SetMovementSide(float x)
{
    MovementSide.get_subscriber().on_next(x);
}


void PlayerCharacterActions::SetAttack(bool isAttack)
{
    AttackSubject.get_subscriber().on_next(isAttack);
}

void PlayerCharacterActions::SetRotateYaw(float x)
{
    RotateYawSubject.get_subscriber().on_next(x);
}

void PlayerCharacterActions::SetRotatePitch(float y)
{
    RotatePitchSubject.get_subscriber().on_next(y);
}

void PlayerCharacterActions::SetMoveSignal(bool signal)
{
    SprintSubject.get_subscriber().on_next(signal);
}


