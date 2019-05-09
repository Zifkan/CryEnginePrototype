#pragma once
#include "RxCpp/rx.hpp"

enum MovementType
{
    WALK,
    DODGE,
    SPRINT,
};

class  ICharacterActions
{
   

public:
   
    virtual ~ICharacterActions() {}

    rxcpp::subjects::subject<Vec2> MovementSubject;
    rxcpp::subjects::subject<bool>  AttackSubject;
    rxcpp::subjects::subject<float> RotateYawSubject;
    rxcpp::subjects::subject<float> RotatePitchSubject;
    rxcpp::subjects::subject<MovementType>  MovementTypeSubject;

    void virtual SetMovementForward(float y) = 0;
    void virtual SetMovementSide(float x) = 0;
    void virtual SetAttack(bool isAttack) = 0;
    void virtual SetRotateYaw(float x) = 0;
    void virtual SetRotatePitch(float y) = 0;
    void virtual SetMoveType(MovementType movementType) = 0;

protected:
    rxcpp::subjects::subject<float> MovementForward;
    rxcpp::subjects::subject<float> MovementSide;
};
