# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "interactive_cleanup: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iinteractive_cleanup:/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(interactive_cleanup_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" NAME_WE)
add_custom_target(_interactive_cleanup_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "interactive_cleanup" "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(interactive_cleanup
  "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/interactive_cleanup
)

### Generating Services

### Generating Module File
_generate_module_cpp(interactive_cleanup
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/interactive_cleanup
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(interactive_cleanup_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(interactive_cleanup_generate_messages interactive_cleanup_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" NAME_WE)
add_dependencies(interactive_cleanup_generate_messages_cpp _interactive_cleanup_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(interactive_cleanup_gencpp)
add_dependencies(interactive_cleanup_gencpp interactive_cleanup_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS interactive_cleanup_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(interactive_cleanup
  "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/interactive_cleanup
)

### Generating Services

### Generating Module File
_generate_module_eus(interactive_cleanup
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/interactive_cleanup
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(interactive_cleanup_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(interactive_cleanup_generate_messages interactive_cleanup_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" NAME_WE)
add_dependencies(interactive_cleanup_generate_messages_eus _interactive_cleanup_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(interactive_cleanup_geneus)
add_dependencies(interactive_cleanup_geneus interactive_cleanup_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS interactive_cleanup_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(interactive_cleanup
  "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/interactive_cleanup
)

### Generating Services

### Generating Module File
_generate_module_lisp(interactive_cleanup
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/interactive_cleanup
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(interactive_cleanup_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(interactive_cleanup_generate_messages interactive_cleanup_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" NAME_WE)
add_dependencies(interactive_cleanup_generate_messages_lisp _interactive_cleanup_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(interactive_cleanup_genlisp)
add_dependencies(interactive_cleanup_genlisp interactive_cleanup_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS interactive_cleanup_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(interactive_cleanup
  "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/interactive_cleanup
)

### Generating Services

### Generating Module File
_generate_module_nodejs(interactive_cleanup
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/interactive_cleanup
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(interactive_cleanup_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(interactive_cleanup_generate_messages interactive_cleanup_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" NAME_WE)
add_dependencies(interactive_cleanup_generate_messages_nodejs _interactive_cleanup_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(interactive_cleanup_gennodejs)
add_dependencies(interactive_cleanup_gennodejs interactive_cleanup_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS interactive_cleanup_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(interactive_cleanup
  "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/interactive_cleanup
)

### Generating Services

### Generating Module File
_generate_module_py(interactive_cleanup
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/interactive_cleanup
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(interactive_cleanup_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(interactive_cleanup_generate_messages interactive_cleanup_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kamerider/sim_ws/src/CleanUp/interactive-cleanup-ros/msg/InteractiveCleanupMsg.msg" NAME_WE)
add_dependencies(interactive_cleanup_generate_messages_py _interactive_cleanup_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(interactive_cleanup_genpy)
add_dependencies(interactive_cleanup_genpy interactive_cleanup_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS interactive_cleanup_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/interactive_cleanup)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/interactive_cleanup
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(interactive_cleanup_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/interactive_cleanup)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/interactive_cleanup
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(interactive_cleanup_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/interactive_cleanup)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/interactive_cleanup
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(interactive_cleanup_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/interactive_cleanup)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/interactive_cleanup
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(interactive_cleanup_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/interactive_cleanup)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/interactive_cleanup\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/interactive_cleanup
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(interactive_cleanup_generate_messages_py std_msgs_generate_messages_py)
endif()
