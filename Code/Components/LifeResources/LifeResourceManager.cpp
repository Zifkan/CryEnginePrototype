#include "StdAfx.h"
#include "LifeResourceManager.h"

void CLifeResourceManager::RegisterResource(std::type_index index,IBaseLifeResource* resource)
{
    m_lifeResources.insert(std::pair<std::type_index, IBaseLifeResource*>(index, resource));
}


void CLifeResourceManager::Update(float timePassed)
{
    for (auto const& resource : m_lifeResources)
    {
        resource.second->Update(timePassed);
    }
}
