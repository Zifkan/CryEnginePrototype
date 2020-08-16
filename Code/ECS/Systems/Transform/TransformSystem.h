#pragma once

#include <CryMath/Transform.h>



#include "Components/CryEntityComponent.h"
#include "Components/TransformComponents.h"


class TransformSystem final :public SystemBase<LocalToWorld, CryEntityComponent>
{
public:
     void OnCreate() override;
};
