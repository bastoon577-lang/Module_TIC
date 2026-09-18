#include "tic_extract_utils.h"

static bool tic_extract_is_standard_mode;
static TIC_EXTRACT_t tic_extract;

/**
 * \fn char tic_extract_checksum_compute(const String& data)
 */
static char tic_extract_checksum_compute(const String& data) {
  uint16_t checksum = 0;                                                          // Initialisation
  for (size_t i = 0; i < data.length(); i++) {
    checksum += static_cast<uint8_t>(data[i]);                                    // Somme des caractères
  }
  checksum = (checksum & 0x3F) + 0x20;                                            // Application du masque et ajout 0x20
  return static_cast<char>(checksum);
}

/**
 * \fn void tic_extract_parser(const String& str)
 */
__attribute__((optimize(0))) static void tic_extract_parser(const String& str) {
  if (str.length() > MAX_BUFFER_SIZE || str.length() < 5) return;                 // Overflow de la chaine, échappement

  const char sep = (tic_extract_is_standard_mode) ? SEP_CAR_STANDARD :            // Sélection du séparateur (Historique / Standard)
                                                              SEP_CAR_HISTORIQUE;

  const int check_sum_index = str.length() - 1;
  const char checksum = str.charAt(check_sum_index);

  String raw_payload = str.substring(0, check_sum_index);                         // Extraction de la Payload brute sans le caractère de Checksum
                                                                                  // Isolation des différents champs de la trame
  String horodate = "";
  String label = "";
  String data = "";

  const int first_sep = raw_payload.indexOf(sep);
  if (first_sep == -1) return;

  label = raw_payload.substring(0, first_sep);                                    // Extraction du Label
                                                                                  // En mode Standard (2 ou 3 champs disponibles)
  if (tic_extract_is_standard_mode) {
    const int last_sep = raw_payload.lastIndexOf(sep);
    const int second_sep = raw_payload.indexOf(sep, first_sep + 1);
                                                                                  // Vérification du nombre de champs
    if (second_sep != -1 && second_sep != last_sep) {
                                                                                  // 3 champs (Etiquette \t Horodate \t Donnée)
      horodate = raw_payload.substring(first_sep + 1, second_sep);
      data = raw_payload.substring(second_sep + 1, last_sep);
    } else {
                                                                                  // 2 champs (Etiquette \t Donnée)
      data = raw_payload.substring(first_sep + 1, last_sep);
    }
  } else {                                                                        // En mode Historique (Etiquette ' ' Donnée)
    if (raw_payload.endsWith(" ")) {
      raw_payload.remove(raw_payload.length() - 1);                               // Suppression de l'espace final s'il est présent avant la checksum
    }
    data = raw_payload.substring(first_sep + 1);                                  // Extraction de la donnée
  }

  if (!data.length() && horodate.length() > 0) {
    data = horodate;
  }
                                                                                  // Reconstitution exacte du buffer de calcul de Checksum selon le mode
  String payload_for_checksum = "";

  if (tic_extract_is_standard_mode) {                                             // Mode Standard (INCLUT le dernier séparateur 0x09)
    if (horodate.length() > 0) {
      payload_for_checksum = label + sep + horodate + sep + data + sep;
    } else {
      payload_for_checksum = label + sep + data + sep;
    }
  } else {                                                                        // Mode Historique (N'INCLUT PAS l'espace de fin)
    payload_for_checksum = label + sep + data;
  }
                                                                                  // Validation Checksum
  if (tic_extract_checksum_compute(payload_for_checksum) != checksum) {
    return;
  }
                                                                                  // Contrôles de taille et de format
  if (!label.length() || label.length() > MAX_SIZE_LABEL) return;
  if (!data.length() || data.length() > MAX_SIZE_VALUE) return;

  // Mise à jour d'une données existante de la table
  for (uint8_t i = 0; i < tic_extract.tic_data->_nb_elem; i++) {                  // Parcourt de la table de données
    if (strcmp(tic_extract.tic_data->etiquette[i], label.c_str()) == 0) {         // L'étiquette existe
      if (strcmp(tic_extract.tic_data->valeur[i], data.c_str()) != 0) {           // La valeur sauvegardée est différente
        memset(tic_extract.tic_data->valeur[i], 0, MAX_SIZE_VALUE);               // Mise à jour de la donnée
        strncpy(tic_extract.tic_data->valeur[i], data.c_str(), MAX_SIZE_VALUE - 1);
        ws_server_send_broadcast(i);                                              // Envois en Broadcast de la nouvelle donnée
      }
      return;
    }
  }

  // Autrement, ajout d'une nouvelle donnée (Etiquette, Valeur) dans la table uniquement si l'accumulation est active !
  if (!tic_extract.tic_data->_accumulate && tic_extract.tic_data->_nb_elem < MAX_TIC_DATA) {
    memset(tic_extract.tic_data->etiquette[tic_extract.tic_data->_nb_elem], 0, MAX_SIZE_LABEL);
    memset(tic_extract.tic_data->valeur[tic_extract.tic_data->_nb_elem], 0, MAX_SIZE_VALUE);

    strncpy(tic_extract.tic_data->etiquette[tic_extract.tic_data->_nb_elem], label.c_str(), MAX_SIZE_LABEL - 1);
    strncpy(tic_extract.tic_data->valeur[tic_extract.tic_data->_nb_elem], data.c_str(), MAX_SIZE_VALUE - 1);

    tic_extract.tic_data->_nb_elem++;
  }
}

