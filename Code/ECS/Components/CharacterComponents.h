#pragma once
#include <CryEntitySystem/IEntity.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>

struct PlayerTag
{   
   IEntity* pCryEntity;   
};


struct CharacterComponent
{
    Cry::DefaultComponents::CCharacterControllerComponent* pCharacterController;
};