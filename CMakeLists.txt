cmake_minimum_required(VERSION 3.0.0)

project(sip)

add_subdirectory(thirdparty/json)

add_library(${PROJECT_NAME} SHARED)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED 17

  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"

  OUTPUT_NAME "sip"
  SUFFIX ".asi"
)

set(SRC
  src/main.cpp
  src/init.cpp
  src/sighelper.cpp
  src/util.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_sources(${PROJECT_NAME} PRIVATE ${SRC})