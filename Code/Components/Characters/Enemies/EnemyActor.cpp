﻿#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "EnemyActor.h"
#include "StateMachine/StateAction/DeathAction.h"
#include <StateMachine/StateAction/HitAction.h>
#include "StateMachine/StateAction/AttackAction.h"
#include "StateMachine/StateAction/IdleAction.h"
#include "StateMachine/StateAction/EnemyMovementAction.h"
#include "StateMachine/StateAction/EnemyAttackAction.h"
#include "StateMachine/StateAction/PushBackAction.h"
#include "Components/LifeResources/StaminaLifeResource.h"
#include "Components/LifeResources/HealthLifeResource.h"
#include "Components/Inputs/Actions/PlayerCharacterActions.h"
#include <CryCore/StaticInstanceList.h>


enum EnemyState
{};


static void RegisterEnemyActor(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(СEnemyActor));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterEnemyActor)


void СEnemyActor::Initialize()
{
    CCharacterComponent::Initialize();
    pPlayerEntity = gEnv->pEntitySystem->FindEntityByName("Player");

    m_pCharacterActions = new PlayerCharacterActions();
    m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();
    const float m_movementSpeed = 5.0f;
    IEntityNavigationComponent::SMovementProperties movementProperties;
    movementProperties.normalSpeed = m_movementSpeed;
    movementProperties.maxSpeed = m_movementSpeed;
    movementProperties.maxAcceleration = m_movementSpeed * 1.5f;
    m_pNavigationComponent->SetMovementProperties(movementProperties);
    m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");

    
    m_pNavigationComponent->SetStateUpdatedCallback([this](const Vec3& recommendedVelocity)
    {
        m_pCharacterActions->MovementSubject.get_subscriber().on_next(Vec2(recommendedVelocity));
    });

    m_pBehaviorTreeComponent = m_pEntity->GetOrCreateComponent<IEntityBehaviorTreeComponent>();
    if (IEntityBehaviorTreeComponent* pBehaviorTreeComponent = static_cast<IEntityBehaviorTreeComponent*>(m_pBehaviorTreeComponent))
    {
        const bool variableSetSuccessfully = pBehaviorTreeComponent->SetBBKeyValue("Health", 100.0f);
       
        CryLog("IS set: %i", variableSetSuccessfully);
    }
}


void СEnemyActor::CreateStateMachine()
{
    FragmentID m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
    FragmentID m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
    FragmentID m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    FragmentID m_hitReactionFragmentId = m_pAnimationComponent->GetFragmentId("HitReaction");
    FragmentID m_deathFragmentId = m_pAnimationComponent->GetFragmentId("Death");
    FragmentID m_pushBackFragmentId = m_pAnimationComponent->GetFragmentId("PushBack");


    m_stateMachine = new CStateMachine(m_pAnimationComponent);

    m_stateMachine->RegisterState(typeid(IdleAction), new IdleAction(m_pEntity,nullptr,m_pCharacterActions, 0, m_idleFragmentId));
    m_stateMachine->RegisterState(typeid(EnemyMovementAction), new EnemyMovementAction(m_pEntity, m_pCharacterActions, 1, m_walkFragmentId));
    m_stateMachine->RegisterState(typeid(EnemyAttackAction), new EnemyAttackAction(m_pWeaponSystem,m_pEntity, m_pCharacterActions, 2, m_attackFragmentId));
    m_stateMachine->RegisterState(typeid(HitAction), new HitAction(m_pEntity,m_pHitDamageComponent,m_pCharacterActions, 3, m_hitReactionFragmentId));
    m_stateMachine->RegisterState(typeid(PushBackAction), new PushBackAction(m_pEntity, m_pCharacterActions, 4, m_pushBackFragmentId));
    m_stateMachine->RegisterState(typeid(DeathAction), new DeathAction(m_pEntity, m_pWeaponSystem,m_pCharacterActions, 5, m_deathFragmentId));
}

void СEnemyActor::InitLifeResources()
{
    m_lifeResourceManager->RegisterResource(typeid(CHealthLifeResource), new CHealthLifeResource(100));
    m_lifeResourceManager->RegisterResource(typeid(CStaminaLifeResource), new CStaminaLifeResource(100,1));
}

void СEnemyActor::SetupActions()
{
    auto subscription = rxcpp::composite_subscription();
    m_pCharacterActions->MovementSubject.get_observable().subscribe(subscription, [this](Vec2 Vector2)
    {
        if (Vector2.GetLength2() > 0)
        {
            m_stateMachine->SetCurrentState(typeid(EnemyMovementAction));
        }
    });

    m_pCharacterActions->AttackSubject.get_observable().subscribe(subscription, [this](AttackType type)
    {
        m_stateMachine->SetCurrentState(typeid(EnemyAttackAction));
    });

    /* m_lifeResourceManager.GetResource<CHealthResource>()->Value.get_observable().skip_while([](float value) {return value <= 0; }).first().subscribe([subscription, this](float value)
     {
         m_stateMachine->SetCurrentState(typeid(DeathAction));
         subscription.unsubscribe();
     });*/

    SetSprint();
}

void СEnemyActor::SetSprint()
{
    m_pCharacterActions->MovementTypeSubject.get_subscriber().on_next(m_isSprint?SPRINT:WALK);
}


void СEnemyActor::StartGame()
{
    InitLifeResources();
    CreateStateMachine();

    InitWeaponSystem();
    SetupActions();
}

void СEnemyActor::GameUpdate(float fFrameTime)
{
    CCharacterComponent::GameUpdate(fFrameTime);

    m_currentState = Idle;

    float distance = m_pEntity->GetWorldPos().GetDistance(pPlayerEntity->GetWorldPos());

    if (distance <= m_detectDistance && distance>= m_attackDetectDistance)
    {
        m_currentState = Chase;
    }

    if (distance < m_attackDetectDistance)
    {
        m_currentState = Attack;
    }

    SetState();
}

void СEnemyActor::PropertyChanged()
{
   
}



void СEnemyActor::SetState()
{
    switch (m_currentState)
    {
    case Idle:
    {
        m_stateMachine->SetCurrentState(typeid(IdleAction));
    }
    break;
    case Chase:
    {
        m_pNavigationComponent->NavigateTo(pPlayerEntity->GetWorldPos());
    }
    break;
    case Attack:
    {
        m_pCharacterActions->AttackSubject.get_subscriber().on_next(ATTACK);
    }
    break;
    }
}