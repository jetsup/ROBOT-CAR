#include "comm.hpp"

String receivedMessage = "";
unsigned long lastDataReceiveTime = 0;

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
RH_ASK driver(RF433_SPEED, RF433_DATA_PIN, RF433_TX_PIN);
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
// Let these addresses be used for the pair
RF24 driver(NRF24L01_CE_PIN, NRF24L01_CSN_PIN);
const uint8_t address[] = "00001";
#endif  // CONTROLLER_COMM_MODULE_NRF24