cmake_minimum_required(VERSION 3.3)

project(libdunemusic)

option(BUILD_PLAYER "Build the player" OFF)

set(CMAKE_CXX_STANDARD "14")
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN "hidden")

if(CMAKE_C_COMPILER_ID MATCHES "^GNU|Clang$")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "^GNU|Clang$")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

###
find_package(SDL2 REQUIRED)
add_library(dunemusic_SDL2 INTERFACE)
if(TARGET SDL2::SDL2)
  target_link_libraries(dunemusic_SDL2 INTERFACE SDL2::SDL2)
else()
  target_include_directories(dunemusic_SDL2 INTERFACE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(dunemusic_SDL2 INTERFACE ${SDL2_LIBRARIES})
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

if(BUILD_PLAYER)
  add_executable(duneplay
    "sources/play.cpp")
  target_link_libraries(duneplay
    PRIVATE dunemusic
    PRIVATE dunemusic_SDL2)
endif()
