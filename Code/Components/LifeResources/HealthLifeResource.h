#pragma once
#include "BaseLifeResource.h"

class CHealthResource :public IBaseLifeResource
{
public:


    CHealthResource(float maxValue)
        : IBaseLifeResource(maxValue)
    {
    }


    void Update(float timePassed) override;
};
