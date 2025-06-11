add_library(lib_core
  myMain.cpp myMain.h
    "Game.cpp" "Game.h" "Role.cpp" "Role.h" "Global.h" "Map.h" "Map.cpp" "mapManager.cpp" "mapManager.h")

target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC pugixml sfml-graphics sfml-audio ImGui-SFML::ImGui-SFML)
