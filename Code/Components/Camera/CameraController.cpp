#include "StdAfx.h"
#include "CameraController.h"
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>
#include <CryPhysics/physinterface.h>
#include "Components/CopyTransformFromCryEntity.h"
#include "Components/CryEntityComponent.h"
#include "Components/TransformComponents.h"
#include "Core/CryEcsWorld.h"
#include "ECS/Components/CameraComponent.h"



static void RegisterCameraController(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCameraController));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCameraController)


void CCameraController::Initialize()
{   
    m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
 
    SetName("GameCamera");

    currentRadius = radius;
    entityName = "Camera";

}

void CCameraController::Convert(flecs::entity entity, CEntityManager dstManager)
{
    
     dstManager.SetComponentData<CameraComponent>(entity,
    {
            m_pEntity,heightOffset, focusHeightOffset,x,y,radius, currentRadius,xSpeed,ySpeed, yMinLimit,yMaxLimit,m_pitchLimit
         });

     dstManager.SetComponentData<LocalToWorld>(entity,{ZERO});
     dstManager.SetComponentData<CopyTransformFromCryEntity>(entity, {});
     dstManager.SetComponentData<CryEntityComponent>(entity, {m_pEntity});
     dstManager.SetComponentData<Rotation>(entity, { ZERO });    
     dstManager.SetComponentData<Translation>(entity, { ZERO });    
}

Cry::Entity::EventFlags CCameraController::GetEventMask() const
{
    return CConvertToEntity::GetEventMask();
}

void CCameraController::ProcessEvent(const SEntityEvent& event)
{
     CConvertToEntity::ProcessEvent(event);    
}
