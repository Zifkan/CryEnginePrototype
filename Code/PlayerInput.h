#pragma once

#include <DefaultComponents/Input/InputComponent.h>
#include "CharacterActions.h"




class CPlayerInputComponent : public Cry::DefaultComponents::CInputComponent
{
public:
   
    void RegisterInputs(ICharacterActions* charActions);

    static void ReflectType(Schematyc::CTypeDesc<CPlayerInputComponent>& desc)
    {
        desc.SetGUID("{183E3C84-2E08-4506-95FA-B362F7E735E9}"_cry_guid);
        desc.SetEditorCategory("Input");
        desc.SetLabel("Input");
        desc.SetDescription("Exposes support for inputs and action maps");
        //desc.SetIcon("icons:ObjectTypes/object.ico");
        desc.SetComponentFlags({EFlags::Socket, EFlags::Attach, EFlags::HideFromInspector  });
    }
protected:
    ICharacterActions* characterActions = nullptr;
};
