#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

#include "CollisionDetectorComponent.h"


static void RegisterCollisionDetectorComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCollisionDetectorComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCollisionDetectorComponent)

Cry::Entity::EntityEventMask CCollisionDetectorComponent::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_COLLISION) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CCollisionDetectorComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {   
    case ENTITY_EVENT_COLLISION:
    {          
        const EventPhysCollision* pPhysCollision = reinterpret_cast<const EventPhysCollision*>(event.nParam[0]);
        const int thisEntityIndex = static_cast<int>(event.nParam[1]);

        if (pPhysCollision!=nullptr)
        {

            IPhysicalEntity *pHitEntityPhysics = pPhysCollision->pEntity[0];
            IEntity *pHitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pHitEntityPhysics);

            //IPhysicalEntity *pColliderPhysics = pPhysCollision->pEntity[1];
            //IEntity *pCollider = gEnv->pEntitySystem->GetEntityFromPhysics(pColliderPhysics);

            if (pHitEntity && pHitEntity->GetId()!=m_pEntity->GetId())
            {
                if (std::find(m_lastHitEntities.begin(), m_lastHitEntities.end(), pHitEntity) == m_lastHitEntities.end())
                {
                    m_lastHitEntities.push_back(pHitEntity);
                    OnCollisionEnter.get_subscriber().on_next(pHitEntity);

                  /*  CryLog("Frame: %i", lastFrame);
                    for (int i = 0; i < m_lastHitEntities.size(); ++i)
                    {                       
                        CryLog("EntityName: %s", m_lastHitEntities[i]->GetName());
                    }
                    CryLog("**************************************");*/
                }
               
                if (std::find(m_newHitEntities.begin(), m_newHitEntities.end(), pHitEntity) == m_newHitEntities.end())
                {
                    m_newHitEntities.push_back(pHitEntity);
                }
            }           
        }
    }
    break;
        
    case ENTITY_EVENT_UPDATE:
    {
        if (!gEnv->IsGameOrSimulation()) return;

       SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

            

       if (pCtx->frameID-lastFrame>1)
       {     
          // if (!m_newHitEntities.empty())
           {
               std::vector<IEntity*> diffResult;
               std::vector<IEntity*> sameResult;
               std::sort(m_newHitEntities.begin(), m_newHitEntities.end());
               std::sort(m_lastHitEntities.begin(), m_lastHitEntities.end());

               std::set_difference(m_lastHitEntities.begin(), m_lastHitEntities.end(), m_newHitEntities.begin(), m_newHitEntities.end(), std::back_inserter(diffResult));
               std::set_intersection(m_newHitEntities.begin(), m_newHitEntities.end(), m_lastHitEntities.begin(), m_lastHitEntities.end(), std::back_inserter(sameResult));


              /* for (int i = 0; i < diffResult.size(); ++i)
               {
                   it = std::find(m_lastHitEntities.begin(), m_lastHitEntities.end(), diffResult[i]);

                   if (it != m_lastHitEntities.end())
                   {
                       OnCollisionExit.get_subscriber().on_next(diffResult[i]);
                       m_lastHitEntities.erase(it);
                   }
                  
               }*/

               for (int i = 0; i < diffResult.size(); ++i)
               {
                   OnCollisionExit.get_subscriber().on_next(diffResult[i]);
               }
               m_lastHitEntities = sameResult;
           }

           lastFrame = pCtx->frameID;
       }

       for (int i = 0; i < m_lastHitEntities.size(); ++i)
       {
           OnCollisionStay.get_subscriber().on_next(m_lastHitEntities[i]);
       }

       m_newHitEntities.clear();
    }
    break;
    }
}

void CCollisionDetectorComponent::Initialize()
{
   /* OnCollisionEnter.get_observable().subscribe([this] (IEntity* entity){CryLog("OnCollisionEnter: %s",entity->GetName()); });
    OnCollisionStay.get_observable().subscribe([this] (IEntity* entity){CryLog("OnCollisionStay: %s",entity->GetName()); });
    OnCollisionExit.get_observable().subscribe([this] (IEntity* entity){CryLog("OnCollisionExit: %s",entity->GetName()); });*/
}                                                                                                   
