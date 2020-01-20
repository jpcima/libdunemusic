cmake_minimum_required(VERSION 3.3)

project(libdunemusic C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(DUNEMUSIC_BUILD_FORCE_STATIC "Pass flag -static to linker" OFF)

option(BUILD_PLAYER "Build the player" OFF)
option(BUILD_FILE_SYNTH "Build the sound file sythesizer (needs libsndfile)" OFF)
option(BUILD_SONG_LENGTH "Build the song length analyzer" OFF)
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
add_library(fakeSDLmixer STATIC
  "sources/fakeSDLmixer/fakeSDLmixer.cpp")
target_include_directories(fakeSDLmixer
  PUBLIC "sources/fakeSDLmixer")

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
  "sources/dunelegacy/misc/FileStream.cpp"
  "sources/dunelegacy/misc/fnkdat.cpp"
  "sources/dunelegacy/misc/format.cpp"
  "sources/dunelegacy/misc/md5.cpp"
  "sources/dunelegacy/misc/string_util.cpp"
  "sources/dunelegacy/mmath.cpp"
  )
target_include_directories(dunemusic
  PRIVATE "sources/dunelegacy")
target_link_libraries(dunemusic
  PRIVATE fakeSDLmixer)
target_compile_definitions(dunemusic
  PRIVATE "DUNEMUSIC_BUILD=1")

if(BUILD_PLAYER)
  find_package(SDL2 REQUIRED)
  add_executable(duneplay
    "sources/play.cpp")
  target_link_libraries(duneplay
    PRIVATE dunemusic)
  if(TARGET SDL2::SDL2)
    target_link_libraries(duneplay PRIVATE SDL2::SDL2)
  else()
    target_include_directories(duneplay PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(duneplay PRIVATE ${SDL2_LIBRARIES})
  endif()
endif()

if(BUILD_FILE_SYNTH)
  add_executable(dunesynth
    "sources/synth.cpp")
  target_link_libraries(dunesynth
    PRIVATE dunemusic)
  find_library(SNDFILE_LIBRARY "sndfile")
  target_link_libraries(dunesynth PRIVATE "${SNDFILE_LIBRARY}")
endif()

if(BUILD_SONG_LENGTH)
  add_executable(songlengths
    "sources/songlengths.cpp")
  target_link_libraries(songlengths
    PRIVATE dunemusic)
  target_include_directories(songlengths
    PRIVATE "sources/dunelegacy")
  target_link_libraries(songlengths
    PRIVATE fakeSDLmixer)
endif()

if(USE_STDCALL)
  target_compile_definitions(dunemusic PUBLIC "DUNEMUSIC_STDCALL=1")
else()
  target_compile_definitions(dunemusic PUBLIC "DUNEMUSIC_CDECL=1")
endif()

if(DUNEMUSIC_BUILD_FORCE_STATIC)
  target_link_options(dunemusic PRIVATE "-static")
endif()

###
message(STATUS "Build the player: ${BUILD_PLAYER}")
message(STATUS "Use stdcall: ${USE_STDCALL}")
