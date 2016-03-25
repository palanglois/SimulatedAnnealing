#Getting the .cpp files
file(GLOB_RECURSE sAsourceFiles ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

#Creating the executable funcOpt
add_executable(funcOpt ${sAsourceFiles})

#Linking Qt5 to funcOpt
target_link_libraries(funcOpt Qt5::Widgets)

#Linking the truncated_normal library
target_link_libraries(funcOpt truncated_normal)
set_target_properties(funcOpt PROPERTIES CXX_STANDARD 11)
set_target_properties(funcOpt PROPERTIES CXX_STANDARD_REQUIRED ON)