/**
 * \fn void tic_extract_init(TIC_DATA_t *tic, uint8_t standard_mode)
 */
void tic_extract_init(TIC_DATA_t *tic, uint8_t standard_mode) {
  tic_extract_is_standard_mode = standard_mode;
  tic_extract.timer_accumulate = millis();
  tic_extract.tic_data = tic;
}

/**
 * \fn void tic_extract_handler(void)
 */
void tic_extract_handler(void) {
  static unsigned long timer_no_tic_data = millis();
  static bool processing = false;
  static String data_buffer;

  while (hal_tic_data_avalaible() > 0) {                                          // Tant que de la donnee est disponible sur l'UART
    char data = hal_get_tic_data();                                               // Lecture de la données sur l'UART

    timer_no_tic_data = millis();                                                 // Réarmement du timer inter-trames
                                                                                  // Filtre des caractères d'encapsulation (STX / ETX)
    if (data == STX_CAR_0 || data == ETX_CAR_0)
      continue;

    if (data == STX_CAR_1) {                                                      // Caractère <LF> (0x0A) - Début de ligne détecté
      data_buffer = "";                                                           // Initialisation du buffer temporaire
      data_buffer.reserve(MAX_BUFFER_SIZE);                                       // Optimisation (anti-fragmentation)
      processing = true;                                                          // Lancement du processus de bufferisation
    } else if (processing) {                                                      // Le porcessus d'accumulation est en cours...
      if (data == ETX_CAR_1) {                                                    // Caractère <CR> (0x0D) - Fin de ligne détecté
        tic_extract_parser(data_buffer);                                          // Traitement du buffer temporaire
        processing = false;                                                       // Arrêt du processus de bufferisation
      } else {                                                                    // Autrement...
        if (data_buffer.length() < MAX_BUFFER_SIZE - 1) {                         // Si le buffer temporaire peut encore accumuler de la donnée
          data_buffer += data;                                                    // Bufferisation de la nouvelle donnée
        }
      }
    }
                                                                                  // Timeout : Gestion de la période d'accumulation de nouvelles données
    if (millis() - tic_extract.timer_accumulate >= TIMEOUT_ACCUMULATE) {          // Fin du temps impartit
      tic_extract.tic_data->_accumulate = 1;                                      // Stop l'accumulation de nouvelles données
    }
  }
                                                                                  // Timeout : Absence de données TIC, réinitialisation sécurisée
  if (millis() - timer_no_tic_data >= TIMEOUT_LINKY_DATA) {                       // Fin du temps impartit
    if (tic_extract.tic_data->_nb_elem > 0) {                                     // Des données sont mémorisées
      memset(tic_extract.tic_data, 0, sizeof(TIC_DATA_t));                        // Suppression des données
      tic_extract.timer_accumulate = millis();                                    // Réinitialisation du timer d'accumulation
    }
    timer_no_tic_data = millis();                                                 // Evite la réinitialisation en boucle
  }
}
