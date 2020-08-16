#include "CameraSystem.h"

#include <CryPhysics/physinterface.h>


#include "ECS/Components/CharacterComponents.h"



void CameraSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, CameraComponent& camera)
    {
        auto s = flecs::entity(*m_pWorld->DefaultWorld, flecs::Singleton);
        const PlayerTag* playerComponent = s.get<PlayerTag>();
        const InputComponent* input = s.get<InputComponent>();

        if(playerComponent==nullptr || camera.CameraEntity == nullptr || input == nullptr) return;

        const auto frameTime = GetDeltaTime();

        Vec2 vec2 = Vec2(CLAMP(input->AimAxis.x, -1, 1), CLAMP(input->AimAxis.y, -1, 1)) ;


        if (crymath::abs(vec2.x) < 0.4f  &&  crymath::abs(vec2.y) > crymath::abs(vec2.x))
            vec2 = Vec2(0, vec2.y);
        if (crymath::abs(vec2.y) < 0.4f  &&  crymath::abs(vec2.x) > crymath::abs(vec2.y))
            vec2 = Vec2(vec2.x, 0);

        Vec2  deltaRotation = vec2;

        const auto heightOffset = camera.HeightOffset;
        const auto yMinLimit = camera.yMinLimit;
        const auto yMaxLimit = camera.yMaxLimit;
        const auto m_pitchLimit = camera.m_pitchLimit;
        const auto xSpeed = camera.xSpeed;
        const auto ySpeed = camera.ySpeed;
        const auto currentRadius = camera.currentRadius;

        IEntity* m_pTargetEntity  = nullptr;

        Quat rotation = ZERO;
        float x = camera.x;
        float y = camera.y;

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


        camera.x = x;
        camera.y = y;

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

void CameraCollisionSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, CameraComponent& camera)
    {
        auto s = flecs::entity(*m_pWorld->DefaultWorld, flecs::Singleton);
        const PlayerTag* playerComponent = s.get<PlayerTag>();
     
        if (playerComponent == nullptr || camera.CameraEntity == nullptr) return;

        const auto frameTime = GetDeltaTime();
        const auto heightOffset = camera.HeightOffset;

        static IPhysicalEntity* pSkipEnts[10];
        pSkipEnts[0] = playerComponent->pCryEntity->GetPhysics();
        auto org = playerComponent->pCryEntity->GetWorldPos() + Vec3(0, 0, heightOffset);
        ray_hit rayhit;
        // Perform the ray cast.
        int hits = gEnv->pPhysicalWorld->RayWorldIntersection(org, camera.CameraEntity->GetWorldPos() - org,
            ent_static | ent_sleeping_rigid | ent_rigid | ent_independent | ent_terrain, rwi_stop_at_pierceable | rwi_colltype_any,
            &rayhit, 1, pSkipEnts, 2);

        if (hits)
        {
            camera.currentRadius -= frameTime;
        }
        else
        {
            if (camera.currentRadius < camera.radius)
                camera.currentRadius += frameTime;
        }
    });
}

