set(hdrs RaspberryPiMouseController_Joystick.h
    PARENT_SCOPE
    )

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

