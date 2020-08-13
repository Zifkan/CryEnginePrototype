#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "Core/IConvertGameObjectToEntity.h"

class CConvertToEntity  : public IEntityComponent,public IConvertGameObjectToEntity
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CConvertToEntity>& desc)
    {
        desc.SetGUID("{15038638-6AEB-4F52-AEB5-80658F82B92F}"_cry_guid);
        desc.SetEditorCategory("ECS");
        desc.SetLabel("Convert To Entity");
    }

    Cry::Entity::EventFlags GetEventMask() const override;

    void ProcessEvent(const SEntityEvent& event) override;

    void Convert(flecs::entity entity, CEntityManager dstManager) override {};   
   
};
