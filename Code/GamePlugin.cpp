#include "StdAfx.h"
#include "GamePlugin.h"



#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <IGameObjectSystem.h>
#include <IGameObject.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

#include "CryECSPlugin.h"
#include "Components/Camera/CameraController.h"
#include "Components/Characters/PlayerComponent.h"
#include "ECS/Systems/Camera/CameraSystem.h"
#include "ECS/Systems/Input/InputMoveProcessingSystem.h"
#include "ECS/Systems/Movement/MovementCharacterSystem.h"
#include "ECS/Systems/Movement/MovementVelocitySystem.h"
#include "ECS/Systems/Movement/PlayerViewDirectionSystem.h"
#include "ECS/Systems/Movement/RotationSystem.h"

CGamePlugin::~CGamePlugin()
{
	// Remove any registered listeners before 'this' becomes invalid
	if (gEnv->pGameFramework != nullptr)
	{
		gEnv->pGameFramework->RemoveNetworkedClientListener(*this);
	}

	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CGamePlugin::GetCID());
	}
}

bool CGamePlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	// Register for engine system events, in our case we need ESYSTEM_EVENT_GAME_POST_INIT to load the map
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGamePlugin");
	
	return true;
}

void CGamePlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
    switch (event)
    {
        // Called when the game framework has initialized and we are ready for game logic to start
    case ESYSTEM_EVENT_GAME_POST_INIT:
    {
        // Listen for client connection events, in order to create the local player
        gEnv->pGameFramework->AddNetworkedClientListener(*this);

        // Don't need to load the map in editor
        if (!gEnv->IsEditor())
        {
            gEnv->pConsole->ExecuteString("map example", false, true);
        }

    }
    break;

    case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
    {
        // Register all components that belong to this plug-in
        auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
        {
            // Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
            Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
        };

        if (gEnv->pSchematyc)
        {
            gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
                stl::make_unique<Schematyc::CEnvPackage>(
                    CGamePlugin::GetCID(),
                    "EntityComponents",
                    "Crytek GmbH",
                    "Components",
                    staticAutoRegisterLambda
                    )
            );
        }
    }
    break;

    case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
    {
        if (gEnv->IsEditor()) return;


        EnableUpdate(IEnginePlugin::EUpdateStep::MainUpdate, true);
    }
    break;

    case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
    {
        if (!gEnv->IsEditor()) return;

        pEcsPlugin = gEnv->pSystem->GetIPluginManager()->QueryPlugin<CryECSPlugin>();

        pEcsPlugin->World = new CryEcsWorld();

        world = CryEcsWorld::instance();
        auto w = world->DefaultWorld;
        flecs::component<InputComponent>(*w, "InputComponent");
        flecs::component<CameraComponent>(*w, "CameraComponent");
        flecs::component<PlayerTag>(*w, "PlayerTag");

        flecs::component<Velocity>(*w, "Velocity");
        flecs::component<CharacterComponent>(*w, "CharacterComponent");
        flecs::component<MoveDirectionData>(*w, "MoveDirectionData");
        flecs::component<MovementSpeed>(*w, "MovementSpeed");
        flecs::component<Rotation>(*w, "Rotation");
        flecs::component<ViewDirectionData>(*w, "ViewDirectionData");

        

        RegisterSystem();
        EnableUpdate(IEnginePlugin::EUpdateStep::MainUpdate, true);

    }
    break;
    }
}

bool CGamePlugin::OnClientConnectionReceived(int channelId, bool bIsReset)
{    
	return true;
}

bool CGamePlugin::OnClientReadyForGameplay(int channelId, bool bIsReset)
{
	return true;
}

void CGamePlugin::OnClientDisconnected(int channelId, EDisconnectionCause cause, const char* description, bool bKeepClient)
{
	// Client disconnected, remove the entity and from map
	auto it = m_players.find(channelId);
	if (it != m_players.end())
	{
		gEnv->pEntitySystem->RemoveEntity(it->second);

		m_players.erase(it);
	}
}

void CGamePlugin::InitGameCamera()
{
    CCameraController* cameraComponent;
    if (IEntity* pCamEntity = gEnv->pEntitySystem->FindEntityByName("GameCamera"))
    {
        cameraComponent = pCamEntity->GetOrCreateComponent<CCameraController>();
    }
    else
    {
        SEntitySpawnParams spawnParameters;
        spawnParameters.sName = "GameCamera";
        pCamEntity = gEnv->pEntitySystem->SpawnEntity(spawnParameters);
        cameraComponent = pCamEntity->GetOrCreateComponent<CCameraController>();
    }
  
 
}

void CGamePlugin::RegisterSystem()
{
    systemsLauncher = new SystemLauncher(world);

    systemsLauncher->RegisterSystem(new InputMoveProcessingSystem());

    systemsLauncher->RegisterSystem(new  CameraCollisionSystem());
    systemsLauncher->RegisterSystem(new  CameraSystem());
    systemsLauncher->RegisterSystem(new  PlayerViewDirectionSystem());

    systemsLauncher->RegisterSystem(new  MovementVelocitySystem());
    systemsLauncher->RegisterSystem(new  RotationSystem());

    systemsLauncher->RegisterSystem(new  MovementCharacterSystem());
}

void CGamePlugin::MainUpdate(float frameTime)
{
    pEcsPlugin->UpdateTransformSystemGroup(frameTime);


    systemsLauncher->Update(frameTime);
   
}


CRYREGISTER_SINGLETON_CLASS(CGamePlugin)