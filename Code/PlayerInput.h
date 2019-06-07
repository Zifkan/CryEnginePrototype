#pragma once

#include <DefaultComponents/Input/InputComponent.h>
#include "CharacterActions.h"




class CPlayerInputComponent : public IEntityComponent
{
public:
   
    void RegisterInputs(ICharacterActions* charActions);

    static void ReflectType(Schematyc::CTypeDesc<CPlayerInputComponent>& desc)
    {
        desc.SetGUID("{41C00993-F62F-4949-98F7-D98CB3CF2979}"_cry_guid);
        desc.SetEditorCategory("Input");
        desc.SetLabel("Player Input");
        desc.SetDescription("Exposes support for inputs and action maps");
    }

protected:
    void Initialize() override;

    ICharacterActions* characterActions = nullptr;
    Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

};
