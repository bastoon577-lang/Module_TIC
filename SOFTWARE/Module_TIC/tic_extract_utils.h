#ifndef __TIC_EXTRACT_UTILS__
#define __TIC_EXTRACT_UTILS__

#include "websocket_utils.h"
#include "common_utils.h"

//< Caractères d'encapsulation de trame globale Linky
#define STX_CAR_0           '\x02'  // <STX> Start of Text (Début de trame)
#define ETX_CAR_0           '\x03'  // <ETX> End of Text (Fin de trame)

//< Caractères de début et fin de ligne (communs aux deux modes)
#define STX_CAR_1           '\x0A'  // <LF> Début de groupe (0x0A)
#define ETX_CAR_1           '\x0D'  // <CR> Fin de groupe (0x0D)

//< Séparateurs de champs
#define SEP_CAR_HISTORIQUE  '\x20'  // <SP> Space (0x20)
#define SEP_CAR_STANDARD    '\x09'  // <HT> Horizontal Tab (0x09)

//< Define des Timeouts
#define TIMEOUT_ACCUMULATE  20000   // TimeOut d'accumulation
#define TIMEOUT_LINKY_DATA  2500    // TimeOut inter-trames

//< Structure des données TIC_EXTRACT
typedef struct {
  unsigned long timer_accumulate;
  TIC_DATA_t *tic_data;
} TIC_EXTRACT_t;

/**
 * \fn void tic_extract_init(TIC_DATA_t *tic)
 * \brief Fonction permettant l'initialisation du service tic_extract
 * \param in, le pointeur vers la structure de données TIC_DATA_t
 * \param in, le mode du compteur (Standard ou Historique)
 */
void tic_extract_init(TIC_DATA_t *tic, uint8_t standard_mode);

/**
 * \fn void tic_extract_handler(void)
 * \brief Handler du tic_extract, à appeler régulièrement.
 *        Cette fonction permet :
 *          - La lecture sur l'UART des données issues du compteur
 *          - L'extraction/desencapsulation autour du protocole Linky
 *          - La sauvegarde des données dans la structure TIC_DATA_t 
 */
void tic_extract_handler(void);

#endif
