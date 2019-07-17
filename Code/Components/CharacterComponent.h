#pragma once

#include <CryEntitySystem/IEntity.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include "PlayerInput.h"
#include "Weapon/WeaponSystem.h"
#include "LifeResources/LifeResourceManager.h"
#include "StateMachine/StateMachine.h"


class CCharacterComponent  : public IEntityComponent
{  

public:
	CCharacterComponent();
	virtual ~CCharacterComponent() {}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	

	void Revive();


    Cry::DefaultComponents::CAdvancedAnimationComponent* GetAnimationComponent()
    {
        return m_pAnimationComponent;
    }
  
protected:

    ICharacterActions* m_pCharacterActions = nullptr;
        
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
    CWeaponSystem* m_pWeaponSystem = nullptr;
    CLifeResourceManagerComponent* m_lifeResourceManager = nullptr;

    Schematyc::CSharedString  m_characterEntityName;
        
    CStateMachine* m_stateMachine;


    virtual void StartGame() = 0;
    virtual void CreateStateMachine() = 0;
    virtual void InitLifeResources() = 0;
    virtual void SetupActions() = 0;
    virtual void PropertyChanged() = 0;

    virtual void EditorUpdate(float fFrameTime);
    virtual void GameUpdate(float fFrameTime);

    void InitWeaponSystem();

    bool IsDead;
};
