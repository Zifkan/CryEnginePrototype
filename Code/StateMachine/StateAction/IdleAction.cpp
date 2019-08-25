#include "StdAfx.h"
#include "IdleAction.h"


IdleAction::IdleAction(IEntity* characterEntity, 
    IEntity* mainCamera, 
    ICharacterActions* characterAction, 
    int priority,
    FragmentID fragmentID, 
    const TagState& fragTags,
    uint32 flags,
    ActionScopes scopeMask,
    uint32 userToken)
: BaseAction(characterEntity,characterAction, priority, fragmentID, fragTags, flags, scopeMask,userToken)
, m_pMainCameraEntity(mainCamera)
{
    m_pMainCameraComponent = m_pMainCameraEntity? m_pMainCameraEntity->GetComponent<CCameraController>():nullptr;
    m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");
}

IAction::EStatus IdleAction::Update(float timePassed)
{
    if (!m_pMainCameraComponent) return m_eStatus;


    m_pAnimationComponent->SetTagWithId(m_rotateTagId, m_pMainCameraComponent->IsCameraFocus());

    if ( m_pMainCameraComponent->IsCameraFocus())
    {      

        if (timer <= 1)
        {
            Ang3 viewAngle = Ang3(Quat::CreateRotationV0V1(m_lastDirection, m_pMainCameraEntity->GetForwardDir()));
            timer += timePassed;
           // auto angle = (viewAngles.z);
           // CryLog("x = %g y = %g z = %g", viewAngles.x, viewAngles.y,viewAngles.z);
            CryLog("Angle: %f", viewAngle.z);
         // CryLog("Angle: %f",  angle);
            m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, viewAngle.z);
        }
        
       

        const auto lookDir = GetLookDirNormalized(m_pCharacterEntity->GetPos(), m_pMainCameraEntity->GetPos());
        m_pCharacterEntity->SetRotation(Quat::CreateSlerp(m_pCharacterEntity->GetRotation(), Quat::CreateRotationVDir(Vec3(lookDir.x, lookDir.y, 0)), timer));
    }
    else
    {
        m_lastDirection = m_pCharacterEntity->GetForwardDir();
        timer = 0;

    }

    if (m_rootScope->IsDifferent(m_fragmentID, m_fragTags))
        SetFragment(m_fragmentID, m_fragTags);

    return m_eStatus;;
}


Vec3 IdleAction::GetLookDirNormalized(Vec3 target, Vec3 location)
{
    auto x = target.x - location.x;
    auto y = target.y - location.y;
    auto z = target.z - location.z;

    return Vec3(x, y, z).normalize();
}

void IdleAction::SetRotation()
{
   
}

