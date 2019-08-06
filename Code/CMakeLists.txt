cmake_minimum_required (VERSION 3.6.2)
set(CRYENGINE_DIR "F:/Crytek/CRYENGINE Launcher/Crytek/CRYENGINE_5.5")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"GamePlugin.h"
		"StdAfx.h"
)
add_sources("Components_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components"
		"Components/AIAction.cpp"
		"Components/AICharacterActions.cpp"
		"Components/AIAction.h"
		"Components/AICharacterActions.h"
)

add_sources("Damage_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Damage"
		"Components/Damage/HitDamageComponent.cpp"
		"Components/Damage/HitDamageComponent.h"
)

add_sources("Camera_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Camera"
		"Components/Camera/CameraController.cpp"
		"Components/Camera/CameraController.h"
)

add_sources("GamePlay_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\GamePlay"
		"Components/GamePlay/PlayerSoul.cpp"
		"Components/GamePlay/PlayerSoul.h"
)
add_sources("LifeResources_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\LifeResources"
		"Components/LifeResources/HealthLifeResource.cpp"
		"Components/LifeResources/LifeResourceManager.cpp"
		"Components/LifeResources/LifeResourcesManager.cpp"
		"Components/LifeResources/SoulEnergyLifeResource.cpp"
		"Components/LifeResources/StaminaLifeResource.cpp"
		"Components/LifeResources/BaseLifeResource.h"
		"Components/LifeResources/HealthLifeResource.h"
		"Components/LifeResources/LifeResourceManager.h"
		"Components/LifeResources/LifeResourcesManager.h"
		"Components/LifeResources/SoulEnergyLifeResource.h"
		"Components/LifeResources/StaminaLifeResource.h"
)
add_sources("Weapon_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Weapon"
		"Components/Weapon/MeleeWeapon.cpp"
		"Components/Weapon/ShieldWeapon.cpp"
		"Components/Weapon/BaseCustomWeapon.cpp"
		"Components/Weapon/WeaponSystem.cpp"
		"Components/Weapon/MeleeWeapon.h"
		"Components/Weapon/ShieldWeapon.h"
		"Components/Weapon/WeaponSystem.h"
		"Components/Weapon/WeaponExtensionData.h" 
		"Components/Weapon/BaseCustomWeapon.h"
)

add_sources("Characters_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Characters"
		"Components/Characters/CharacterComponent.cpp"
		"Components/Characters/PlayerComponent.cpp"
		"Components/Characters/CharacterComponent.h"
		"Components/Characters/PlayerComponent.h" 
)
 
add_sources("Enemies_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Characters\\\\Enemies"
		"Components/Characters/Enemies/EnemyActor.cpp"
		"Components/Characters/Enemies/EnemyActor.h"
)

add_sources("Inputs_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Inputs"
		"Components/Inputs/PlayerInput.cpp"
		"Components/Inputs/PlayerInput.h"
)

add_sources("InputActions_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Inputs\\\\Actions"
		"Components/Inputs/Actions/CharacterActions.h"
		"Components/Inputs/Actions/PlayerCharacterActions.h"
		"Components/Inputs/Actions/PlayerCharacterActions.cpp"
)


add_sources("StateMachine_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "StateMachine"
		"StateMachine/StateMachine.cpp"
		"StateMachine/StateMachine.h"
)

add_sources("Physics_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Physics"
		"Components/Physics/CollisionLayerStruct.h"
		"Components/Physics/CollisionLayerComponent.h"		
		"Components/Physics/CollisionDetectorComponent.h"
		"Components/Physics/CollisionLayerComponent.cpp"
		"Components/Physics/CollisionDetectorComponent.cpp"
)


add_sources("StateAction_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "StateMachine\\\\StateAction"
		"StateMachine/StateAction/AttackAction.cpp"
		"StateMachine/StateAction/BaseAction.cpp"
		"StateMachine/StateAction/BlockAction.cpp"
		"StateMachine/StateAction/DeathAction.cpp"
		"StateMachine/StateAction/EnemyAttackAction.cpp"
		"StateMachine/StateAction/EnemyMovementAction.cpp"
		"StateMachine/StateAction/HitAction.cpp"
		"StateMachine/StateAction/IdleAction.cpp"
		"StateMachine/StateAction/MovementAction.cpp"
		"StateMachine/StateAction/PushBackAction.cpp"
		"StateMachine/StateAction/AttackAction.h"
		"StateMachine/StateAction/BaseAction.h"
		"StateMachine/StateAction/BlockAction.h"
		"StateMachine/StateAction/DeathAction.h"
		"StateMachine/StateAction/EnemyAttackAction.h"
		"StateMachine/StateAction/EnemyMovementAction.h"
		"StateMachine/StateAction/HitAction.h"
		"StateMachine/StateAction/IdleAction.h"
		"StateMachine/StateAction/MovementAction.h"
		"StateMachine/StateAction/PushBackAction.h"
)

end_sources()

CryEngineModule(Game FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
		add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
		set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
		if (WIN32)
			set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
		endif()
	endif()
else()
	add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
	endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
    endif()

	add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
	endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}Game.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM