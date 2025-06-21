#include "comm.hpp"

#if defined(ARDUINO_AVR_UNO)
#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
// Data pin is connected to pin 11
RH_ASK driver;
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
RH_NRF24 driver(9, 10);  // CE pin 9, CSN pin 10
#else
#error \
    "Unsupported communication module! Please define ROBOT_COMM_MODULE to either ROBOT_COMM_MODULE_RF433 or ROBOT_COMM_MODULE_NRF24."
#endif  // ROBOT_COMM_MODULE
#elif defined(ARDUINO_ESP32_DEV)
#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
RH_ASK driver(2000, 21, 22);
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
RH_NRF24 driver(21, 22);  // CE pin 21, CSN pin 22
#else
#error \
    "Unsupported communication module! Please define ROBOT_COMM_MODULE to either ROBOT_COMM_MODULE_RF433 or ROBOT_COMM_MODULE_NRF24."
#endif  // ROBOT_COMM_MODULE
#endif

String receivedMessage = "";
unsigned long lastDataReceiveTime = 0;