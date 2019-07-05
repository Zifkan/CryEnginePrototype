#pragma once
#include "CharacterComponent.h"
#include <CryAISystem/Components/IEntityNavigationComponent.h>

class СEnemyActor : public CCharacterComponent
{
public:
      

    static void ReflectType(Schematyc::CTypeDesc<СEnemyActor>& desc)
    {
        desc.SetGUID("{435A5847-CB46-445E-8508-90EFA8E82F71}"_cry_guid);
        desc.SetEditorCategory("Enemy");

        desc.AddMember(&СEnemyActor::_isSprint, 'sprt', "CharacterSprint", "Is Characetr Sprint", "Is Characetr Sprint", false);
        desc.AddMember(&СEnemyActor::_detectDistance, 'dist', "DetectionDist", "Enemy detection distance", "Enemy detection distance", 2);

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

private:
    bool _isSprint;
    float _detectDistance;
};
