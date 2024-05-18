cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_GENERATOR_PLATFORM x64)

project(EldenRingLoadoutMod
  VERSION   "0.0.0"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include(FetchContent)

FetchContent_Declare(minhook
  GIT_REPOSITORY        https://github.com/TsudaKageyu/minhook.git
  GIT_TAG               f5485b8454544c2f034c78f8f127c1d03dea3636)

FetchContent_Declare(pattern16
  GIT_REPOSITORY        https://github.com/Dasaav-dsv/Pattern16.git
  GIT_TAG               728eac543cee7e7bf6fda445d7db06e3dc8a61d0
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         "")

FetchContent_Declare(mini
  GIT_REPOSITORY        https://github.com/metayeti/mINI.git
  GIT_TAG               0.9.15
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         "")

set(SPDLOG_DISABLE_DEFAULT_LOGGER ON)
set(SPDLOG_USE_STD_FORMAT ON)
FetchContent_Declare(spdlog
  GIT_REPOSITORY        https://github.com/gabime/spdlog.git
  GIT_TAG               v1.13.0)

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY        https://github.com/nlohmann/json.git
  GIT_TAG               v3.11.3)

# Set iterator debug level to 0 for ELDEN RING ABI compatibility
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
FetchContent_Declare(liber
  GIT_REPOSITORY        https://github.com/Dasaav-dsv/libER.git
  GIT_TAG               8249033051ee1c1c7d4e7ad861cf98ac90951225)

FetchContent_MakeAvailable(minhook pattern16 mini spdlog nlohmann_json liber)

add_library(pattern16 INTERFACE)
target_include_directories(pattern16 INTERFACE ${pattern16_SOURCE_DIR}/include)

add_library(mini INTERFACE)
target_include_directories(mini INTERFACE ${mini_SOURCE_DIR}/src)

add_library(steamapi STATIC IMPORTED GLOBAL)
set_property(TARGET steamapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(steamapi PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG CXX
        IMPORTED_LOCATION_DEBUG steam_api64.lib)

add_library(EldenRingLoadoutMod SHARED
  src/dllmain.cpp
  src/erloadout_config.cpp
  src/erloadout_config.hpp
  src/erloadout_gear_info.hpp
  src/erloadout_loadout.cpp
  src/erloadout_loadout.hpp
  src/erloadout_logging.cpp
  src/erloadout_logging.hpp
  src/erloadout_messages.cpp
  src/erloadout_messages.hpp
  src/erloadout_messages_by_lang.cpp
  src/erloadout_save_file.cpp
  src/erloadout_save_file.hpp
  src/erloadout_shop.cpp
  src/erloadout_shop.hpp
  src/erloadout_stringify.cpp
  src/erloadout_stringify.hpp
  src/erloadout_talkscript.cpp
  src/erloadout_talkscript.hpp
  src/erloadout_talkscript_states.hpp
  src/erloadout_to_player.cpp
  src/erloadout_to_player.hpp
  src/from/EzState.hpp
  src/from/List.hpp
  src/from/MapItemManImpl.cpp
  src/from/MapItemManImpl.hpp
  src/from/WorldChrManImp.cpp
  src/from/WorldChrManImp.hpp
  src/utils/modutils.cpp
  src/utils/ModUtils.hpp
  src/utils/players.cpp
  src/utils/players.hpp)

set_target_properties(EldenRingLoadoutMod PROPERTIES OUTPUT_NAME "erloadout")

add_definitions(-DPROJECT_VERSION="${CMAKE_PROJECT_VERSION}")

add_custom_command(TARGET EldenRingLoadoutMod POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:EldenRingLoadoutMod> ${CMAKE_SOURCE_DIR}/erloadout.ini
  COMMAND_EXPAND_LISTS)

# Allow importing internal symbols using libER
target_include_directories(EldenRingLoadoutMod PUBLIC ${liber_SOURCE_DIR}/symbols)

target_link_libraries(EldenRingLoadoutMod
  minhook
  pattern16
  mini
  spdlog
  nlohmann_json
  libER_static
  ${CMAKE_SOURCE_DIR}/steam_api64.lib)
