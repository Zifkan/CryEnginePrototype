#include "PlayerCharacterActions.h"

PlayerCharacterActions::PlayerCharacterActions()
{
    MovementSide.get_observable().start_with(0.0f).combine_latest(MovementForward.get_observable().start_with(0.0f))
    .subscribe([this](std::tuple<float, float> v) {MovementSubject.get_subscriber().on_next(Vec2(v._Myfirst._Val,v._Get_rest()._Myfirst._Val)); });


    Movement.get_observable().map([](bool isPressed) { return HoldDetectionStruct<MovementType>(isPressed, gEnv->pTimer->GetCurrTime(), WALK); })
    .scan(HoldDetectionStruct<MovementType>(false, 0.0f, WALK), [](HoldDetectionStruct<MovementType> last, HoldDetectionStruct<MovementType> current)
    {
        if (current.IsSignal && !last.IsSignal)
        {
            return HoldDetectionStruct<MovementType>(current.IsSignal, current.Time, DODGE);
        }

        if (current.IsSignal && last.IsSignal)
        {
            return HoldDetectionStruct<MovementType>(last.IsSignal, last.Time, current.Time - last.Time > 0.25f ? SPRINT : DODGE);
        }
        return current;
    })
    .map([](HoldDetectionStruct<MovementType> value) {return value.TypeValue; })
    .distinct_until_changed()
    .subscribe([this](MovementType value){ MovementTypeSubject.get_subscriber().on_next(value); });

    Attack.get_observable()
    .map([](bool isPressed) { return HoldDetectionStruct<bool>(isPressed, 0, false); })
    .scan(HoldDetectionStruct<bool>(false, 0, false), [](HoldDetectionStruct<bool> last, HoldDetectionStruct<bool> current)
    {
        float time = 0.0f;

        if (last.Time >= 0.25f)
        {
            return HoldDetectionStruct<bool>(true, 0, true);
        }

        if (current.IsSignal)
        {
            time += last.Time + gEnv->pTimer->GetFrameTime();
            return HoldDetectionStruct<bool>(false, time, false);
        }

        if (last.Time > 0.1f && last.Time < 0.25f)
        {
            return HoldDetectionStruct<bool>(true, 0, false);
        }

        return HoldDetectionStruct<bool>(false, time, false);
    })
    .subscribe([this](HoldDetectionStruct<bool> attackData)
    {
        if (attackData.IsSignal)
            AttackSubject.get_subscriber().on_next(attackData.TypeValue?FORCE_ATTACK:ATTACK);
    });
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
    Attack.get_subscriber().on_next(isAttack);
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
    Movement.get_subscriber().on_next(signal);
}


