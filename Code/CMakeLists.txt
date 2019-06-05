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
		"Components/CollisionControl.cpp"
		"Components/Player.cpp"
		"Components/SpawnPoint.cpp"		
		"Components/Bullet.h"
		"Components/Player.h"
		"Components/SpawnPoint.h"
		"Components/CollisionControl.h"
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

add_sources("StateActions_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "StateMachine\\\\StateAction"
		"StateMachine/StateAction/AttackAction.cpp"
		"StateMachine/StateAction/AttackAction.h"		
		"StateMachine/StateAction/BaseAction.cpp"
		"StateMachine/StateAction/BaseAction.h"	
		"StateMachine/StateAction/MovementAction.cpp"
		"StateMachine/StateAction/MovementAction.h"		
		"StateMachine/StateAction/IdleAction.h"		
		"StateMachine/StateAction/IdleAction.cpp"		
)

add_sources("StateMachine.cpp"
    PROJECTS Game
    SOURCE_GROUP "StateMachine"
		"StateMachine/StateMachine.cpp"
		"StateMachine/StateMachine.h"		
)
		
add_sources("Rx_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "RxCpp"
		
)

add_sources("PlayerInput_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"PlayerInput.cpp"
		"PlayerInput.h"
)

add_sources("CharacterActions_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"CharacterActions.h"
		"PlayerCharacterActions.cpp"
		"PlayerCharacterActions.h"
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