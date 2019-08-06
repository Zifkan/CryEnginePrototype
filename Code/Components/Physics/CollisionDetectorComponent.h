#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "RxCpp/rx.hpp"

class CCollisionDetectorComponent :public IEntityComponent
{
public:
    static void ReflectType(Schematyc::CTypeDesc<CCollisionDetectorComponent>& desc)
    {
        desc.SetGUID("{FAC541A7-3A3C-4274-8CAE-0DA780CFC90B}"_cry_guid);
        desc.SetEditorCategory("Physics");
        desc.SetLabel("Collision Detector Component");

        desc.SetComponentFlags({ EFlags::Singleton , EEntityComponentFlags::Attach , EEntityComponentFlags::Socket });
    }


    Cry::Entity::EntityEventMask GetEventMask() const override;
    void ProcessEvent(const SEntityEvent& event) override;

protected:
    virtual void Initialize();
public:
    rxcpp::subjects::subject<IEntity*> OnCollisionEnter;
    rxcpp::subjects::subject<IEntity*> OnCollisionStay;
    rxcpp::subjects::subject<IEntity*> OnCollisionExit;
    uint32 lastFrame;


private:
    std::vector<IEntity*> m_newHitEntities;
    std::vector<IEntity*> m_lastHitEntities;

   
    std::_Vector_iterator<std::_Vector_val<std::_Simple_types<IEntity*>>> it;
};
