# Find Pangolin (https://github.com/stevenlovegrove/Pangolin)
find_package(Pangolin 0.4 REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

set( CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra ${CMAKE_CXX_FLAGS}" )


# Debug build setting
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")


# Build directory setting
set(
CMAKE_RUNTIME_OUTPUT_DIRECTORY
${CMAKE_HOME_DIRECTORY}/build
)

# Add this if there are multiple cpp files. Otherwise the compiler will fail to find other cpp files
file(GLOB ALLFILES_PARK
        "*.h"
        "*.cpp"
        )


add_executable(SurfelViewer ${ALLFILES_PARK})

target_link_libraries(SurfelViewer ${Pangolin_LIBRARIES} )

