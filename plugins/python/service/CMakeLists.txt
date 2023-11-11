find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Boost REQUIRED COMPONENTS python)

include_directories(
  include
  ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp
  ${PROJECT_SOURCE_DIR}/util/include
  ${PROJECT_SOURCE_DIR}/webserver/include
  ${PROJECT_BINARY_DIR}/service/language/gen-cpp
  ${PROJECT_BINARY_DIR}/service/project/gen-cpp
  ${PROJECT_SOURCE_DIR}/service/project/include)

include_directories(SYSTEM
  ${THRIFT_LIBTHRIFT_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${Python3_INCLUDE_DIRS})

add_library(pythonservice SHARED
  src/pythonservice.cpp
  src/plugin.cpp)

target_compile_options(pythonservice PUBLIC -Wno-unknown-pragmas)

target_link_libraries(pythonservice
  util
  projectservice
  languagethrift
  ${THRIFT_LIBTHRIFT_LIBRARIES}
  ${ODB_LIBRARIES}
  ${Boost_LIBRARIES}
  ${Python3_LIBRARIES})

install(TARGETS pythonservice DESTINATION ${INSTALL_SERVICE_DIR})
install(
        DIRECTORY ${PLUGIN_DIR}/service/pyservice/
        DESTINATION ${INSTALL_SERVICE_DIR}/pyservice/)
