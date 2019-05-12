#include "StdAfx.h"
#include "GamePlugin.h"

#include "Components/Player.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <IGameObjectSystem.h>
#include <IGameObject.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>
#include "Components/Camera/CameraController.h"

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
            InitPlayerInput();

            InitGameCamera();
        }
        break;

        case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
        {
            if (!gEnv->IsEditor()) return;
            InitPlayerInput();

            InitGameCamera();
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
	// Revive players when the network reports that the client is connected and ready for gameplay
	auto it = m_players.find(channelId);
	if (it != m_players.end())
	{
		if (IEntity* pPlayerEntity = gEnv->pEntitySystem->GetEntity(it->second))
		{
			if (CPlayerComponent* pPlayer = pPlayerEntity->GetComponent<CPlayerComponent>())
			{
				pPlayer->Revive();
			}
		}
	}

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

void CGamePlugin::InitPlayerInput()
{
    if (pPlayerEntity != nullptr) return;

    pPlayerEntity = gEnv->pEntitySystem->FindEntityByName("Player");

    CRY_ASSERT(pPlayerEntity != nullptr);

    if (pPlayerEntity != nullptr)
        pPlayerEntity->GetComponent<CPlayerComponent>()->InitInput(m_playerCharacterActions);
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
  
    
    cameraComponent->InitInput(m_playerCharacterActions);
    cameraComponent->SetTargetEntity(pPlayerEntity);
}

CRYREGISTER_SINGLETON_CLASS(CGamePlugin)
