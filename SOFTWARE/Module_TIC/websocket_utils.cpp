#include <WebSocketsServer_Generic.h>

#include "websocket_utils.h"

//< Déclaration des variables globales
static WebSocketsServer* ws_server_socket;
static TIC_DATA_t *tic_data;

/**
 * \fn void ws_server_on_event(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
 * \brief Fonction permettant des gerer les évênements lies aux WS Serveur
 * 
 * \param in, le numéro de l'évènement reçu en WS
 * \param in, le type d'évènement reçu en WS
 * \param in, la chaine de caractères reçue sur la WS
 * \param in, la longeur de la chaine reçue sur la WS
 */
static void ws_server_on_event(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  char data_to_send[64];
  switch (type) {
    case WStype_CONNECTED:                                  // Un nouveau client s'est connecté !
      if(tic_data->_nb_elem) {                              // Des données TIC sont présentes
        for (uint8_t i = 0; i < tic_data->_nb_elem; i++) {  // Itération sur les donnees TIC
          snprintf(data_to_send, sizeof(data_to_send),      // Stringification des données
                   "{\"%s\":\"%s\"}",
                   tic_data->etiquette[i],
                   tic_data->valeur[i]);
          ws_server_socket->sendTXT(num,data_to_send);      // Envois au client uniquement
        }
      }
      break;
    case WStype_TEXT:
      ws_server_socket->sendTXT(num,payload);               // Réalisation d'un echo !
      break;
    case WStype_DISCONNECTED:
    default:
      break;
  }
}

/**
 * \fn void ws_server_init(uint16_t port, TIC_DATA_t *tic)
 */
void ws_server_init(uint16_t port, TIC_DATA_t *tic) {
  ws_server_socket = new WebSocketsServer(port);            // Configuration du serveur de WS
  ws_server_socket->onEvent(ws_server_on_event);            // Configuration du handler OnEvent
  ws_server_socket->begin();                                // Activation du serveur de WS
  tic_data = tic;
}

/**
 * \fn void ws_server_send_broadcast(uint8_t elem)
 */
void ws_server_send_broadcast(uint8_t elem) {
    char data_to_send[64];
    snprintf(data_to_send, sizeof(data_to_send),            // Stringification des données
         "{\"%s\":\"%s\"}",
         tic_data->etiquette[elem],
         tic_data->valeur[elem]);
    ws_server_socket->broadcastTXT(data_to_send);           // Envois en broadcast sur la WS Server
}

/**
 * \fn void ws_handler(void)
 */
void ws_handler(void) {
   ws_server_socket->loop();                                // Handler du service WebSocket Server
}
