#pragma once

#include <DefaultComponents/Input/InputComponent.h>
#include "CharacterActions.h"


class CPlayerInputComponent : public Cry::DefaultComponents::CInputComponent
{
public:
   
    void RegisterInputs(ICharacterActions* charActions);

protected:
    ICharacterActions* characterActions = nullptr;
};
