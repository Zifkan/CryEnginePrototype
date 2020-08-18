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
#include "Components/CryEntityComponent.h"
#include "Components/Camera/CameraController.h"
#include "Components/Characters/PlayerComponent.h"
#include "Core/SimpleModule.h"
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
        auto em = world->EntityManager;

       

        em->RegisterComponent<InputComponent>("InputComponent");
        em->RegisterComponent<CameraComponent>("CameraComponent");
        em->RegisterComponent<PlayerTag>("PlayerTag");
        em->RegisterComponent<MovementVelocity>("MovementVelocity");
        em->RegisterComponent<CharacterComponent>("CharacterComponent");
        em->RegisterComponent<MoveDirectionData>("MoveDirectionData");
        em->RegisterComponent<MovementSpeed>("MovementSpeed");


        em->RegisterComponent<WorldToLocal>("WorldToLocal");
        em->RegisterComponent<Translation>("Translation");
        em->RegisterComponent<CopyTransformToGameObject>("CopyTransformToGameObject");
        em->RegisterComponent<Rotation>("Rotation");
        em->RegisterComponent<CryEntityComponent>("CryEntityComponent");

     
        RegisterTransformSystems();
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

   

    systemsLauncher->RegisterSystem(new  CameraCollisionSystem());
    systemsLauncher->RegisterSystem(new  CameraSystem());
    systemsLauncher->RegisterSystem(new  PlayerViewDirectionSystem());
    systemsLauncher->RegisterSystem(new  InputMoveProcessingSystem());
    systemsLauncher->RegisterSystem(new  MovementVelocitySystem());
    systemsLauncher->RegisterSystem(new  CharacterRotationSystem());

    systemsLauncher->RegisterSystem(new  MovementCharacterSystem());

}

void CGamePlugin::RegisterTransformSystems()
{
    transformLauncher = new SystemLauncher(world);


     transformLauncher->RegisterSystem(new CopyTransformFromCryEntitySystem());
     transformLauncher->RegisterSystem(new TRSToLocalToWorldSystem());
     transformLauncher->RegisterSystem(new WorldToLocalSystem());
     transformLauncher->RegisterSystem(new CopyTransformToCryEntitySystem());
}


void CGamePlugin::MainUpdate(float frameTime)
{

    gEnv->pSystem->GetIPluginManager()->QueryPlugin<CryECSPlugin>()->UpdateTransformSystemGroup(frameTime);
    transformLauncher->Update(frameTime);
    systemsLauncher->Update(frameTime);
}


CRYREGISTER_SINGLETON_CLASS(CGamePlugin)