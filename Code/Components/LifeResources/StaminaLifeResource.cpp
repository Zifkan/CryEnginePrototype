#include "StdAfx.h"
#include "StaminaLifeResource.h"


void CStaminaLifeResource::Update(float timePassed)
{
    Regen(timePassed);
}

void CStaminaLifeResource::Regen(float timePassed)
{
    if (Value.get_value()>=m_maxValue) return;

    Value.get_subscriber().on_next(Value.get_value() + (timePassed*m_regenFactor));   
}
