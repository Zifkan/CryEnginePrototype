#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "CharacterActions.h"

class AIActionComponent : public IEntityComponent , public ICharacterActions
{
public:
    void SetMovementForward(float y) override;
    void SetMovementSide(float x) override;
    void SetAttack(bool isAttack) override;
    void SetRotateYaw(float x) override;
    void SetRotatePitch(float y) override;
    void SetMoveSignal(bool signal) override;

    EntityEventMask GetEventMask() const override;

    static void ReflectType(Schematyc::CTypeDesc<AIActionComponent>& desc)
    {
        desc.SetGUID("{9044B6ED-6CF8-4F9D-9E00-CF56993BD175}"_cry_guid);
        desc.SetEditorCategory("AI Enemy");
    }



protected:
    void ProcessEvent(const SEntityEvent& event) override;

   
};
