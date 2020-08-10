#pragma once

#include <DefaultComponents/Input/InputComponent.h>


#include "Core/EntityManager.h"
#include "ECS/Components/InputComponent.h"



class CPlayerInputComponent : public IEntityComponent
{
public:
    ~CPlayerInputComponent() {};


    void RegisterInputs();

    static void ReflectType(Schematyc::CTypeDesc<CPlayerInputComponent>& desc)
    {
        desc.SetGUID("{41C00993-F62F-4949-98F7-D98CB3CF2979}"_cry_guid);
        desc.SetEditorCategory("Input");
        desc.SetLabel("Player Input");
        desc.SetDescription("Exposes support for inputs and action maps");
        desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Attach });
    }
    

    Cry::Entity::EventFlags GetEventMask() const override;

    virtual void ProcessEvent(const SEntityEvent& event) override;

protected:
    void Initialize() override;
  
    Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

    InputComponent  inputComponent;
    CEntityManager* entityManager = nullptr;

    flecs::entity inputEntity;

private:
    void SetInput() const;
};
