#include "StdAfx.h"
#include "CameraController.h"


#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "Components/Player.h"


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
}

uint64 CCameraController::GetEventMask() const
{
    return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED);
}

void CCameraController::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {

    }
    break;
    case ENTITY_EVENT_UPDATE:
    {
        SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
        UpdateCamera(pCtx->fFrameTime);
    }
    break;
    }
}

void CCameraController::InitInput(ICharacterActions* charActions)
{
    m_pCharacterActions = charActions;

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
}

void CCameraController::SetTargetEntity(IEntity* entity)
{
    m_pTargetEntity = entity;
}

void CCameraController::UpdateCamera(float frameTime)
{
    if(!m_pTargetEntity) return;  

    x -= m_deltaRotation.x * xSpeed * radius * frameTime;
    y -= m_deltaRotation.y * ySpeed * frameTime;
  
    y = ClampAngle(y, yMinLimit, yMaxLimit);
      
    Quat rotation = Quat::CreateRotationXYZ(Ang3(DEG2RAD(y),0, DEG2RAD(x)));

    Vec3 negDistance = Vec3(0.0f, -radius, 0);
    Vec3 position = rotation * negDistance + (m_pTargetEntity->GetWorldPos()+ Vec3(0, 0, heightOffset));  

    CryTransform::CTransform tr = CryTransform::CTransform(position, CryTransform::CRotation(rotation), Vec3(1, 1, 1));
    m_pCameraComponent->SetTransformMatrix(tr.ToMatrix34());
}

float CCameraController::ClampAngle(float angle, float min, float max)
{
    if (angle < -360)
        angle += 360;
    if (angle > 360)
        angle -= 360;
    return CLAMP(angle, min, max);
}

