#include "StdAfx.h"
#include "CameraController.h"
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>
#include <CryPhysics/physinterface.h>


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

Cry::Entity::EventFlags CCameraController::GetEventMask() const
{
    return ENTITY_EVENT_START_GAME | ENTITY_EVENT_UPDATE | ENTITY_EVENT_EDITOR_PROPERTY_CHANGED;
}

void CCameraController::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {
        m_pPlayerEntity = gEnv->pEntitySystem->FindEntityByName("Player");
        //TODO: Create select targer system
        m_pEnemy = gEnv->pEntitySystem->FindEntityByName("Enemy");
    }
    break;
    case ENTITY_EVENT_UPDATE:
    {
        if (!gEnv->IsGameOrSimulation()) return;

        SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
        UpdateCamera(pCtx->fFrameTime); 
        CollisionDetection(pCtx->fFrameTime);
    }
    break;
    }
}
/*
void CCameraController::InitInput()
{   

    m_pCharacterActions->RotateYawSubject.get_observable().start_with(0.0f).combine_latest(m_pCharacterActions->RotatePitchSubject.get_observable().start_with(0.0f))
    .map([](std::tuple<float, float> v){ return Vec2(CLAMP(v._Myfirst._Val,-1,1), CLAMP(v._Get_rest()._Myfirst._Val, -1, 1));})  
    .subscribe([this](Vec2 vec2)
    {
        if (crymath::abs(vec2.x) < 0.4f  &&  crymath::abs(vec2.y) > crymath::abs(vec2.x))
            vec2 = Vec2(0, vec2.y);
        if (crymath::abs(vec2.y) < 0.4f  &&  crymath::abs(vec2.x) > crymath::abs(vec2.y))
            vec2 = Vec2(vec2.x,0);

        m_deltaRotation = vec2;
    });

    m_pCharacterActions->SetFocusOnSubject.get_observable().subscribe([this](bool isHit)
    {
       if (isHit)
       {
           m_pTargetEntity = m_pTargetEntity == nullptr ? m_pEnemy : nullptr;
       }
    });

}
*/
void CCameraController::SetTargetEntity(IEntity* entity)
{
    m_pTargetEntity = entity;
}

bool CCameraController::IsCameraFocus()
{
    return m_pTargetEntity ? true : false;
}

void CCameraController::UpdateCamera(float frameTime)
{
    Quat rotation = ZERO;
    if(m_pTargetEntity)
    {
        Vec3 target = m_pTargetEntity->GetPos();

        Vec3 location = m_pPlayerEntity->GetPos() + Vec3(0, 0, heightOffset);
              
       x = target.x - location.x;
       y = target.y - location.y;

       Vec3 lookDir = Vec3(x, y, target.z - location.z).normalize();
       rotation = Quat::CreateRotationVDir(lookDir);

       auto angles = Ang3::GetAnglesXYZ(rotation);
       angles = Vec3(RAD2DEG(angles.x), RAD2DEG(angles.y), RAD2DEG(angles.z));

       if (angles.x <= m_pitchLimit)
           angles.x =  m_pitchLimit;

       rotation = Quat::CreateRotationXYZ(Vec3(DEG2RAD(angles.x), DEG2RAD(angles.y), DEG2RAD(angles.z)));
    }
    else
    {
        x -= m_deltaRotation.x * xSpeed * currentRadius * frameTime;
        y -= m_deltaRotation.y * ySpeed * frameTime;
   
        y = ClampAngle(y, yMinLimit, yMaxLimit);
        auto angles = Ang3::GetAnglesXYZ(m_pEntity->GetRotation());
        rotation = Quat::CreateRotationXYZ(Ang3(DEG2RAD(y),0, DEG2RAD(x)));
    }

  //  CryLog("rotation: x = %f, y = %f, z = %f", rotation.v.x, rotation.v.y, rotation.v.z);

    Vec3 negDistance = Vec3(0.0f, -currentRadius, 0);
    Vec3 position = rotation * negDistance + (m_pPlayerEntity->GetWorldPos()+ Vec3(0, 0, heightOffset));

    CryTransform::CTransform tr = CryTransform::CTransform(position, CryTransform::CRotation(rotation), Vec3(1, 1, 1));
    m_pEntity->SetWorldTM(tr.ToMatrix34());
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


float CCameraController::ClampAngle(float angle, float min, float max)
{
    if (angle < -360)
        angle += 360;
    if (angle > 360)
        angle -= 360;
    return CLAMP(angle, min, max);
}

void CCameraController::Convert(flecs::entity entity, CEntityManager dstManager)
{
 
}

