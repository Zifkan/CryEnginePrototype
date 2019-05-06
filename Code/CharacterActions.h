#pragma once
#include "RxCpp/rx.hpp"

class  ICharacterActions
{
   

public:
   
    virtual ~ICharacterActions() {}

    rxcpp::subjects::subject<Vec2> MovementSubject;
    rxcpp::subjects::subject<bool>  AttackSubject;
    rxcpp::subjects::subject<float> RotateYawSubject;
    rxcpp::subjects::subject<float> RotatePitchSubject;
    rxcpp::subjects::subject<bool>  SprintSubject;

    void virtual SetMovementForward(float y) = 0;
    void virtual SetMovementSide(float y) = 0;
    void virtual Attack(bool isAttack) = 0;
    void virtual SetRotateYaw(float x) = 0;
    void virtual SetRotatePitch(float y) = 0;
    void virtual SetSprint(bool isSprint) = 0;

protected:
    rxcpp::subjects::subject<float> MovementForward;
    rxcpp::subjects::subject<float> MovementSide;
};
