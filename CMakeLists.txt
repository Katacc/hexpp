cmake_minimum_required(VERSION 4.0)

configure_file(${CMAKE_SOURCE_DIR}/project.cfg ${CMAKE_BINARY_DIR}/project.cfg COPYONLY)
file(STRINGS ${CMAKE_SOURCE_DIR}/project.cfg config_lines)


foreach(line IN LISTS config_lines)
     if(line MATCHES "^name =")
         string(LENGTH "name = " prefix_len)
         string(SUBSTRING "${line}" ${prefix_len} -1 PROJ_NAME)

     endif()
     if(line MATCHES "^version =")
         string(LENGTH "version = " prefix_len_ver)
         string(SUBSTRING "${line}" ${prefix_len_ver} -1 PROJ_VERSION)

     endif()
     if(line MATCHES "^CXX_version =")
         string(LENGTH "CXX_version = " prefix_len_cxx)
         string(SUBSTRING "${line}" ${prefix_len_cxx} -1 cxx_version)

     endif()
endforeach()


project(${PROJ_NAME} VERSION ${PROJ_VERSION})


# Static libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# C++ version
set(CMAKE_CXX_STANDARD ${cxx_version})
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Determine the executable extension based on the operating system
if(WIN32)
    set(EXECUTABLE_EXTENSION ".exe")
else()
    set(EXECUTABLE_EXTENSION "")
endif()

# Find all .cpp files in src/commands
file(GLOB_RECURSE COMMANDS_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Target
add_executable(${PROJECT_NAME}
                    # .cpp files here or variables containing them
                    ${COMMANDS_SOURCES}
                )

# Output


# Includes
target_include_directories(${PROJECT_NAME} PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/src"
                            "${PROJECT_SOURCE_DIR}/src/libraries"
                            )


# EXAMPLE OF ADDING A PACKAGE
# Packages
# find_package(SDL3 REQUIRED)

# SDL
# target_link_libraries(yoru PUBLIC SDL3::SDL3)

