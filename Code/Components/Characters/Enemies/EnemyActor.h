#pragma once
#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include "Components/Characters/CharacterComponent.h"
#include "Components/Damage/HitDamageComponent.h"


class СEnemyActor : public CCharacterComponent
{

    enum EnemyState
    {
        Idle,
        Chase,
        Attack,
    };
    

public:
      

    static void ReflectType(Schematyc::CTypeDesc<СEnemyActor>& desc)
    {
        desc.SetGUID("{435A5847-CB46-445E-8508-90EFA8E82F71}"_cry_guid);
        desc.SetEditorCategory("Enemy");
        desc.AddBase<CCharacterComponent>();
        desc.AddMember(&СEnemyActor::m_isSprint, 'sprt', "CharacterSprint", "Is Characetr Sprint", "Is Characetr Sprint", false);
        desc.AddMember(&СEnemyActor::m_detectDistance, 'dist', "DetectionDist", "Enemy detection distance", "Enemy detection distance", 2);
        desc.AddMember(&СEnemyActor::m_attackDetectDistance, 'adis', "AttackDetectionDist", "Enemy attack detection distance", "Enemy attack detection distance", 2);

    }

    virtual void Initialize();

protected:
    void CreateStateMachine() override;
    void InitLifeResources() override;
    void SetupActions() override;
    void SetSprint();

    virtual void StartGame();
    virtual void GameUpdate(float fFrameTime);
    void PropertyChanged() override;

    IEntityNavigationComponent* m_pNavigationComponent = nullptr;
    IEntity* pPlayerEntity;

    void SetState();

private:
    bool m_isSprint;
    float m_detectDistance;
    float m_attackDetectDistance;

    EnemyState m_currentState;
};
