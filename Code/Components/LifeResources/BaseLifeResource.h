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
      

    virtual void SetValue(float value) = 0;    
    virtual void Update(float timePassed) = 0;

protected:

    float m_maxValue;
};
