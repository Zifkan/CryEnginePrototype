#pragma once
#include "BaseLifeResource.h"

class CHealthLifeResource :public IBaseLifeResource
{
public:


    CHealthLifeResource(float maxValue)
        : IBaseLifeResource(maxValue)
    {
    }


    void Update(float timePassed) override;
};
