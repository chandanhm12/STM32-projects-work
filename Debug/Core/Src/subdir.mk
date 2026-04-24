################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LED.c \
../Core/Src/PWM_Timer.c \
../Core/Src/SPI.c \
../Core/Src/Uart.c \
../Core/Src/ds18b20.c \
../Core/Src/eeprom.c \
../Core/Src/i2c_gpio.c \
../Core/Src/main.c \
../Core/Src/mpr121.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c 

OBJS += \
./Core/Src/LED.o \
./Core/Src/PWM_Timer.o \
./Core/Src/SPI.o \
./Core/Src/Uart.o \
./Core/Src/ds18b20.o \
./Core/Src/eeprom.o \
./Core/Src/i2c_gpio.o \
./Core/Src/main.o \
./Core/Src/mpr121.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o 

C_DEPS += \
./Core/Src/LED.d \
./Core/Src/PWM_Timer.d \
./Core/Src/SPI.d \
./Core/Src/Uart.d \
./Core/Src/ds18b20.d \
./Core/Src/eeprom.d \
./Core/Src/i2c_gpio.d \
./Core/Src/main.d \
./Core/Src/mpr121.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/LED.cyclo ./Core/Src/LED.d ./Core/Src/LED.o ./Core/Src/LED.su ./Core/Src/PWM_Timer.cyclo ./Core/Src/PWM_Timer.d ./Core/Src/PWM_Timer.o ./Core/Src/PWM_Timer.su ./Core/Src/SPI.cyclo ./Core/Src/SPI.d ./Core/Src/SPI.o ./Core/Src/SPI.su ./Core/Src/Uart.cyclo ./Core/Src/Uart.d ./Core/Src/Uart.o ./Core/Src/Uart.su ./Core/Src/ds18b20.cyclo ./Core/Src/ds18b20.d ./Core/Src/ds18b20.o ./Core/Src/ds18b20.su ./Core/Src/eeprom.cyclo ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/eeprom.su ./Core/Src/i2c_gpio.cyclo ./Core/Src/i2c_gpio.d ./Core/Src/i2c_gpio.o ./Core/Src/i2c_gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mpr121.cyclo ./Core/Src/mpr121.d ./Core/Src/mpr121.o ./Core/Src/mpr121.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su

.PHONY: clean-Core-2f-Src

