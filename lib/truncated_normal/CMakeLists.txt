file(GLOB_RECURSE tNsourceFiles ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_library(truncated_normal STATIC ${tNsourceFiles})
set_target_properties(truncated_normal PROPERTIES CXX_STANDARD 11)
set_target_properties(truncated_normal PROPERTIES CXX_STANDARD_REQUIRED ON)

