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


////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{
    enum MovementType
    {
        WALK,
        DODGE,
        SPRINT,
    };

    struct MovementSprintStruct
    {
        bool IsSignal;
        float Time;
        MovementType Type;

        MovementSprintStruct(bool isSignal, float time, MovementType type) : IsSignal(isSignal), Time(time), Type(type)
        {
        }
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
        
        desc.AddMember(&CPlayerComponent::m_characterEntityName, 'name', "Player Entity Name", "Player Entity Name", "Set Entity Name", "Player");
	    desc.AddMember(&CPlayerComponent::m_sprintRatio, 'spri', "Sprint Ratio", "Sprint Ratio", "Sprint Ratio multiplier", 1.5f);
	    desc.AddMember(&CPlayerComponent::m_sprintAnimRatio, 'anim', "Sprint Anim Ratio", "Sprint Anim Ratio", "Sprint Ratio Anim multiplier", 4.0f);
	}

	void Revive();
    
    void InitInput(ICharacterActions* playerCharacterActions);
    
  
protected:
	void UpdateMovementRequest(float frameTime);
	void UpdateAnimation(float frameTime);

    void SetupActions();

protected:
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
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;

	FragmentID m_activeFragmentId;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;

    bool IsAnimationPlaying(FragmentID fragmentId, int animLayer);
    bool IsAttack;
    IAction* pAction;
    MovementType m_movementType;
};
