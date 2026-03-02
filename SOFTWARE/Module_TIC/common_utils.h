#ifndef __COMMON_UTILS__
#define __COMMON_UTILS__

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <stdint.h>

//< MACRO de version logicielle
#define V_LOGICIEL                "v0.1"                // Version Logicielle

//< MACRO du Mode Access Point
#define AP_SSID                   "ModuleTIC"           // SSID ModuleTIC Access Point
#define AP_PASS                   AP_SSID               // Password ModuleTIC Access Point
#define AP_CHANNEL                0                     // Channel ModuleTIC Access Point
#define AP_VISIBILITE             false                 // Visibilite ModuleTIC Access Point
#define AP_MAX_CONN               1                     // Nombre de connexion ModuleTIC Access Point
#define AP_TCP_PORT               8082                  // Port TCP ModuleTIC Access Point

//< Define des In/Out
#define GPIO_DMD_RESET            0                     // GPIO du bouton Reset
#define GPIO_LED_IHM              2                     // GPIO de la LED

//< Define des Timeout
#define TIMEOUT_SCAN_NETWORK      300000                // Temps de scrutation des réseaux Wifi disponibles (5 Minutes)
#define TIMEOUT_REBOOT            1000                  // Temps d'anti-rebond pour le reboot (1 Seconde)
#define TIMEOUT_LED_LENT          500                   // Clignotement lent de la LED
#define TIMEOUT_LED_RAPI          50                    // Clignotement rapide de la LED

//< Define des données TIC
#define MAX_BUFFER_SIZE           50                    // Taille du buffer d'accumation
#define MAX_TIC_DATA              26                    // Nombre maximum de données TIC
#define MAX_SIZE_LABEL            9                     // Taille maximum de l'étiquette TIC
#define MAX_SIZE_VALUE            13                    // Taille maximum de la valeur TIC

//< Structure de Configurations fonctionnelles figées (Configurable uniquement à la configuration de l'équippement)
typedef struct
{
  char      Hostname[65];                               // Hostname
  char      SmSsid[33];                                 // SSID
  char      SmPass[65];                                 // Password
  uint8_t   address[4];                                 // Adresse IP Static
  uint8_t   subnet[4];                                  // Masque de sous reseau
  uint8_t   gateway[4];                                 // Passerelle
  uint16_t  port;                                       // Port de service Web
  uint16_t  portWs;                                     // Port de service WebSocket
  uint8_t   is_configured         : 1;                  // Bitfield indiquant que l'équippement est configuré
  uint8_t   is_wifi_network_used  : 1;                  // Bitfield indiquant un accrochage sur une borne Wifi
  uint8_t   RUF                   : 6;                  // Reservé Usage Future
} STATIC_CONF_FIELDS_t;

//< Structure de Configurations fonctionnelles qui peuvent evoluer dans le temps (Configurable à la volé dans l'onglet d'exploitation)
typedef struct
{
  uint8_t   theme                 :1;                   // Theme (0 : fonce & 1 : clair)
  uint8_t   RUF                   :7;                   // Reserve Usage Future
} VOLATILE_CONF_FIELDS_t;

//< Structure des donnees TIC
typedef struct
{
  uint8_t   _accumulate           :1;                   // Variable d'accumulation (0 : Actif, 1 : Innactif)
  uint8_t   _nb_elem              :7;                   // Nombre d'elements courants dans la table
  char      etiquette[MAX_TIC_DATA][MAX_SIZE_LABEL];    // Etiquettes (ADCO,ISOUSC,IMAX,...)
  char      valeur[MAX_TIC_DATA][MAX_SIZE_VALUE];       // Valeurs liée aux étiquettes
} TIC_DATA_t;

//< Enumération des types de sauvegardes en mémoire EEPROM
enum EepromDataType {
  EEPROM_STATIC_CFG,                                    // Configuration des données STATIC
  EEPROM_VOLATILE_CFG                                   // Configuration des données VOLATILE
};

#endif
