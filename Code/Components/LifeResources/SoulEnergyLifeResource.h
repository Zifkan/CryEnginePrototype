#pragma once
#include "BaseLifeResource.h"

class CSoulEnergyLifeResource :public IBaseLifeResource
{
public:


    CSoulEnergyLifeResource(float maxValue, float discreaseFactor)
        : IBaseLifeResource(maxValue),
          m_discreaseFactor(discreaseFactor)
    {
       
    }
   
    void Update(float timePassed) override;

private:
    float m_discreaseFactor;
};
