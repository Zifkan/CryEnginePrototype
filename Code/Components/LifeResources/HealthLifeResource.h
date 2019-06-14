#pragma once
#include "BaseLifeResource.h"

class СHealthResource :public IBaseLifeResource
{
public:


    СHealthResource(float maxValue)
        : IBaseLifeResource(maxValue)
    {
    }


    void Update(float timePassed) override;
     void SetValue(float value)override;
};
