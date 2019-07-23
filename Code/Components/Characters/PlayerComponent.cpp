#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include "PlayerComponent.h"
#include <StateMachine/StateAction/HitAction.h>
#include "StateMachine/StateAction/AttackAction.h"
#include "StateMachine/StateAction/MovementAction.h"
#include "StateMachine/StateAction/IdleAction.h"
#include "StateMachine/StateAction/DeathAction.h"
#include "StateMachine/StateAction/BlockAction.h"
#include "StateMachine/StateAction/PushBackAction.h"
#include "Components/LifeResources/StaminaLifeResource.h"


static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent)



void CPlayerComponent::Initialize()
{
    m_pEntity->SetName("Player");

    CCharacterComponent::Initialize();
    m_pPlayerInput = m_pEntity->GetOrCreateComponent<CPlayerInputComponent>();
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
    CCharacterComponent::ProcessEvent(event);      
}


void CPlayerComponent::InitInput(ICharacterActions* playerCharacterActions)
{
    m_pCharacterActions = playerCharacterActions;
    m_pPlayerInput->RegisterInputs(playerCharacterActions);
}

void CPlayerComponent::CreateStateMachine()
{
    FragmentID m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
    FragmentID m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
    FragmentID m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    FragmentID m_hitReactionFragmentId = m_pAnimationComponent->GetFragmentId("HitReaction");
    FragmentID m_deathFragmentId = m_pAnimationComponent->GetFragmentId("Death");
    FragmentID m_blocFragmentId = m_pAnimationComponent->GetFragmentId("Block");
    FragmentID m_pushBackFragmentId = m_pAnimationComponent->GetFragmentId("PushBack");
    m_stateMachine = new CStateMachine(m_pAnimationComponent);

    m_stateMachine->RegisterState(typeid(IdleAction), new IdleAction(m_pCharacterActions, 0, m_idleFragmentId));
    m_stateMachine->RegisterState(typeid(MovementAction), new MovementAction(m_lifeResourceManager->GetResource<CStaminaLifeResource>(), m_pEntity, m_pAnimationComponent, m_pCharacterController, m_pMainCamera, m_pCharacterActions, 1, m_walkFragmentId));
    m_stateMachine->RegisterState(typeid(AttackAction), new AttackAction(m_lifeResourceManager->GetResource<CStaminaLifeResource>(), m_pEntity, m_pAnimationComponent, m_pCharacterController, m_pCharacterActions, 2, m_attackFragmentId));
    m_stateMachine->RegisterState(typeid(HitAction), new HitAction(m_pCharacterActions, 3, m_hitReactionFragmentId));
    m_stateMachine->RegisterState(typeid(PushBackAction), new PushBackAction(m_pCharacterActions, 4, m_pushBackFragmentId));
    m_stateMachine->RegisterState(typeid(DeathAction), new DeathAction(m_pAnimationComponent,m_pCharacterActions, 5, m_deathFragmentId));
    m_stateMachine->RegisterState(typeid(BlockAction), new BlockAction(m_pCharacterActions, 1, m_blocFragmentId));
}

void CPlayerComponent::InitLifeResources()
{
    m_lifeResourceManager->RegisterResource(typeid(CHealthLifeResource), new CHealthLifeResource(100));
    m_lifeResourceManager->RegisterResource(typeid(CStaminaLifeResource), new CStaminaLifeResource(100, 1));
  //  m_lifeResourceManager.RegisterResource(typeid(CSoulEnergyLifeResource), new CSoulEnergyLifeResource(100, 1));
}

void CPlayerComponent::HitReaction(float damage)
{
    m_stateMachine->SetCurrentState(typeid(HitAction));
}

void CPlayerComponent::StartGame()
{
    m_pMainCamera = gEnv->pEntitySystem->FindEntityByName("GameCamera");
    CRY_ASSERT(m_pMainCamera != nullptr);
    m_pHitDamageComponent = m_pEntity->GetOrCreateComponent<CHitDamageComponent>();

    Revive();

    InitLifeResources();

    CreateStateMachine();

    InitWeaponSystem();

    SetupActions();
}

void CPlayerComponent::GameUpdate(float fFrameTime)
{
    CCharacterComponent::GameUpdate(fFrameTime);

    if (IsDead)
    {
   //     m_pAnimationComponent->SetTag("Dying", true);
    }
    else
    {
        m_stateMachine->SetCurrentState(typeid(IdleAction));
    }
}

void CPlayerComponent::PropertyChanged()
{
}


void CPlayerComponent::SetupActions()
{
    auto subscription = rxcpp::composite_subscription();
    m_pCharacterActions->MovementSubject.get_observable()
    .subscribe(subscription,[this](Vec2 Vector2)
    {
        if (Vector2.GetLength2() > 0)
        {
            m_stateMachine->SetCurrentState(typeid(MovementAction));
        }
    });

    m_pCharacterActions->AttackSubject.get_observable()
    .subscribe(subscription,[this](AttackType type)
    {
        IBaseLifeResource* staminaResource = m_lifeResourceManager->GetResource<CStaminaLifeResource>();
        if (staminaResource->Value.get_value() - m_attackStaminaCost >= 0)
        {
            m_stateMachine->SetCurrentState(typeid(AttackAction));
        }
    });

    m_pCharacterActions->BlockSubject.get_observable()
    .subscribe(subscription, [this](bool isBlock)
    {
        if(isBlock)
            m_stateMachine->SetCurrentState(typeid(BlockAction));
    });

    m_lifeResourceManager->GetResource<CHealthLifeResource>()->Value.get_observable().skip_while([](float value){ return value > 0; }).first()
    .subscribe([subscription, this](float value)
    {
      /*  IsDead = true;
        m_pAnimationComponent->SetTag("Dying", true);
        m_stateMachine->SetCurrentState(typeid(DeathAction));*/
       // subscription.unsubscribe();
    });

    m_pHitDamageComponent->HitSubject.get_observable().subscribe([this](SideHitEnum sideHit)
    {
       
        switch (sideHit)
        {
        case LeftSide:
        {
            m_pAnimationComponent->SetTag("LeftSide", true);
        }
        break;
        case RightSide:
        {           
            m_pAnimationComponent->SetTag("RightSide", true);
        }
        break;
        case PushBack:
        {
            m_stateMachine->SetCurrentState(typeid(PushBackAction));
        }
        break;
        }

        m_stateMachine->SetCurrentState(typeid(HitAction));
    });
}


