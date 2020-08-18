#include "CopyTransformToCryEntitySystem.h"


void CopyTransformToCryEntitySystem::OnCreate()
{
    SystemRun->each([](flecs::entity e, LocalToWorld& localToWorld, CryEntityComponent& cryEntity)
    {        
        cryEntity.Value->SetWorldTM(localToWorld.Value);
    });
}
