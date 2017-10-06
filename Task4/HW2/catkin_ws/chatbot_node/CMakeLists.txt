cmake_minimum_required(VERSION 2.8.3)
project(chatbot_node)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

## Generate messages in the 'msg' folder
add_message_files(
   FILES
   reply_msg.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(chatbot_node src/chatbot_node.cpp)
add_dependencies(chatbot_node ${catkin_EXPORTED_TARGETS})
add_dependencies(chatbot_node message_ui_generate_messages_cpp)
target_link_libraries(chatbot_node ${catkin_LIBRARIES} )
