#ifndef __WEBSOCKET_UTILS__
#define __WEBSOCKET_UTILS__

#include "common_utils.h"

/**
 * \fn void ws_server_init(uint16_t port)
 * \brief Fonction permettant d'initialiser le service WebSocket Server
 *        Ce service permet le raffraichissement des données sur l'interface WEB
 *        du SmartCharger et doit donc être initialisé tout le temps
 * 
 * \param in, le port de service WS à utiliser
 * \param in, le pointeur vers la structure de données TIC_DATA_t
 */
void ws_server_init(uint16_t port, TIC_DATA_t *tic);

/**
 * \fn void ws_server_send_broadcast(String etiquette, String value)
 * \brief Fonction permettant d'émettre en broadcast les informations
 *        aux clients WS connectés
 * \param in, l'élément devant être transmis
 */
void ws_server_send_broadcast(uint8_t elem);

/**
 * \fn void ws_handler(void)
 * \brief Handler du module WebSocket à appeler régulièrement
 */
void ws_handler(void);

#endif
