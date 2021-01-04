add_executable(space_invaders main.c)

target_link_libraries(space_invaders PRIVATE ${TIVAWARE_PATH}/usblib/gcc/libusb.a
                                             ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a)
