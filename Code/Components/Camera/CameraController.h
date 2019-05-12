#pragma once

#include <CryEntitySystem/IEntitySystem.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include "CharacterActions.h"

class CCameraController final : public IEntityComponent
{
public:
    
    ~CCameraController(){}

    // IEntityComponent
    virtual void Initialize() override;

    virtual uint64 GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& event) override;
   
    // ~IEntityComponent

    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<CCameraController>& desc)
    {
        desc.SetGUID("{30339CE3-9E69-450B-AE4E-E2D39E5C7EF0}"_cry_guid);
        desc.SetEditorCategory("Cameras");
        desc.SetLabel("Camera Controller");

        
        desc.AddMember(&CCameraController::x,'xdel',"xDelta", "xDelta","Mouse X delta rotation",0.0);
        desc.AddMember(&CCameraController::y,'ydel',"yDelta", "yDelta","Mouse Y delta rotation",0.0);
        desc.AddMember(&CCameraController::radius, 'radi', "Radius", "Radius", "Radius from target Entity", 2.0f);
        desc.AddMember(&CCameraController::xSpeed, 'xSpd', "x Speed", "x Speed", "Rotation camera speed by X", 80.0f);
        desc.AddMember(&CCameraController::ySpeed, 'ySpd', "y Speed", "y Speed", "Rotation camera speed by Y", 80.0f);
        desc.AddMember(&CCameraController::yMinLimit,'ymin', "y min", "y min", "Min Y rotation border", -80.0f);
        desc.AddMember(&CCameraController::yMaxLimit,'ymax', "y max", "y max", "Max Y rotation border", 90.0f);
        desc.AddMember(&CCameraController::heightOffset,'heig', "height", "height", "Camera height offset", 2.0f);
    }

    void InitInput(ICharacterActions* charActions);

    void SetTargetEntity(IEntity* entity);


protected:
    Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
    ICharacterActions* m_pCharacterActions = nullptr;

    void UpdateCamera(float frameTime);



    float ClampAngle(float angle, float min, float max);

    IEntity* m_pTargetEntity = nullptr;

    Vec2 m_deltaRotation = ZERO;

    float heightOffset = 2.0f;

    float x = 0.0f;
    float y = 0.0f;
    float radius = 2.0f;

    float xSpeed = 80.0f;
    float ySpeed = 80.0f;
    float yMinLimit = -90.0f;
    float yMaxLimit = 90.0f;
};