﻿#include "StdAfx.h"
#include "SoulEnergyLifeResource.h"

void CSoulEnergyLifeResource::SetValue(float value)
{
}

void CSoulEnergyLifeResource::Update(float timePassed)
{
    Value.get_subscriber().on_next(Value.get_value() - timePassed*m_discreaseFactor);
}
