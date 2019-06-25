#pragma once
#include "BaseLifeResource.h"


class CStaminaLifeResource :public IBaseLifeResource
{
public:


    CStaminaLifeResource(float maxValue, float regenFactor)
        : IBaseLifeResource(maxValue),
          m_regenFactor(regenFactor)
    {
       /* Value.get_observable().subscribe([](float v)
        {
            if (v<100.f)
                CryLog("Stamina: %f",v);
        });*/
    }  

    void Update(float timePassed) override;

private:
    void Regen(float timePassed);

    float m_regenFactor;
};
