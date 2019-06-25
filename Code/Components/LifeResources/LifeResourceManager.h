#pragma once
#include <map>
#include <typeindex>
#include "BaseLifeResource.h"



class CLifeResourceManager
{
public:

    void RegisterResource(std::type_index index,IBaseLifeResource* resource);

    template <class T>
    IBaseLifeResource* GetResource()
    {
        return m_lifeResources[typeid(T)];
    }

    void Update(float timePassed);

private:
   
    std::map<std::type_index, IBaseLifeResource*> m_lifeResources;
};
