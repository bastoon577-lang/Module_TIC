# Module_TIC
Le Module TIC est un équippement fonctionnant sur plateforme [ESP01](https://www.captain-arduino.fr/esp8266-01s/) capable d'extraire les données issues du bornier TIC du compteur Linky et de les partager/offrir
sur un réseau TCP/IP au travers du WIFI et d'un mécanisme WebSocket.

*******
Table des matières
 1. [Concept du Module TIC ?](#concept)
 2. [Mes motivations](#motivation)
 3. [Comment compiler/flasher l'ESP01 ?](#howdoesmoduleticbuild)
*******

<div id='concept'/> 

## Concept du Module TIC
Veuillez consulter le [Wiki du projet](https://github.com/bastoon577-lang/Module-TIC-SOFTWARE/wiki) pour plus d'informations concernant le concept.

<div id='motivation'/> 

## Mes motivations
Aveugle sur la consommation d'énergie de mon foyer, j'ai souhaité intégrer un mécanisme capable de lire en temps réel les données issues du compteur Linky.
Le but étant par la suite d'effectuer l'asservissement des équipements en foncton de l'énergie disponible de l'installation électrique par le délestage :
 1. De la charge de la voiture électrique,
 2. Du ballon d'eau chaude,
 3. De Miner de bitcoin,
 4. etc...
 
 <div id='howdoesmoduleticbuild'/> 

## Comment compiler/flasher l'ESP01 ?
La compilation/téléversement du projet Module TIC implique les étapes suivantes :
 * Installation de [l'IDE Arduino](https://www.arduino.cc/en/software/).
 * Configuration de l'environnement en suivant [ceci](https://fr.macerobotics.com/developpeur/tutoriels/programmer-le-microcontroleur-esp8266-avec-lide-arduino/).
 * Installation ou téléchargement de la librairie [WebSockets_Generic](https://github.com/khoih-prog/WebSockets_Generic).

Enfin, le flashage de l'ESP01 requiert une manipulation expliquée [ici](https://www.diyhobi.com/flash-program-esp-01-using-usb-serial-adapter/), suivez ces instructions, puis téléversez, et le tour et joué.
 
 ###### Auteur : *Sébastien DALIGAULT*. 
