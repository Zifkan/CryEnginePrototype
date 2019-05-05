#pragma once

#include <DefaultComponents/Input/InputComponent.h>
#include "CharacterActions.h"


class CPlayerInputComponent : public Cry::DefaultComponents::CInputComponent
{
public:
   
    void RegisterInputs(ICharacterActions* charActions);

    static void CPlayerInputComponent::ReflectType(Schematyc::CTypeDesc<CPlayerInputComponent>& desc)
    {
        desc.SetGUID("{183E3C84-2E08-4506-95FA-B362F7E735E9}"_cry_guid);
        desc.SetEditorCategory("Input");
        desc.SetLabel("Input");
        desc.SetDescription("Exposes support for inputs and action maps");
        //desc.SetIcon("icons:ObjectTypes/object.ico");
        desc.SetComponentFlags({IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach, IEntityComponent::EFlags::HideFromInspector  });
    }
protected:
    ICharacterActions* characterActions = nullptr;
};
