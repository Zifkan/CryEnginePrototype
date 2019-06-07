#pragma once

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntity.h>
#include <CryEntitySystem/IEntityComponent.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include "PlayerInput.h"
#include <DefaultComponents/Physics/AreaComponent.h>
#include <DefaultComponents/Physics/AreaComponent.h>
#include "Camera/CameraController.h"
#include "IBaseState.h"
#include "Camera/CameraController.h"
#include "StateMachine.h"


////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////

class CPlayerComponent final : public IEntityComponent
{  

public:
	CPlayerComponent();
	virtual ~CPlayerComponent() {}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
        
       // desc.AddMember(&CPlayerComponent::m_characterEntityName, 'name', "Player Entity Name", "Player Entity Name", "Set Entity Name", "Player");
	    desc.AddMember(&CPlayerComponent::m_sprintRatio, 'spri', "SprintRatio", "Sprint Ratio", "Sprint Ratio multiplier", 1.5f);
	    desc.AddMember(&CPlayerComponent::m_sprintAnimRatio, 'anim', "SprintAnimRatio", "Sprint Anim Ratio", "Sprint Ratio Anim multiplier", 4.0f);
	}

	void Revive();
    
    void InitInput(ICharacterActions* playerCharacterActions);
    
  
protected:
	

    void SetupActions();


    ICharacterActions* m_pCharacterActions = nullptr;
        
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
    CPlayerInputComponent* m_pPlayerInput = nullptr;
    IEntity* m_pMainCamera = nullptr;

    Schematyc::CSharedString  m_characterEntityName;
    float m_sprintRatio;
    float m_sprintAnimRatio;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
    FragmentID m_attackFragmentId;
       
	TagID m_rotateTagId;
    TagID m_forceAttackTagId;


    Vec2 m_moveDirection = ZERO;
    Vec2 m_lastRotationDirection = ZERO;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
  

        
    CStateMachine* m_stateMachine;


private:
    void CreateStateMachine();
};
