#ifndef __TIC_EXTRACT_UTILS__
#define __TIC_EXTRACT_UTILS__

#include "websocket_utils.h"
#include "common_utils.h"

//< MACRO d'encapsulation au protocole Linky
#define ETX_CAR_0           '\x03'  // Caractère de fin de chaine protocole
#define STX_CAR_0           '\x02'  // Caractère de début de chaine protocole
#define ETX_CAR_1           '\n'    // Caractère de fin de chaine
#define STX_CAR_1           '\r'    // Caractère de début de chaine

//< Define des Timeout
#define TIMEOUT_ACCUMULATE  20000   // TimeOut d'accumulation
#define TIMEOUT_LINKY_DATA  1000    // TimeOut 

//< Enumeration des timers
typedef enum
{
  tic_extract_accumulate,           // Timer d'accumulation de nouvelles donnees
  tic_extract_linkyData,            // Timer d'absence de donnee
  tic_extract_nb_timers
} TIC_EXTRACT_TIME_e;

//< Structure des données TIC_EXTRACT
typedef struct {
  unsigned long timers[tic_extract_nb_timers];
  TIC_DATA_t *tic_data;
} TIC_EXTRACT_t;

/**
 * \fn void tic_extract_init(TIC_DATA_t *tic)
 * \brief Fonction permettant l'initialisation du service tic_extract
 * \param in, le pointeur vers la structure de données TIC_DATA_t
 */
void tic_extract_init(TIC_DATA_t *tic);

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
