#pragma once

#include <CryEntitySystem/IEntitySystem.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include "ECS/Converts/ConvertToEntity.h"

class CCameraController final : public CConvertToEntity
{
public:
    
    ~CCameraController(){}

    // IEntityComponent
    virtual void Initialize() override;
  
   
    // ~IEntityComponent

    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<CCameraController>& desc)
    {
        desc.SetGUID("{30339CE3-9E69-450B-AE4E-E2D39E5C7EF0}"_cry_guid);
        desc.SetEditorCategory("Cameras");
        desc.SetLabel("Camera Controller");

        
        desc.AddMember(&CCameraController::x,'xdel',"xDelta", "xDelta","Mouse X delta rotation",0.0);
        desc.AddMember(&CCameraController::y,'ydel',"yDelta", "yDelta","Mouse Y delta rotation",0.0);
        desc.AddMember(&CCameraController::radius, 'radi', "Radius", "Radius", "Radius from target Entity", 5.0f);
        desc.AddMember(&CCameraController::xSpeed, 'xSpd', "xSpeed", "x Speed", "Rotation camera speed by X", 80.0f);
        desc.AddMember(&CCameraController::ySpeed, 'ySpd', "ySpeed", "y Speed", "Rotation camera speed by Y", 80.0f);
        desc.AddMember(&CCameraController::yMinLimit,'ymin', "yMin", "y min", "Min Y rotation border", -80.0f);
        desc.AddMember(&CCameraController::yMaxLimit,'ymax', "yMax", "y max", "Max Y rotation border", 90.0f);
        desc.AddMember(&CCameraController::heightOffset,'heig', "Height", "Height", "Camera height offset", 3.0f);
        desc.AddMember(&CCameraController::focusHeightOffset, 'fhei', "FocusHeight", "Focus Height", "Focus Camera height offset", 3.0f);
        desc.AddMember(&CCameraController::m_pitchLimit, 'plim', "PitchLimit", "Focus Pitch Limit", "Focus Pitch Limit", -35.f);
       
        desc.SetComponentFlags({ EFlags::Singleton});
    }


    void Convert(flecs::entity entity, CEntityManager dstManager) override;

    virtual Cry::Entity::EventFlags GetEventMask() const;
    virtual void ProcessEvent(const SEntityEvent& event);

protected:  

    Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;   

    Vec2 m_deltaRotation = ZERO;

    float heightOffset = 3.0f;
    float focusHeightOffset = 3.0f;

    float x = 0.0f;
    float y = 0.0f;
    float radius = 5.0f;

    float currentRadius = 0.0f;

    float xSpeed = 80.0f;
    float ySpeed = 80.0f;
    float yMinLimit = -90.0f;
    float yMaxLimit = 90.0f;
    float m_pitchLimit = -35.f;
};
