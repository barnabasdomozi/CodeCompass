include_directories(
  include
  ${CMAKE_SOURCE_DIR}/parser/include
  ${CMAKE_SOURCE_DIR}/util/include
  ${CMAKE_SOURCE_DIR}/model/include)

add_library(pythonparser SHARED
  src/pythonparser.cpp)

target_link_libraries(pythonparser
  model)

target_compile_options(pythonparser PUBLIC -Wno-unknown-pragmas)

install(TARGETS pythonparser DESTINATION ${INSTALL_PARSER_DIR})
