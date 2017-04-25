add_subdirectory(RaspberryPiMouseController_Joystick)

MAP_ADD_STR(hdrs "${PROJECT_NAME}/" headers)
set(headers ${headers} PARENT_SCOPE)
