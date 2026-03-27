cmake_minimum_required(VERSION 3.14)

project(freertos-cpp)

# If no freertos_config target was defined by a parent project, a default
# one is created with default compile options, and the include path is
# expected to be added later in the CMake configuration.
if(NOT TARGET freertos_config)
    add_library(freertos_config INTERFACE)
    target_compile_definitions(freertos_config
    INTERFACE
        projCOVERAGE_TEST=0
        projENABLE_TRACING=0
    )

    message(
    STATUS
        "freertos-cpp: Empty freertos_config target created. Please include the directory "
        "containing FreeRTOSConfig.h to this target using \"target_include_directories\"".
    )
endif()

include(FetchContent)

# Get the latest version of the kernel from the master (main) branch.
# If a parent project declares a more specific version before this one,
# the more specific version will be used.
FetchContent_Declare(
    freertos_kernel
    GIT_REPOSITORY  https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG         main
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(freertos_kernel)

# Library definition
add_library(freertos_cpp INTERFACE)
target_include_directories(freertos_cpp INTERFACE "include")
target_link_libraries(freertos_cpp INTERFACE freertos_kernel)
