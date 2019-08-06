#pragma once
#include <CryEntitySystem/IEntity.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include "StateMachine/StateMachine.h"
#include "Components/LifeResources/LifeResourceManager.h"
#include "Components/Weapon/WeaponSystem.h"
#include "Components/Inputs/Actions/CharacterActions.h"
#include "Components/Damage/HitDamageComponent.h"

enum class CharecterStatusFlag
{
    Normal = 1 << 0,
    Hit = 1 << 1,
    AlreadyHitted = 1 << 2,
    Block = 1 << 3,
};

class CCharacterComponent  : public IEntityComponent
{  

public:
	CCharacterComponent();
	virtual ~CCharacterComponent() {}


    static void ReflectType(Schematyc::CTypeDesc<CCharacterComponent>& desc)
    {
        desc.SetGUID("{4C58FF1B-60CC-4E08-95DB-824FBE9AEF81}"_cry_guid);
    }

	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	void Revive();


    Cry::DefaultComponents::CAdvancedAnimationComponent* GetAnimationComponent()
    {
        return m_pAnimationComponent;
    }
    CStateMachine* m_stateMachine;

    void SetStatus(uint32 flag);
    void UnSetStatus(uint32 flag);
protected:

    ICharacterActions* m_pCharacterActions = nullptr;
        
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
    Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
    WeaponSystemComponent* m_pWeaponSystem = nullptr;
    CLifeResourceManagerComponent* m_lifeResourceManager = nullptr;
    CHitDamageComponent*  m_pHitDamageComponent = nullptr;

    Schematyc::CSharedString  m_characterEntityName;

    virtual void StartGame() = 0;
    virtual void CreateStateMachine() = 0;
    virtual void InitLifeResources() = 0;
    virtual void SetupActions() = 0;
    virtual void PropertyChanged() = 0;

    virtual void EditorUpdate(float fFrameTime);
    virtual void GameUpdate(float fFrameTime);

    void InitWeaponSystem();

    bool IsDead;

    uint32 m_currentStatus;
};
