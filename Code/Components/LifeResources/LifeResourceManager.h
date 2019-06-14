#pragma once
#include <map>
#include <typeindex>
#include "BaseLifeResource.h"

class IBaseLifeResource;

class CLifeResourceManager
{
public:

    void RegisterResource(std::type_index index,IBaseLifeResource* resource);

    IBaseLifeResource* GetResource(std::type_index resourceType);

    void Update(float timePassed);

private:
   
    std::map<std::type_index, IBaseLifeResource*> m_lifeResources;
};
