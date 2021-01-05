#Set cross compilation information
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# GCC toolchain prefix
set(TOOLCHAIN_PREFIX "arm-none-eabi")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-as)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)

enable_language(ASM)

set(CPU "-mcpu=cortex-m4")
set(FPU "-mfpu=fpv4-sp-d16 -mfloat-abi=softfp")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mthumb ${CPU}  ${FPU} -MD")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb ${CPU} ${FPU} -g3 -Os -std=c11 -ffunction-sections -fdata-sections -MD -Wall -Wextra -Werror -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wundef -fno-common -Wconversion -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthumb ${CPU} ${FPU} -g3 -Os -std=c++17 -ffunction-sections -fdata-sections -MD -Wall -pedantic -fno-exceptions -fno-rtti")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_SOURCE_DIR}/cmake/toolchains/tm4c123g/tm4c123g.ld -specs=${CMAKE_SOURCE_DIR}/cmake/toolchains/tm4c123g/tiva.specs")

# Processor specific definitions
add_definitions(-DPART_TM4C123GH6PM)
add_definitions(-DTARGET_IS_TM4C123_RA1)
add_definitions(-Dgcc)

set(FLASH_EXECUTABLE "lm4flash")
ADD_CUSTOM_TARGET("flash" DEPENDS ./src/${CMAKE_PROJECT_NAME}
  COMMAND ${CMAKE_OBJCOPY} -O binary ./src/${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}.bin 
  COMMAND ${FLASH_EXECUTABLE} ${CMAKE_PROJECT_NAME}.bin
)

### Allow user to provide path to the TIVAWARE source
set(TIVAWARE_PATH "/" CACHE PATH "Path to the Tivaware directory")