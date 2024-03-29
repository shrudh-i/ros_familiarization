cmake_minimum_required(VERSION 2.8.3)
project(counter_node)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  chatbot_node
)

### Generate services in the 'srv' filder
add_service_files(
  FILES
  counter.srv
)

### Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(counter_node src/counter_node.cpp)
add_dependencies(counter_node ${catkin_EXPORTED_TARGETS})
add_dependencies(counter_node message_ui_generate_messages_cpp)
add_dependencies(counter_node chatbot_node_generate_messages_cpp)
add_dependencies(counter_node arithmetic_node_generate_messages_cpp)
target_link_libraries(counter_node ${catkin_LIBRARIES} )
