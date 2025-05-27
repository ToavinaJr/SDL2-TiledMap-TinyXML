cmake_minimum_required(VERSION 3.10)

project(TMX_Viewer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Liste des fichiers source
set(SOURCE_FILES
    sources/main.cpp
    sources/TileLayer.cpp
    sources/TMXLoader.cpp
    sources/Renderer.cpp
    sources/Game.cpp
    sources/Entity.cpp
)


# Inclure les headers
include_directories(headers)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# TinyXML2
find_package(TinyXML2 REQUIRED)

# Ajout de l'executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Ajouter les ressources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_BINARY_DIR}/res
)

# Ajouter les include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    headers
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${TinyXML2_INCLUDE_DIRS}
)

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    SDL2_image
    tinyxml2
)

# Options de compilation
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic -g)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic -O2)
endif()

# Répertoire de sortie
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "${PROJECT_NAME}"
)
