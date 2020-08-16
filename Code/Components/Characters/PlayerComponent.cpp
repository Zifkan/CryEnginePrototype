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
#include <CryCore/StaticInstanceList.h>


#include "Components/TransformComponents.h"
#include "Core/CryEcsWorld.h"
#include "ECS/Components/CharacterComponents.h"
#include "ECS/Components/MovementComponents.h"


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




void CPlayerComponent::CreateStateMachine()
{
    FragmentID m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
  //  FragmentID m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
 //   FragmentID m_attackFragmentId = m_pAnimationComponent->GetFragmentId("Attack");
    FragmentID m_hitReactionFragmentId = m_pAnimationComponent->GetFragmentId("HitReaction");
    FragmentID m_deathFragmentId = m_pAnimationComponent->GetFragmentId("Death");
    FragmentID m_blocFragmentId = m_pAnimationComponent->GetFragmentId("Block");
    FragmentID m_pushBackFragmentId = m_pAnimationComponent->GetFragmentId("PushBack");
    m_stateMachine = new CStateMachine(m_pAnimationComponent);

    m_stateMachine->RegisterState(typeid(IdleAction), new IdleAction(m_pEntity,m_pMainCamera , 0, m_idleFragmentId));
   // m_stateMachine->RegisterState(typeid(MovementAction), new MovementAction(m_lifeResourceManager->GetResource<CStaminaLifeResource>(), m_pEntity,   m_pMainCamera,  1, m_walkFragmentId));
  //  m_stateMachine->RegisterState(typeid(AttackAction), new AttackAction(m_pEntity, m_pWeaponSystem,m_lifeResourceManager->GetResource<CStaminaLifeResource>(),  2, m_attackFragmentId));
    m_stateMachine->RegisterState(typeid(HitAction), new HitAction(m_pEntity,m_pHitDamageComponent, 3, m_hitReactionFragmentId));
    m_stateMachine->RegisterState(typeid(PushBackAction), new PushBackAction(m_pEntity,  4, m_pushBackFragmentId));
    m_stateMachine->RegisterState(typeid(DeathAction), new DeathAction(m_pEntity, m_pWeaponSystem, 5, m_deathFragmentId));
    m_stateMachine->RegisterState(typeid(BlockAction), new BlockAction(m_pEntity,  1, m_blocFragmentId));
}

void CPlayerComponent::InitLifeResources()
{
  
}

void CPlayerComponent::HitReaction(float damage)
{
    m_stateMachine->SetCurrentState(typeid(HitAction));
}

void CPlayerComponent::StartGame()
{
    auto e = CryEcsWorld::instance()->EntityManager->CreateEntity(flecs::Singleton);

    CryEcsWorld::instance()->EntityManager->SetComponentData<PlayerTag>(e, { m_pEntity });
    CryEcsWorld::instance()->EntityManager->SetComponentData<Velocity>(e, { ZERO });
    CryEcsWorld::instance()->EntityManager->SetComponentData<CharacterComponent>(e, { m_pCharacterController });
    CryEcsWorld::instance()->EntityManager->SetComponentData<MovementSpeed>(e, { 20.f });
    CryEcsWorld::instance()->EntityManager->SetComponentData<MoveDirectionData>(e, { ZERO });
    CryEcsWorld::instance()->EntityManager->SetComponentData<ViewDirectionData>(e, {ZERO});
    CryEcsWorld::instance()->EntityManager->SetComponentData<Rotation>(e, { ZERO });
    

    m_pMainCamera = gEnv->pEntitySystem->FindEntityByName("GameCamera");
    CRY_ASSERT(m_pMainCamera != nullptr);

    Revive();

    InitLifeResources();

    CreateStateMachine();

    InitWeaponSystem();

    SetupActions();
}

void CPlayerComponent::GameUpdate(float fFrameTime)
{
    CCharacterComponent::GameUpdate(fFrameTime);

       
    m_stateMachine->SetCurrentState(typeid(IdleAction));
}

void CPlayerComponent::PropertyChanged()
{
}

void CPlayerComponent::SetupActions()
{
}
