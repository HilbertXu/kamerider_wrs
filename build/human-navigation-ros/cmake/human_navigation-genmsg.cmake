# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "human_navigation: 7 messages, 0 services")

set(MSG_I_FLAGS "-Ihuman_navigation:/home/kamerider/sim_ws/src/human-navigation-ros/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(human_navigation_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" ""
)

get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" "geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" "geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" "geometry_msgs/Quaternion:geometry_msgs/Pose:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" "geometry_msgs/Quaternion:human_navigation/HumanNaviDestination:geometry_msgs/Point:human_navigation/HumanNaviObjectInfo"
)

get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" ""
)

get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" NAME_WE)
add_custom_target(_human_navigation_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "human_navigation" "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" "geometry_msgs/Quaternion:geometry_msgs/Point:human_navigation/HumanNaviObjectInfo"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)
_generate_msg_cpp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
)

### Generating Services

### Generating Module File
_generate_module_cpp(human_navigation
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(human_navigation_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(human_navigation_generate_messages human_navigation_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_cpp _human_navigation_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(human_navigation_gencpp)
add_dependencies(human_navigation_gencpp human_navigation_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS human_navigation_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)
_generate_msg_eus(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
)

### Generating Services

### Generating Module File
_generate_module_eus(human_navigation
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(human_navigation_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(human_navigation_generate_messages human_navigation_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_eus _human_navigation_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(human_navigation_geneus)
add_dependencies(human_navigation_geneus human_navigation_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS human_navigation_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)
_generate_msg_lisp(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
)

### Generating Services

### Generating Module File
_generate_module_lisp(human_navigation
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(human_navigation_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(human_navigation_generate_messages human_navigation_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_lisp _human_navigation_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(human_navigation_genlisp)
add_dependencies(human_navigation_genlisp human_navigation_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS human_navigation_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)
_generate_msg_nodejs(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
)

### Generating Services

### Generating Module File
_generate_module_nodejs(human_navigation
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(human_navigation_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(human_navigation_generate_messages human_navigation_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_nodejs _human_navigation_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(human_navigation_gennodejs)
add_dependencies(human_navigation_gennodejs human_navigation_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS human_navigation_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)
_generate_msg_py(human_navigation
  "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
)

### Generating Services

### Generating Module File
_generate_module_py(human_navigation
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(human_navigation_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(human_navigation_generate_messages human_navigation_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviDestination.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviAvatarStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviTaskInfo.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviGuidanceMsg.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kamerider/sim_ws/src/human-navigation-ros/msg/HumanNaviObjectStatus.msg" NAME_WE)
add_dependencies(human_navigation_generate_messages_py _human_navigation_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(human_navigation_genpy)
add_dependencies(human_navigation_genpy human_navigation_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS human_navigation_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/human_navigation
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(human_navigation_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(human_navigation_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/human_navigation
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(human_navigation_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(human_navigation_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/human_navigation
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(human_navigation_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(human_navigation_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/human_navigation
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(human_navigation_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(human_navigation_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/human_navigation
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(human_navigation_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(human_navigation_generate_messages_py geometry_msgs_generate_messages_py)
endif()
