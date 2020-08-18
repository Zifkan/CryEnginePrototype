#pragma once

#include "CopyTransformFromCryEntitySystem.h"
#include "CopyTransformToCryEntitySystem.h"
#include "TRSToLocalToWorldSystem.h"
#include "WorldToLocalSystem.h"
#include "Systems/SystemLauncher.h"



class TransformSystemLaunch : public SystemLauncher
{
public:
    explicit TransformSystemLaunch(CryEcsWorld* world)
        : SystemLauncher(world)
    {
        RegisterSystem(new CopyTransformFromCryEntitySystem());
        RegisterSystem(new TRSToLocalToWorldSystem());
        RegisterSystem(new WorldToLocalSystem());
        RegisterSystem(new CopyTransformToCryEntitySystem());

    }


};
