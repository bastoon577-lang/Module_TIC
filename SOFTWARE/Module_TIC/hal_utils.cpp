#include "hal_utils.h"

/**
 * \fn void hal_init(void)
 */
void hal_init(void) {
  pinMode(GPIO_DMD_RESET,INPUT_PULLUP);                               // Configuration de l'entrée Reboot
  pinMode(GPIO_LED_IHM,OUTPUT);                                       // Configuration de la LED
}

/**
 * \fn hal_uart_init(uint8_t standard_mode)
 */
void hal_uart_init(uint8_t standard_mode) {
  if(standard_mode)
    TIC_UART.begin(STANDARD_BAUDRATES,SERIAL_7E1);                   // Configuration de l'UART pour le mode Standard
  else
    TIC_UART.begin(HISTORIC_BAUDRATES,SERIAL_7E1);                   // Configuration de l'UART pour le mode Historique
}

/**
 * \fn void hal_toggle_led(void)
 */
void hal_toggle_led(void) {
  digitalWrite(GPIO_LED_IHM,!digitalRead(GPIO_LED_IHM));              // Toggle de la LED
}

/**
 * \fn bool hal_get_reboot(void)
 */
bool hal_get_reboot(void) {
  return digitalRead(GPIO_DMD_RESET);                                 // Lecture de l'entrée Reboot
}

/**
 * \fn int hal_tic_data_avalaible(void)
 */
int hal_tic_data_avalaible(void) {
  return TIC_UART.available();                                        // Lecture de données disponible sur l'UART
}

/**
 * \fn char hal_get_tic_data(void)
 */
char hal_get_tic_data(void) {
  return (char)TIC_UART.read();                                       // Lecture du buffer UART
}
