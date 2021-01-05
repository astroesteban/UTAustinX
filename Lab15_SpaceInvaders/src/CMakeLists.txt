add_executable(SpaceInvaders main.c)

target_link_libraries(SpaceInvaders PRIVATE ${TIVAWARE_PATH}/usblib/gcc/libusb.a
                                            ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a)
