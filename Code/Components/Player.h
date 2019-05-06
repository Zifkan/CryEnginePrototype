#pragma once

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntity.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include "PlayerCharacterActions.h"
#include "PlayerInput.h"

//#include "PlayerInput.h"


////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{
	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	typedef uint8 TInputFlags;

	enum class EInputFlag
		: TInputFlags
	{
		MoveLeft = 1 << 0,
		MoveRight = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack = 1 << 3
	};

	template<typename T, size_t SAMPLES_COUNT>
	class MovingAverage
	{
		static_assert(SAMPLES_COUNT > 0, "SAMPLES_COUNT shall be larger than zero!");

	public:

		MovingAverage()
			: m_values()
			, m_cursor(SAMPLES_COUNT)
			, m_accumulator()
		{
		}

		MovingAverage& Push(const T& value)
		{
			if (m_cursor == SAMPLES_COUNT)
			{
				m_values.fill(value);
				m_cursor = 0;
				m_accumulator = std::accumulate(m_values.begin(), m_values.end(), T(0));
			}
			else
			{
				m_accumulator -= m_values[m_cursor];
				m_values[m_cursor] = value;
				m_accumulator += m_values[m_cursor];
				m_cursor = (m_cursor + 1) % SAMPLES_COUNT;
			}

			return *this;
		}

		T Get() const
		{
			return m_accumulator / T(SAMPLES_COUNT);
		}

		void Reset()
		{
			m_cursor = SAMPLES_COUNT;
		}

	private:

		std::array<T, SAMPLES_COUNT> m_values;
		size_t m_cursor;

		T m_accumulator;
	};

   

public:
	CPlayerComponent() = default;
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


     //   const char* name = "PlayerCharacter";
   
       // desc.AddMember(&CPlayerComponent::CharacterIntityName, 'floa', "PlayerIntityName", "PlayerIntityName", "Set Entity Name", name);
	}

	void Revive();
    
    void InitInput(ICharacterActions* playerCharacterActions);
    
  
protected:
	void UpdateMovementRequest(float frameTime);
	void UpdateLookDirectionRequest(float frameTime);
	void UpdateAnimation(float frameTime);
	void UpdateCamera(float frameTime);

	void SpawnAtSpawnPoint();

	void CreateWeapon(const char *name);

	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);

    void SetupActions();

protected:
    ICharacterActions* m_pCharacterActions = nullptr;

    Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
    CPlayerInputComponent* m_pPlayerInput = nullptr;

  
    char*  CharacterIntityName;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	TInputFlags m_inputFlags;
	Vec2 m_mouseDeltaRotation;
    Vec2 m_moveDirection = ZERO;
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;

	FragmentID m_activeFragmentId;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;

    bool IsAnimationPlaying(FragmentID fragmentId, int animLayer);

    bool m_isSprint;
};
