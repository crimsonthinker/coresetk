PROJECT(CoreSampling VERSION 1.0.0 LANGUAGES CXX)
# Find python3 and boost
FIND_PACKAGE(PythonInterp)
if (PYTHONINTERP_FOUND)
  if (UNIX AND NOT APPLE)
    FIND_PACKAGE(Boost REQUIRED COMPONENTS python${PYTHON_VERSION_MAJOR})
    FIND_PACKAGE(PythonInterp 3)
    FIND_PACKAGE(PythonLibs 3 REQUIRED)
  else()	
    FIND_PACKAGE(Boost REQUIRED COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
    FIND_PACKAGE(PythonInterp 3)
    FIND_PACKAGE(PythonLibs 3 REQUIRED)
  endif()
else()
    message("Python not found")
endif()
message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(core MODULE ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp ${SOURCES})

# Set up the libraries and header search paths for this target
target_link_libraries(core ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(core PRIVATE ${PYTHON_INCLUDE_DIRS})

#change output of core
set_target_properties(core PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../__c__)