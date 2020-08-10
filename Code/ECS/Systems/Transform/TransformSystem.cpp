#include "StdAfx.h"
#include "TransformSystem.h"

void TransformSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e, LocalToWorld& localToWorld, CryEntityComponent& cryEntity)
    {
        //CryLog("Trans: x = %f; y = %f", trs.pEntity);
        cryEntity.Value->SetLocalTM(localToWorld.Value);
      
    });
}
