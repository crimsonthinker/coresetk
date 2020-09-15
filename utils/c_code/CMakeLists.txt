PROJECT(utils VERSION 1.0.0 LANGUAGES CXX)

# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(core MODULE ${CMAKE_CURRENT_SOURCE_DIR}/__c__.cpp ${SOURCES})

# Set up the libraries and header search paths for this target
target_link_libraries(core ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(core PRIVATE ${PYTHON_INCLUDE_DIRS})

#change output of core
set_target_properties(core PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)