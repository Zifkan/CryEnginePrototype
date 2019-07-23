﻿#pragma once
#include "CharacterComponent.h"
#include "Components/Damage/HitDamageComponent.h"

class CPlayerComponent final :  public CCharacterComponent
{

public:
    void ProcessEvent(const SEntityEvent& event) override;

    void Initialize() override;

    static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
    {
        desc.SetGUID("{2BD3260B-366B-44EF-8638-1A3AA578E323}"_cry_guid);
        desc.SetEditorCategory("Player"); 
        desc.SetComponentFlags({ EFlags::Singleton });
        // desc.AddMember(&CPlayerComponent::m_characterEntityName, 'name', "PlayerEntityName", "Player Entity Name", "Set Entity Name", "Player");
      /*   desc.AddMember(&CPlayerComponent::m_sprintRatio, 'spri', "SprintRatio", "Sprint Ratio", "Sprint Ratio multiplier", 1.5f);
         desc.AddMember(&CPlayerComponent::m_sprintAnimRatio, 'anim', "SprintAnimRatio", "Sprint Anim Ratio", "Sprint Ratio Anim multiplier", 4.0f);*/
    }

    void InitInput(ICharacterActions* playerCharacterActions);
    void HitReaction(float damage);
protected:

    CPlayerInputComponent* m_pPlayerInput = nullptr;
    IEntity* m_pMainCamera = nullptr;
    CHitDamageComponent*  m_pHitDamageComponent = nullptr;

    void CreateStateMachine() override;
    void InitLifeResources() override;
    void SetupActions() override;
    void StartGame() override;

    
    virtual void GameUpdate(float fFrameTime);

    void PropertyChanged() override;
private:
    float m_attackStaminaCost = 15;
};