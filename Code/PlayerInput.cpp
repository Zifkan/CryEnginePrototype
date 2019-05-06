#include "PlayerInput.h"


void CPlayerInputComponent::RegisterInputs(ICharacterActions* charActions)
{
    characterActions = charActions;

    // Register an action, and the callback that will be sent when it's triggered
    RegisterAction("player", "moveleft", [this](int activationMode, float value) { characterActions->SetMovementSide(activationMode == eIS_Down ? -value:0.0f); });
    // Bind the 'A' key the "moveleft" action
    BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);

    RegisterAction("player", "moveright", [this](int activationMode, float value) {characterActions->SetMovementSide(activationMode == eIS_Down ? value : 0.0f); });
    BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);

    RegisterAction("player", "moveforward", [this](int activationMode, float value) { characterActions->SetMovementForward(activationMode == eIS_Down ? value : 0.0f); });
    BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

    RegisterAction("player", "moveback", [this](int activationMode, float value) { characterActions->SetMovementForward(activationMode == eIS_Down ? -value : 0.0f);  });
    BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

    RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) {characterActions->SetRotateYaw(value); });
    BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

    RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { characterActions->SetRotatePitch(value); });
    BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

    RegisterAction("player", "sprint", [this](int activationMode, float value)
    {
        if (activationMode == eIS_Pressed)
        {
            characterActions->SetSprint(true);
        }

        if (activationMode == eIS_Released)
        {
            characterActions->SetSprint(false);
        }
    });
    
    BindAction("player", "sprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);

    // Register the shoot action
    
    RegisterAction("player", "Attack", [this](int activationMode, float value)
    {
        // Only fire on press, not release
        if (activationMode == eIS_Pressed)
        {
            characterActions->Attack(true);
        }
    });

    // Bind the shoot action to left mouse click
    BindAction("player", "Attack", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

   
}


