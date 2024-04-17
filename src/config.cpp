/*
* File: config.cpp
* Author: Marc-Olivier Bergeron
* Description:
*   Ce fichier contient la configuration générale de l'application.
*/
#include "config.h"

// Display resolution
const int SCREEN_WIDTH = 1024;  // x
const int SCREEN_HEIGHT = 768;  // y

// Graphic config
const float FPS = 60;
const float FRAME_TIME = 1 / FPS;

// Game config
const int NUMBER_OF_PLAYER = 1;     // Pour Multiplayer (not used for now)
const int STARTING_LIVES = 2;       // Nombre de vie au lancement du jeu
const int INVINSIBILITY_TIME = 2;   // temps d'invincibilité après avoir perdu une vie