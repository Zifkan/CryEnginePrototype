#include "StdAfx.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>

#include "PlayerInput.h"
#include "Actions/PlayerCharacterActions.h"
#include <CryCore/StaticInstanceList.h>


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

void CPlayerInputComponent::RegisterInputs(ICharacterActions* charActions)
{
    characterActions = charActions;

    // Register an action, and the callback that will be sent when it's triggered
    m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value){characterActions->SetMovementSide( -value); });
    
    m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);
    m_pInputComponent->BindAction("player", "moveleft", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);

     m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value){characterActions->SetMovementSide(value); });
     m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);
     m_pInputComponent->BindAction("player", "moveright", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);

     m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) { characterActions->SetMovementForward(value); });
     m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

     m_pInputComponent->RegisterAction("player", "moveforward_XboxPad", [this](int activationMode, float value) { characterActions->SetMovementForward(-value); });
     m_pInputComponent->BindAction("player", "moveforward_XboxPad", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);

     m_pInputComponent->RegisterAction("player", "moveback", [this](int activationMode, float value) { characterActions->SetMovementForward(-value);  });
     m_pInputComponent->BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

     m_pInputComponent->RegisterAction("player", "moveback_XboxPad", [this](int activationMode, float value) { characterActions->SetMovementForward(value);  });
     m_pInputComponent->BindAction("player", "moveback_XboxPad", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);

     m_pInputComponent->RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) {characterActions->SetRotateYaw(value); });
     m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);
     m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_XboxPad, EKeyId::eKI_XI_ThumbRX);


     m_pInputComponent->RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { characterActions->SetRotatePitch(value); });
     m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);
     m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_XboxPad, EKeyId::eKI_XI_ThumbRY);

    m_pInputComponent->RegisterAction("player", "sprint", [this](int activationMode, float value)
    {             
        if (activationMode == eIS_Pressed || activationMode == eIS_Down)
        {
            characterActions->SetMoveSignal(true);
        }                      
       
        if (activationMode == eIS_Released)
        {
            characterActions->SetMoveSignal(false);
        }       
        
    });
    
    m_pInputComponent->BindAction("player", "sprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);
    m_pInputComponent->BindAction("player", "sprint", eAID_XboxPad, EKeyId::eKI_XI_B);
                     
    m_pInputComponent->RegisterAction("player", "Attack", [this](int activationMode, float value)
    {     
        if (activationMode == eIS_Pressed || activationMode == eIS_Down)
        {
            characterActions->SetAttack(true);
        }
      
        if (activationMode == eIS_Released)
        {
            characterActions->SetAttack(false);
        }
    });

    // Bind the shoot action to left mouse click
    m_pInputComponent->BindAction("player", "Attack", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);
    m_pInputComponent->BindAction("player", "Attack", eAID_XboxPad, EKeyId::eKI_XI_TriggerRBtn);


    m_pInputComponent->RegisterAction("player", "Block", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed)
        {
            characterActions->SetBlockSignal(true);
        }

        if (activationMode == eIS_Released)
        {
            characterActions->SetBlockSignal(false);
        }
    });

    m_pInputComponent->BindAction("player", "Block", eAID_KeyboardMouse, EKeyId::eKI_Mouse2);


   
    m_pInputComponent->RegisterAction("player", "Focus", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed)
        {
            characterActions->SetFocusOnSubject.get_subscriber().on_next(true);
        }
    });

    m_pInputComponent->BindAction("player", "Focus", eAID_KeyboardMouse, EKeyId::eKI_F);
}

void CPlayerInputComponent::Initialize()
{
    m_pInputComponent = m_pEntity->GetOrCreateComponent< Cry::DefaultComponents::CInputComponent>();
}


