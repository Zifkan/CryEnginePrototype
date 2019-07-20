#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "EnemyActor.h"
#include "StateMachine/StateAction/DeathAction.h"
#include <StateMachine/StateAction/HitAction.h>
#include "LifeResources/StaminaLifeResource.h"
#include "StateMachine/StateAction/AttackAction.h"
#include "StateMachine/StateAction/IdleAction.h"
#include "LifeResources/HealthLifeResource.h"
#include "PlayerCharacterActions.h"
#include "StateMachine/StateAction/EnemyMovementAction.h"
#include "StateMachine/StateAction/EnemyAttackAction.h"

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

    
}


void СEnemyActor::CreateStateMachine()
{
    FragmentID m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
    FragmentID m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
    FragmentID m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    FragmentID m_hitReactionFragmentId = m_pAnimationComponent->GetFragmentId("HitReaction");
    FragmentID m_deathFragmentId = m_pAnimationComponent->GetFragmentId("Death");


    m_stateMachine = new CStateMachine(m_pAnimationComponent);

    m_stateMachine->RegisterState(typeid(IdleAction), new IdleAction(m_pCharacterActions, 0, m_idleFragmentId));
    m_stateMachine->RegisterState(typeid(EnemyMovementAction), new EnemyMovementAction(m_pEntity, m_pAnimationComponent, m_pCharacterController, m_pCharacterActions, 1, m_walkFragmentId));
    m_stateMachine->RegisterState(typeid(EnemyAttackAction), new EnemyAttackAction(m_pEntity, m_pAnimationComponent, m_pCharacterController, m_pCharacterActions, 2, m_attackFragmentId));
    m_stateMachine->RegisterState(typeid(HitAction), new HitAction(m_pCharacterActions, 3, m_hitReactionFragmentId));
    m_stateMachine->RegisterState(typeid(DeathAction), new DeathAction(m_pAnimationComponent,m_pCharacterActions, 4, m_deathFragmentId));
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
    m_pCharacterActions->MovementTypeSubject.get_subscriber().on_next(_isSprint?SPRINT:WALK);
}


void СEnemyActor::StartGame()
{
    InitLifeResources();
    CreateStateMachine();
    SetupActions();

    InitWeaponSystem();
}

void СEnemyActor::GameUpdate(float fFrameTime)
{
    CCharacterComponent::GameUpdate(fFrameTime);
    float distance = m_pEntity->GetWorldPos().GetDistance(pPlayerEntity->GetWorldPos());

    if (distance <= _detectDistance && distance>= _attackDetectDistance)
    {
        _currentState = Chase;
    }

    if (distance < _attackDetectDistance)
    {
        _currentState = Attack;
    }

    SetState();
}

void СEnemyActor::PropertyChanged()
{
   
}



void СEnemyActor::SetState()
{
    switch (_currentState)
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