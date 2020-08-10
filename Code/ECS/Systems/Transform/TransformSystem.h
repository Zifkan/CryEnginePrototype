#pragma once

#include <CryMath/Transform.h>



#include "Components/CryEntityComponent.h"
#include "Components/TransformComponents.h"
#include "Core/IConvertGameObjectToEntity.h"
#include "ECS/Components/PlayerComponents.h"


class TransformSystem final :public SystemBase<LocalToWorld, CryEntityComponent>
{
public:
     void OnCreate() override;
};
