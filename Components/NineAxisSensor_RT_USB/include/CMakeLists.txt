add_subdirectory(NineAxisSensor_RT_USB)

MAP_ADD_STR(hdrs "${PROJECT_NAME}/" headers)
set(headers ${headers} PARENT_SCOPE)
