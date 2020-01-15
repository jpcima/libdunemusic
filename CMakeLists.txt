cmake_minimum_required(VERSION 3.3)

project(libdunemusic C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(DUNEMUSIC_SDL2_PATH "" CACHE STRING "Use SDL2 libraries from this directory")
option(DUNEMUSIC_BUILD_SDL2 "Build SDL2 libraries ourselves" OFF)

option(BUILD_PLAYER "Build the player" OFF)
option(USE_STDCALL "Build the Windows library using STDCALL convention" OFF)

set(CMAKE_CXX_STANDARD "11")
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN "hidden")

if(CMAKE_C_COMPILER_ID MATCHES "^GNU|Clang$")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "^GNU|Clang$")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

###
add_library(dunemusic_SDL2 INTERFACE)
if(DUNEMUSIC_BUILD_SDL2)
  set(BUILD_SDL2_SHARED OFF)
  set(BUILD_SDL2_STATIC ON)
  include(download_sdl2_hg)
  message(STATUS "Built SDL2 will be in ${SDL2_INSTALL_DIR}")
  target_include_directories(dunemusic_SDL2 INTERFACE "${SDL2_INSTALL_DIR}/include/SDL2")
  if(MSVC)
    target_link_libraries(dunemusic_SDL2 INTERFACE "${SDL2_INSTALL_DIR}/lib/SDL2.lib")
  else()
    target_link_libraries(dunemusic_SDL2 INTERFACE "${SDL2_INSTALL_DIR}/lib/libSDL2.a")
  endif()
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(dunemusic_SDL2 INTERFACE
      version opengl32 dbghelp advapi32 kernel32 winmm imm32 setupapi libucrt vcruntime)
  endif()
  add_dependencies(dunemusic_SDL2 SDL2HG)
elseif(DUNEMUSIC_SDL2_PATH)
  find_library(SDL2_LIBRARY "SDL2"
    PATHS "${DUNEMUSIC_SDL2_PATH}/lib")
  find_path(SDL2_INCLUDE_DIR "SDL.h"
    PATHS "${DUNEMUSIC_SDL2_PATH}/include"
    PATH_SUFFIXES "SDL2")
  if(NOT SDL2_LIBRARY)
    message(FATAL_ERROR "Cannot find SDL2 libraries")
  elseif(NOT SDL2_INCLUDE_DIR)
    message(FATAL_ERROR "Cannot find SDL2 headers")
  endif()
  message(STATUS "SDL library: ${SDL2_LIBRARY}")
  message(STATUS "SDL headers: ${SDL2_INCLUDE_DIR}")
  target_include_directories(dunemusic_SDL2 INTERFACE "${SDL2_INCLUDE_DIR}")
  target_link_libraries(dunemusic_SDL2 INTERFACE "${SDL2_LIBRARY}")
else()
  find_package(SDL2 REQUIRED)
  if(TARGET SDL2::SDL2)
    target_link_libraries(dunemusic_SDL2 INTERFACE SDL2::SDL2)
  else()
    target_include_directories(dunemusic_SDL2 INTERFACE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(dunemusic_SDL2 INTERFACE ${SDL2_LIBRARIES})
  endif()
endif()

# find_library(SDL2_MIXER_LIBRARY "SDL2_mixer")
# target_link_libraries(dunemusic_SDL2 INTERFACE "${SDL2_MIXER_LIBRARY}")

###
add_library(fakeSDLmixer STATIC
  "sources/fakeSDLmixer/fakeSDLmixer.cpp")
target_include_directories(fakeSDLmixer
  PUBLIC "sources/fakeSDLmixer")
target_link_libraries(fakeSDLmixer
  PUBLIC dunemusic_SDL2)

###
add_library(dunemusic SHARED
  "sources/dunemusic.cpp"
  "sources/dunelegacy/FileClasses/adl/sound_adlib.cpp"
  "sources/dunelegacy/FileClasses/adl/surroundopl.cpp"
  "sources/dunelegacy/FileClasses/adl/woodyopl.cpp"
  "sources/dunelegacy/FileClasses/adl/dbopl.cpp"
  "sources/dunelegacy/FileClasses/adl/nukedopl3.c"
  "sources/dunelegacy/FileClasses/adl/wemuopl.cpp"
  "sources/dunelegacy/FileClasses/adl/demuopl.cpp"
  "sources/dunelegacy/FileClasses/adl/nemuopl.cpp"
  "sources/dunelegacy/FileClasses/adl/opl.cpp"
  "sources/dunelegacy/FileClasses/music/ADLPlayer.cpp"
  "sources/dunelegacy/FileClasses/FileManager.cpp"
  "sources/dunelegacy/FileClasses/Pakfile.cpp"
  "sources/dunelegacy/misc/FileSystem.cpp"
  "sources/dunelegacy/misc/fnkdat.cpp"
  "sources/dunelegacy/misc/format.cpp"
  "sources/dunelegacy/misc/md5.cpp"
  "sources/dunelegacy/misc/string_util.cpp"
  "sources/dunelegacy/mmath.cpp"
  )
target_include_directories(dunemusic
  PRIVATE "sources/dunelegacy")
target_link_libraries(dunemusic
  PRIVATE dunemusic_SDL2
  PRIVATE fakeSDLmixer)
target_compile_definitions(dunemusic
  PRIVATE "DUNEMUSIC_BUILD=1")

if(BUILD_PLAYER)
  add_executable(duneplay
    "sources/play.cpp")
  target_link_libraries(duneplay
    PRIVATE dunemusic
    PRIVATE dunemusic_SDL2)
endif()


if(USE_STDCALL)
  target_compile_definitions(dunemusic PUBLIC "DUNEMUSIC_STDCALL=1")
else()
  target_compile_definitions(dunemusic PUBLIC "DUNEMUSIC_CDECL=1")
endif()

message(STATUS "Build the player: ${BUILD_PLAYER}")
message(STATUS "Use stdcall: ${USE_STDCALL}")
