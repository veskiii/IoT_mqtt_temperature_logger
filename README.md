# IoT_mqtt_temperature_logger
IoT MQTT Temperature Logger + IP configuration by website. Created for STM32H743ZI using LwIP.


# Features
* Temperature measurement using thermistor and ADC,
* Cyclic temperature logs by MQTT,
* LEDs controlled by MQTT and website,
* Changing IP and subnet mask by website,
* IP configuration stored in flash memory.


# MQTT
Tested with Mosquitto.
## Topics
/presence        - STM says Hello!  
/led             - LED control  
/temperature     - Temperature logs  
/flash           - STM sends IP address if it's read from flash (not default)

## LED control
* LD1_ON, LD2_ON, LD3_ON     - turns selected LED on.
* LD1_OFF, LD2_OFF, LD3_OFF  - turns selected LED off.
* LD_STATE                   - STM will reply which LEDs are on.
