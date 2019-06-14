#pragma once
#include "BaseLifeResource.h"


class CStaminaLifeResource :public IBaseLifeResource
{
public:


    CStaminaLifeResource(float maxValue, float regenFactor)
        : IBaseLifeResource(maxValue),
          m_regenFactor(regenFactor)
    {
    }

    void SetValue(float value) override;

    void Update(float timePassed) override;

private:
    void Regen(float timePassed);

    float m_regenFactor;
};
