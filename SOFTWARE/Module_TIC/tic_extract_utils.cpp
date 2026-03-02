#include "tic_extract_utils.h"

//< Déclaration des variables globales
static TIC_EXTRACT_t tic_extract;

/**
 * \fn char tic_extract_checksum_compute(const String& data)
 */
static char tic_extract_checksum_compute(const String& data) {
  uint8_t checksum = 0;                                             // Initialisation
  for (size_t i = 0; i < data.length(); i++)
      checksum += data[i];                                          // Somme des caractères
  checksum = (checksum & 0x3F) + 0x20;                              // Application du masque et ajout 0x20
  return static_cast<char>(checksum);
}

/**
 * \fn tic_extract_isCheckSumOK(const String& data,char expectedChecksum)
 */
static bool tic_extract_isCheckSumOK(const String& data,char expectedChecksum) {
  char calculatedChecksum = tic_extract_checksum_compute(data);
  return (calculatedChecksum == expectedChecksum);
}

/**
 * \fn void tic_extract_parser(void)
 */
__attribute__((optimize(0))) static void tic_extract_parser(const String& str) {

  /************************************************************
   * Etage de contrôle des données recues
   ************************************************************/
  if(str.length() > MAX_BUFFER_SIZE)                                // Overflow de la chaine !
    return;                                                         // Echappement
    
  int spaceIndex = str.indexOf(' ');                                // Index de l'espace entre l'etiquette et la donnee
  int checksumIndex = str.length() - 1;                             // Index du CheckSum
  int dataEndIndex = checksumIndex - 1;                             // Index du dernier caractere de donnee avant CheckSum
  uint8_t i = 0;                                                    // Index de boucle

  if (spaceIndex == -1 || dataEndIndex < spaceIndex)                // Le format est incorrect
        return;                                                     // Echappement

  String label = str.substring(0,spaceIndex);                       // Extraction de l'etiquette
  String data = str.substring(spaceIndex + 1,dataEndIndex);         // Extraction de la donnee
  char checksum = str.charAt(checksumIndex);                        // Extraction du CheckSum

  if (!tic_extract_isCheckSumOK(label + " " + data,checksum))       // La donnee est corrompue !
    return;                                                         // Echappement

  if(!label.length() || !data.length())                             // Il y a parfois de Bugs cote Linky !              
    return;                                                         // Echappement

  if(label.length() > MAX_SIZE_LABEL || data.length() >MAX_SIZE_VALUE)// La chaine est trop longue
    return;                                                         // Echappement

  for(; i < label.length(); i++)                                    // L'etiquette doit etre en MAJUSCULE !
  {
    char c = label.charAt(i);
    if(c < 'A' || c > 'Z' )
      break;
  }
  for(; i < label.length(); i++)                                    // Mais peut contenir des chiffres aussi !
  {
    char c = label.charAt(i);
    if (c < '0' || c > '9')
      return;                                                       // Echappement
  }

  /************************************************************
   * Etage de stockage des données dans la structure TIC_DATA_t
   ************************************************************/
  for (i = 0; i < tic_extract.tic_data->_nb_elem; i++) {                        // Itération sur la table
    if (strcmp(tic_extract.tic_data->etiquette[i], label.c_str()) == 0) {       // L'etiquette existe deja
      if (strcmp(tic_extract.tic_data->valeur[i], data.c_str()) != 0) {         // La valeur est differente
        memset(tic_extract.tic_data->valeur[i],0,MAX_SIZE_VALUE);               // Suppression complète de la valeur
        strncpy(tic_extract.tic_data->valeur[i], data.c_str(), MAX_SIZE_VALUE); // Ecriture de la nouvelle valeur
        ws_server_send_broadcast(i);
      }
      return;
    }
  }

  /*
   * Meme si le checksum est correct, les donnees sur le BUS LInky sont parfois foireuses ! 
   * Exemples après quelques semaines de fonctionnement :
   *    - IINSTP05      avec le 'P'  qui n'a rien a faire la
   *    - PAPPP0780KU2  avec le "KU" qui n'a rien a faire la
   * Ce qui evidement met un bordel terrible dans l'algo agnostique !
   * La startegie consiste a accumuler de nouvelles donnees uniquement dans les premiers instants (timeOutAccumulate),
   * puis d'uniquement mettre à jour celles accumulees precedement !   */
  // Ajout d'une nouvelle donnee (Etiquette, Valeur) dans la table uniquement si l'accumulation est active !
  if (!tic_extract.tic_data->_accumulate && tic_extract.tic_data->_nb_elem < MAX_TIC_DATA) {
    memset(tic_extract.tic_data->etiquette[tic_extract.tic_data->_nb_elem],0,MAX_SIZE_LABEL);               // Suppression complète de l'etiquette
    memset(tic_extract.tic_data->valeur[tic_extract.tic_data->_nb_elem],0,MAX_SIZE_VALUE);                  // Supression complète de la valeur
    strncpy(tic_extract.tic_data->etiquette[tic_extract.tic_data->_nb_elem], label.c_str(), label.length());// Ecriture de l'etiquette
    strncpy(tic_extract.tic_data->valeur[tic_extract.tic_data->_nb_elem], data.c_str(), data.length());     // Ecriture de la valeur
    tic_extract.tic_data->_nb_elem++;                                                                       // Incrementation des données
  }
}

/**
 * \fn void tic_extract_init(TIC_DATA_t *tic)
 */
void tic_extract_init(TIC_DATA_t *tic) {
  Serial.begin(1200,SERIAL_7E1);                                    // Initialisation du port Série

  for(uint8_t t=0;t<tic_extract_nb_timers;t++)
    tic_extract.timers[t] = millis();

  tic_extract.tic_data = tic;
}

/**
 * \fn void tic_extract_handler(void)
 */
void tic_extract_handler(void) {
  static String tmp_buffer_data;
  
  while(Serial.available() > 0)                                     // Tant que de la donnee est disponible sur l'UART
  {
    tic_extract.timers[tic_extract_linkyData] = millis();           // Réarmement du timer inter-trames 
    char data = (char)Serial.read();                                // Lecture de la donnee
    static bool processing = false;                                 // Initialisation du process de sauvegarde
    
    if (data == ETX_CAR_0 || data == STX_CAR_0)                     // Echappement des caractères d'encapsulation protocole (ETX) et (STX)
      continue;
      
    if(data == ETX_CAR_1) {                                         // Caractère de début de chaine, activation de l'enregistrement
      tmp_buffer_data = "";
      processing = true;
    }
    else if (processing) {                                          // L'enregistrement est en cours...
      if (data == STX_CAR_1) {                                      // Caractère de fin de chaine, le traitement peut être effectuer
        tic_extract_parser(tmp_buffer_data);                        // Traitement du parsage
        processing = false;
      } else {                                                      // Accumulation
        if (tmp_buffer_data.length() < MAX_BUFFER_SIZE - 1)
          tmp_buffer_data+=data;
      }
    }
    
    // Gestion de l'accumulation des nouvelles donnees Linky
    if(millis() - tic_extract.timers[tic_extract_accumulate] >= TIMEOUT_ACCUMULATE)
      tic_extract.tic_data->_accumulate = 1;
  }

  // Absence de données TIC en temps impartit, il est nécessaire de réinitialiser le données stockées
  if(millis() - tic_extract.timers[tic_extract_linkyData] >= TIMEOUT_LINKY_DATA) {
    memset(tic_extract.tic_data,0,sizeof(TIC_DATA_t));
  }
}
