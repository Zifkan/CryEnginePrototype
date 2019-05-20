#pragma once
#include <CryEntitySystem/IEntitySystem.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Lights/PointLightComponent.h>
#include "RxCpp/rx.hpp"


class CPlayerSoul : public IEntityComponent
{
public:
    ~CPlayerSoul(){}

    static void ReflectType(Schematyc::CTypeDesc<CPlayerSoul>& desc)
    {
        desc.SetGUID("{D4CC9077-1852-4467-B060-6C5A1EB7B671}"_cry_guid);
        desc.SetEditorCategory("Game Play");
        desc.SetLabel("Player Soul Component");
        
    }


    Cry::Entity::EntityEventMask GetEventMask() const override;

protected:
    void Initialize() override;
    void ProcessEvent(const SEntityEvent& event) override;

    Cry::DefaultComponents::CPointLightComponent* m_pPoinLight = nullptr;

    void LifeTimeReaction();

    float m_lifeTimeLimit = 0.0f;
    rxcpp::subjects::subject<float> m_lifeTimeCycle = rxcpp::subjects::subject<float>();
};
