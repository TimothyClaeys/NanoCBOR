add_library(${PROJECT_NAME})

set(nanocbor_sources
  ${PROJECT_SOURCE_DIR}/src/decoder.c
  ${PROJECT_SOURCE_DIR}/src/encoder.c)

target_sources(${PROJECT_NAME} PRIVATE ${nanocbor_sources})
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  PRIVATE
  ${PROJECT_SOURCE_DIR}/src)
