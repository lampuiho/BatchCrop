cmake_minimum_required(VERSION 2.8...3.13)
set(PROJECT_NAME "BatchCrop")
project(${PROJECT_NAME})

set(WX_WIN_PATH "${PROJECT_SOURCE_DIR}/dep/wx_win" CACHE PATH "Path to wxWidgets libraries and includes")
include_directories(${WX_WIN_PATH}/lib/mswu)
include_directories(${WX_WIN_PATH}/include)
link_directories(${WX_WIN_PATH}/lib)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    __WXMSW__         # Defines for Windows-specific wxWidgets
    _UNICODE          # Enables Unicode support
    NDEBUG            # Disables debugging
    WXUSINGDLL        # Uses wxWidgets as DLL
)
set(WX_LIBRARIES 
    wxmsw32u_core
    wxbase32u
    wxmsw32u_adv
)
set(WX_LIB_BUILD gcc810_x64)
target_link_libraries(${PROJECT_NAME} ${WX_LIBRARIES})
foreach(LIB ${WX_LIBRARIES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${WX_WIN_PATH}/lib/${LIB}_${WX_LIB_BUILD}.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()