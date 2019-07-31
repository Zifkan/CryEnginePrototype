#pragma once
#include "RxCpp/rx.hpp"

enum MovementType
{
    WALK,
    DODGE,
    SPRINT,
};

enum AttackType
{
    ATTACK,
    FORCE_ATTACK
};


template <typename T>
struct HoldDetectionStruct
{
    bool IsSignal;
    float Time;
    T TypeValue;


    HoldDetectionStruct(bool isSignal, float time, T typeValue) : IsSignal(isSignal), Time(time), TypeValue(typeValue)
    {
    }
};


class  ICharacterActions
{
   

public:
   
    virtual ~ICharacterActions() {}

    rxcpp::subjects::subject<Vec2> MovementSubject;
    rxcpp::subjects::subject<AttackType>  AttackSubject;
    rxcpp::subjects::subject<float> RotateYawSubject;
    rxcpp::subjects::subject<float> RotatePitchSubject;
    rxcpp::subjects::subject<MovementType>  MovementTypeSubject;
    rxcpp::subjects::subject<bool>  BlockSubject;
    rxcpp::subjects::subject<bool>  SetFocusOnSubject;

    void virtual SetMovementForward(float y) = 0;
    void virtual SetMovementSide(float x) = 0;
    void virtual SetAttack(bool isAttack) = 0;
    void virtual SetRotateYaw(float x) = 0;
    void virtual SetRotatePitch(float y) = 0;
    void virtual SetMoveSignal(bool signal) = 0;
    void virtual SetBlockSignal(bool signal) = 0;
    void virtual SetFocusOnSignal(bool signal) = 0;

protected:
    rxcpp::subjects::subject<float> MovementForward;
    rxcpp::subjects::subject<float> MovementSide;
   
};