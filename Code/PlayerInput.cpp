#include "PlayerInput.h"


void CPlayerInput::RegisterInputs()
{   
    // Register an action, and the callback that will be sent when it's triggered
    RegisterAction("player", "moveleft", [this](int activationMode, float value) {  });

    // Bind the 'A' key the "moveleft" action
    BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);

    RegisterAction("player", "moveright", [this](int activationMode, float value) { });
    BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);

    RegisterAction("player", "moveforward", [this](int activationMode, float value) {  });
    BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

    RegisterAction("player", "moveback", [this](int activationMode, float value) {  });
    BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

    RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) { });
    BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

    RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { });
    BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

       CInputComponent::RegisterAction("player", "sprint", [this](int activationMode, float value)
   {
        if (activationMode == eIS_Pressed)
        {

           
        }

        if (activationMode == eIS_Released)
        {
           
        }
    });
    
    BindAction("player", "sprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);

    // Register the shoot action
    
    RegisterAction("player", "Attack", [this](int activationMode, float value)
    {
        // Only fire on press, not release
        if (activationMode == eIS_Pressed)
        {
          

        }
    });

    // Bind the shoot action to left mouse click
    BindAction("player", "Attack", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

   
} 
