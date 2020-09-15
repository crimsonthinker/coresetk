PROJECT(sampling VERSION 1.0.0 LANGUAGES CXX)

# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include #from sampling
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/c_code/include) #from utils

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp #from sampling
                  ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/c_code/src/*.cpp) #from utils
                  
add_library(__c__ MODULE ${CMAKE_CURRENT_SOURCE_DIR}/__c__.cpp ${SOURCES}) 

# Set up the libraries and header search paths for this target
target_link_libraries(__c__ ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(__c__ PRIVATE ${PYTHON_INCLUDE_DIRS})

#change output of core
set_target_properties(__c__ PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)