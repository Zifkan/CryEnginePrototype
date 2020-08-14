#include "StdAfx.h"
#include "CameraController.h"
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>
#include <CryPhysics/physinterface.h>
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
}

void CCameraController::CollisionDetection(float frameTime)
{

    // Skip the target actor for this.
    ray_hit rayhit;
    static IPhysicalEntity* pSkipEnts[10];
    pSkipEnts[0] = m_pPlayerEntity->GetPhysics();
    auto org = m_pPlayerEntity->GetWorldPos() + Vec3(0, 0, heightOffset);
    // Perform the ray cast.
    int hits = gEnv->pPhysicalWorld->RayWorldIntersection(org,  m_pEntity->GetWorldPos()-org,
        ent_static | ent_sleeping_rigid | ent_rigid | ent_independent | ent_terrain, rwi_stop_at_pierceable | rwi_colltype_any,
        &rayhit, 1, pSkipEnts, 2);

    if (hits)
    {
        currentRadius -= frameTime;
    }
    else
    {
        if (currentRadius<radius)
            currentRadius += frameTime;
    }
}
void CCameraController::Convert(flecs::entity entity, CEntityManager dstManager)
{
    dstManager.SetComponentData<CameraComponent>(entity,
    {
            m_pEntity,heightOffset, focusHeightOffset,x,y,radius, currentRadius,xSpeed,ySpeed, yMinLimit,yMaxLimit,m_pitchLimit
         });

   

}

Cry::Entity::EventFlags CCameraController::GetEventMask() const
{
    return CConvertToEntity::GetEventMask();
}

void CCameraController::ProcessEvent(const SEntityEvent& event)
{
    CConvertToEntity::ProcessEvent(event);
}
