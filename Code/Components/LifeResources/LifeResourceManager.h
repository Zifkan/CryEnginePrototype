#pragma once
#include <map>
#include <typeindex>
#include "BaseLifeResource.h"
#include <CryEntitySystem/IEntityComponent.h>


class CLifeResourceManagerComponent : public IEntityComponent
{
public:

    static void ReflectType(Schematyc::CTypeDesc<CLifeResourceManagerComponent>& desc)
    {
        desc.SetGUID("{9F538F51-1782-47B7-B218-63CEE4B938B3}"_cry_guid);
        desc.SetEditorCategory("Player");
        desc.SetComponentFlags({ EFlags::Singleton });
    }


    void RegisterResource(std::type_index index,IBaseLifeResource* resource);

    template <class T>
    IBaseLifeResource* GetResource()
    {
        return m_lifeResources[typeid(T)];
    }

    virtual Cry::Entity::EntityEventMask GetEventMask() const;

protected:
    virtual void ProcessEvent(const SEntityEvent& event);

    void Update(float timePassed);   

    std::map<std::type_index, IBaseLifeResource*> m_lifeResources;
};
