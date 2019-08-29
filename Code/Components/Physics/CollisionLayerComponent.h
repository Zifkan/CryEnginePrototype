#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "CollisionLayerStruct.h"

class CCollisionLayerComponent final :public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CCollisionLayerComponent>& desc)
    {
        desc.SetGUID("{389B3B0C-5189-4C22-940C-E7448549322A}"_cry_guid);
        desc.SetEditorCategory("Physics");
        desc.SetLabel("Collision Layer Component");

        desc.AddMember(&CCollisionLayerComponent::m_type, 'type', "CollisionType", "Collision Type", "Defines the type collision classes of this object", Schematyc::CArray<ECollisionLayerFlags>());
        desc.AddMember(&CCollisionLayerComponent::m_ignore, 'ignr', "CollisionIgnore", "Collision Ignore", "Defines the ignore collision classes of this object", Schematyc::CArray<ECollisionLayerFlags>());
    }
    
   
    Cry::Entity::EventFlags GetEventMask() const override;
    void ProcessEvent(const SEntityEvent& event) override;

private:   
    Schematyc::CArray<ECollisionLayerFlags> m_type;
    Schematyc::CArray<ECollisionLayerFlags> m_ignore;
};
