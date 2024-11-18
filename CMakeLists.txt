cmake_minimum_required(VERSION 3.5)
message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

set(wxWidgets_USE_UNICODE ON)
if( CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(wxWidgets_USE_STATIC ON)
    set(wxWidgets_CONFIGURATION mswu)
endif()

project(BatchCrop)
set(WX_WIN_PATH "${PROJECT_SOURCE_DIR}/dep/wx_win" CACHE PATH "Path to wxWidgets libraries and includes")
# Request the required wxWidgets libs
find_package(wxWidgets 3.2 COMPONENTS core base REQUIRED CONFIG PATHS ${WX_WIN_PATH})

# Include the wxWidgets use file to initialize various settings
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
