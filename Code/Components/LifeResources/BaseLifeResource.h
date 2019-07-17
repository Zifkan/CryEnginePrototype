#pragma once
#include "rxcpp/rx.hpp"


class IBaseLifeResource
{
public:
    IBaseLifeResource(float maxValue)
        : Value(maxValue), m_maxValue(maxValue)
    {
    }

    rxcpp::subjects::behavior<float> Value;
      

    virtual void ChangeValue(float value);    
    virtual void Update(float timePassed) = 0;

protected:

    float m_maxValue;
};

inline void IBaseLifeResource::ChangeValue(float value)
{
    auto result = Value.get_value() + value;

    if (result <= 0)
        result = 0;

    if (result >= m_maxValue)
        result = m_maxValue;
 /*   CryLog("Hit Result: %f", result);*/
    Value.get_subscriber().on_next(result);
}
