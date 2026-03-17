#ifndef __HAL_UTILS__
#define __HAL_UTILS__

#include <ESP8266WiFi.h>
#include <Arduino.h>

//< Définition des In/Out
#define TIC_UART                  Serial                // UART des données issues du bornier TIC
#define GPIO_DMD_RESET            0                     // GPIO du bouton Reset
#define GPIO_LED_IHM              2                     // GPIO de la LED

/**
 * \fn void hal_init(void)
 * \brief Fonction permettant d'initialiser la HAL
 */
void hal_init(void);

/**
 * \fn void hal_toggle_led(void)
 * \brief Fonction permettant de toggle l'état de la LED
 */
void hal_toggle_led(void);

/**
 * \fn bool hal_get_reboot(void)
 * \brief Fonction permettant de lire l'état de la GPIO de Reboot
 * \return
 *        L'état de la GPIO Reboot
 */
bool hal_get_reboot(void);

/**
 * \fn int hal_tic_data_avalaible(void)
 * \brief Fonction permettant de vérifier si des données TIC 
 *        sont disponible sur l'UART.
 * \return
 *        Le nombre d'octets disponibles
 */
int hal_tic_data_avalaible(void);

/**
 * \fn char hal_get_tic_data(void)
 * \brief Fonction permettant de lire une donnée TIC 
 *        disponible sur l'UART.
 * \return
 *        Un carractère du buffer UART
 */
char hal_get_tic_data(void);

#endif
