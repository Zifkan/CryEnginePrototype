#include "StdAfx.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>

#include "PlayerInput.h"
#include <CryCore/StaticInstanceList.h>



#include "Core/CryEcsWorld.h"
#include "ECS/Components/InputComponent.h"
#include "ECS/Components/PlayerComponents.h"


static void RegisterCPlayerInputComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerInputComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCPlayerInputComponent)



Cry::Entity::EventFlags CPlayerInputComponent::GetEventMask() const
{
    return ENTITY_EVENT_START_GAME | ENTITY_EVENT_UPDATE | ENTITY_EVENT_EDITOR_PROPERTY_CHANGED;
}

void CPlayerInputComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case ENTITY_EVENT_START_GAME:
    {
        RegisterInputs();
    }
    break;
    }
}


void CPlayerInputComponent::RegisterInputs()
{
     entityManager = CryEcsWorld::instance()->EntityManager;
     inputEntity = entityManager->CreateEntity(flecs::Singleton);
    

    // Register an action, and the callback that will be sent when it's triggered
    m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value)
    {
        inputComponent.MovementAxis = Vec2(-value, 0);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);
    m_pInputComponent->BindAction("player", "moveleft", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);

    m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value)
    {
      
        inputComponent.MovementAxis = Vec2(value, 0);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);
    m_pInputComponent->BindAction("player", "moveright", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);
   
    
    m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value)
    {
        inputComponent.MovementAxis = Vec2( 0, value);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);
    m_pInputComponent->BindAction("player", "moveforward", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);




    m_pInputComponent->RegisterAction("player", "moveback", [this](int activationMode, float value)
    {
        inputComponent.MovementAxis = Vec2(0, -value);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);




    m_pInputComponent->RegisterAction("player", "moveback_XboxPad", [this](int activationMode, float value)
    {
        inputComponent.MovementAxis = Vec2(0, -value);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "moveback_XboxPad", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);
   

    m_pInputComponent->RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value)
    {
        inputComponent.AimAxis = Vec2(value,0);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);
    m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_XboxPad, EKeyId::eKI_XI_ThumbRX);
   


    m_pInputComponent->RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value)
    {
        inputComponent.AimAxis = Vec2( 0,value);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);
    m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_XboxPad, EKeyId::eKI_XI_ThumbRY);
    

    m_pInputComponent->RegisterAction("player", "sprint", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed || activationMode == eIS_Down)
        {

        }

        if (activationMode == eIS_Released)
        {

        }
        inputComponent.Sprint = static_cast<EInputState>(activationMode);
        SetInput();
    });
    m_pInputComponent->BindAction("player", "sprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);
    m_pInputComponent->BindAction("player", "sprint", eAID_XboxPad, EKeyId::eKI_XI_B);
   

    m_pInputComponent->RegisterAction("player", "Attack", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed || activationMode == eIS_Down)
        {

        }

        if (activationMode == eIS_Released)
        {

        }
        inputComponent.Combat.FirstAttack = static_cast<EInputState>(activationMode);
        SetInput();
      
    });   
    m_pInputComponent->BindAction("player", "Attack", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);
    m_pInputComponent->BindAction("player", "Attack", eAID_XboxPad, EKeyId::eKI_XI_TriggerRBtn);
  


    m_pInputComponent->BindAction("player", "Block", eAID_KeyboardMouse, EKeyId::eKI_Mouse2);
    m_pInputComponent->RegisterAction("player", "Block", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed)
        {

        }

        if (activationMode == eIS_Released)
        {

        }
        inputComponent.Combat.SecondAttack = static_cast<EInputState>(activationMode);
        SetInput();
    });


    m_pInputComponent->BindAction("player", "Focus", eAID_KeyboardMouse, EKeyId::eKI_F);
    m_pInputComponent->RegisterAction("player", "Focus", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed)
        {

        }
    });

    
}

void CPlayerInputComponent::Initialize()
{
    m_pInputComponent = m_pEntity->GetOrCreateComponent< Cry::DefaultComponents::CInputComponent>();

  
}

void CPlayerInputComponent::SetInput() const
{
    entityManager->SetComponentData<InputComponent>(inputEntity, { inputComponent });
}