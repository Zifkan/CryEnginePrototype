#pragma once

#include "Systems/SystemBase.h"
#include "Components/CopyTransformFromCryEntity.h"
#include "Components/CryEntityComponent.h"
#include "Components/TransformComponents.h"


class CopyTransformFromCryEntitySystem final :public SystemBase<CopyTransformFromCryEntity,LocalToWorld, CryEntityComponent>
{
public:
    void OnCreate() override;
};
