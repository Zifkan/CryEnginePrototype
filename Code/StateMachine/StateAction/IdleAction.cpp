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
: BaseAction(characterAction, priority, fragmentID, fragTags, flags, scopeMask,userToken)
, m_pMainCameraEntity(mainCamera)
, m_pCharacterEntity(characterEntity)
{
    m_pMainCameraComponent = m_pMainCameraEntity? m_pMainCameraEntity->GetComponent<CCameraController>():nullptr;    
}

IAction::EStatus IdleAction::Update(float timePassed)
{
    if (!m_pMainCameraComponent) return m_eStatus;

    auto animComponent = m_pStateMachine->GetAnimationComponent();


    animComponent->SetTagWithId(m_rotateTagId, m_pMainCameraComponent->IsCameraFocus());

    if ( m_pMainCameraComponent->IsCameraFocus())
    {      

        if (timer <= 1)
        {
            auto dir = m_pCharacterEntity->GetPos() - m_pMainCameraEntity->GetPos();
            const float angle = acosf(dir.dot(m_pCharacterEntity->GetForwardDir()));

            Ang3 viewAngles = Ang3(Quat::CreateRotationV0V1(m_pCharacterEntity->GetPos(), m_pMainCameraEntity->GetPos()));
            timer += timePassed;
           // auto angle = (viewAngles.z);
           // CryLog("x = %g y = %g z = %g", viewAngles.x, viewAngles.y,viewAngles.z);
           // CryLog("Angle: %f", angle * 180 / g_PI);
            CryLog("Angle: %f",  angle);
            animComponent->SetMotionParameter(eMotionParamID_TurnAngle, angle);
        }
        
       

        const auto lookDir = GetLookDirNormalized(m_pCharacterEntity->GetPos(), m_pMainCameraEntity->GetPos());
        m_pCharacterEntity->SetRotation(Quat::CreateSlerp(m_pCharacterEntity->GetRotation(), Quat::CreateRotationVDir(Vec3(lookDir.x, lookDir.y, 0)), timer));
    }
    else
    {
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

void IdleAction::Enter()
{
    if (m_rotateTagId==TAG_ID_INVALID)
        m_rotateTagId = m_pStateMachine->GetAnimationComponent()->GetTagId("Rotate");
}

