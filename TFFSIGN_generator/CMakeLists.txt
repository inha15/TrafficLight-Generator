cmake_minimum_required(VERSION 3.0.2)
project(TFFSIGN_generator)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  pcl_conversions
  pcl_ros
  std_msgs
  message_generation
  rospy
  cv_bridge
  image_transport
)


generate_messages(
  DEPENDENCIES
  sensor_msgs
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS message_generation roscpp std_msgs rospy cv_bridge image_transport message_runtime
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(TFF_SIG_GEN src/TFF_SIG_generator.cpp)
target_link_libraries(TFF_SIG_GEN ${catkin_LIBRARIES} ${OpenCV_INCLUDE_DIRS})