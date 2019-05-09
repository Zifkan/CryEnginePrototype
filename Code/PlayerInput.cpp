﻿#include "PlayerInput.h"


void CPlayerInputComponent::RegisterInputs(ICharacterActions* charActions)
{
    characterActions = charActions;

    // Register an action, and the callback that will be sent when it's triggered
    RegisterAction("player", "moveleft", [this](int activationMode, float value){characterActions->SetMovementSide( -value); });
    
    BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A); 
    BindAction("player", "moveleft", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);

    RegisterAction("player", "moveright", [this](int activationMode, float value){characterActions->SetMovementSide(value); });
    BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);
    BindAction("player", "moveright", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);

    RegisterAction("player", "moveforward", [this](int activationMode, float value) { characterActions->SetMovementForward(value); });
    BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

    RegisterAction("player", "moveforward_XboxPad", [this](int activationMode, float value) { characterActions->SetMovementForward(-value); });
    BindAction("player", "moveforward_XboxPad", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);

    RegisterAction("player", "moveback", [this](int activationMode, float value) { characterActions->SetMovementForward(-value);  });
    BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

    RegisterAction("player", "moveback_XboxPad", [this](int activationMode, float value) { characterActions->SetMovementForward(value);  });
    BindAction("player", "moveback_XboxPad", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);

    RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) {characterActions->SetRotateYaw(value); });
    BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

    RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { characterActions->SetRotatePitch(value); });
    BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

    RegisterAction("player", "sprint", [this](int activationMode, float value)
    {     
        if (activationMode == eIS_Pressed)
        {
            characterActions->SetMoveType(DODGE);
            //CryLog("Type of Movement = Dodge");
        }
          
            
       
        if (activationMode == eIS_Released)
        {
            characterActions->SetMoveType(WALK);
        }
     
        if (activationMode == eIS_Down)
        {
            characterActions->SetMoveType(SPRINT);
            //CryLog("Type of Movement = SPRINT");
        }
        
        
    });
    
    BindAction("player", "sprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);

    // Register the shoot action
    
    RegisterAction("player", "Attack", [this](int activationMode, float value)
    {
        // Only fire on press, not release
        if (activationMode == eIS_Pressed)
        {
            characterActions->SetAttack(true);
        }
    });

    // Bind the shoot action to left mouse click
    BindAction("player", "Attack", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

   
}


