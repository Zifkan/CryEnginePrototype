#include "CameraSystem.h"

#include "ECS/Components/PlayerComponents.h"

void CameraSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, CameraComponent& camera)
    {
        auto s = flecs::entity(*m_pWorld->DefaultWorld, flecs::Singleton);
        const PlayerTag* playerComponent = s.get<PlayerTag>();
        const InputComponent* input = s.get<InputComponent>();

        if(playerComponent==nullptr || camera.CameraEntity == nullptr || input == nullptr) return;

        const auto frameTime = e.delta_time();

        Vec2 deltaRotation = input->AimAxis;
        const auto heightOffset = camera.HeightOffset;
        const auto yMinLimit = camera.yMinLimit;
        const auto yMaxLimit = camera.yMaxLimit;
        const auto m_pitchLimit = camera.m_pitchLimit;
        const auto xSpeed = camera.xSpeed;
        const auto ySpeed = camera.ySpeed;
        const auto currentRadius = camera.currentRadius;

        float x = 0.0f;
        float y = 0.0f;

        IEntity* m_pTargetEntity  = nullptr;

        Quat rotation = ZERO;
        if (m_pTargetEntity)
        {
            Vec3 target = m_pTargetEntity->GetPos();

            Vec3 location = playerComponent->pCryEntity->GetPos() + Vec3(0, 0, heightOffset);

            x = target.x - location.x;
            y = target.y - location.y;

            Vec3 lookDir = Vec3(x, y, target.z - location.z).normalize();
            rotation = Quat::CreateRotationVDir(lookDir);

            auto angles = Ang3::GetAnglesXYZ(rotation);
            angles = Vec3(RAD2DEG(angles.x), RAD2DEG(angles.y), RAD2DEG(angles.z));

            if (angles.x <= m_pitchLimit)
                angles.x = m_pitchLimit;

            rotation = Quat::CreateRotationXYZ(Vec3(DEG2RAD(angles.x), DEG2RAD(angles.y), DEG2RAD(angles.z)));
        }
        else
        {
            x -= deltaRotation.x * xSpeed * currentRadius * frameTime;
            y -= deltaRotation.y * ySpeed * frameTime;

            y = ClampAngle(y, yMinLimit, yMaxLimit);
            auto angles = Ang3::GetAnglesXYZ(camera.CameraEntity->GetRotation());
            rotation = Quat::CreateRotationXYZ(Ang3(DEG2RAD(y), 0, DEG2RAD(x)));
        }       

        Vec3 negDistance = Vec3(0.0f, -currentRadius, 0);
        Vec3 position = rotation * negDistance + (playerComponent->pCryEntity->GetWorldPos() + Vec3(0, 0, heightOffset));

        CryTransform::CTransform tr = CryTransform::CTransform(position, CryTransform::CRotation(rotation), Vec3(1, 1, 1));
        camera.CameraEntity->SetWorldTM(tr.ToMatrix34());
    });
}

float CameraSystem::ClampAngle(float angle, float min, float max)
{
    if (angle < -360)
        angle += 360;
    if (angle > 360)
        angle -= 360;
    return CLAMP(angle, min, max);
}

