#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "RxCpp/rx.hpp"

class CLifeResourcesManager : public IEntityComponent
{
public:

    virtual uint64 GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& event) override;
    // ~IEntityComponent

    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<CLifeResourcesManager>& desc)
    {
        desc.SetGUID("{F59CDC61-9890-4F02-BF12-F69E618D24BB}"_cry_guid);

        desc.AddMember(&CLifeResourcesManager::m_maxHealthValue, 'valu', "MaxHealthValue", "Max Health Value", "Max Health Value", 100);
    }



    rxcpp::subjects::subject<float> HealthValue;

private:

    float m_maxHealthValue;
    float m_maxStaminaValue;
};
