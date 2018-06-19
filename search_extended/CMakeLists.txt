set(SC_SEARCH_EXTENDED_SRC "${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

include_directories(${SC_SEARCH_EXTENDED_SRC})

add_library(sc-search-extended SHARED ${SOURCES} ${HEADERS})
add_dependencies(sc-search-extended sc-memory-cpp sc-kpm-common)
target_link_libraries(sc-search-extended sc-memory-cpp sc-kpm-common)

sc_codegen(sc-search-extended ${SC_SEARCH_EXTENDED_SRC})
