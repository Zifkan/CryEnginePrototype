#include "PlayerCharacterActions.h"

void PlayerCharacterActions::SetMovement(Vec2 movement)
{
   MovementSubject.get_subscriber().on_next(movement);
}

void PlayerCharacterActions::Attack(bool isAttack)
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

void PlayerCharacterActions::SetSprint(bool isSprint)
{
    SprintSubject.get_subscriber().on_next(isSprint);
}
