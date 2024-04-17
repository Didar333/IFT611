/*
* File: main.cpp
* Author: Marc-Olivier Bergeron
* Description:
*   Boucle principal de l'application. S'occupe de la gestion des events ainsi
*   que de l'initialisation de Allegro.
*/
#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "engine.h"
#include "config.h"


bool initAllegro(void);

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_TIMER* frame_timer = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;


/****************
* MAIN
*****************/
int main(int argc, char** argv)
{
    if (!initAllegro()) {
        // failed to init
        return -1;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    Engine engine(event_queue);
    bool exit = false;
    bool redraw = false;

    engine.startGame();
    al_start_timer(frame_timer);

    while (!exit) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            engine.gereKeyboard();
            engine.update();
            //engine.redraw();
        }         
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || event.type == QUIT_EVENT) {
            break;
        }
    }
    al_destroy_display(display);
    al_destroy_timer(frame_timer);
    return 0;
}

/******************************************************************************
* Fonction: initAllegro()
* Author: Marc-Olivier Bergeron
* Description:
*   Cette fonction va créer les éléments de Allegro (display, timer, etc)
*   nécessaire pour le fonctionnement du jeu et va s'assurer que les éléments
*   ont bien été initialisés.
******************************************************************************/
bool initAllegro()
{
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "failed to initialize primitive!\n");
        return false;
    }

    // For keyboard
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return false;
    }

    // Create timer for clock
    frame_timer = al_create_timer(1.0 / FPS);
    if (!frame_timer) {
        fprintf(stderr, "failed to create timer!\n");
        return false;
    }

    // Create display
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(frame_timer);
        return false;
    }

    // Create event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(frame_timer);
        return false;
    }

    al_init_font_addon();
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "failed to init ttf addon!\n");
        al_destroy_display(display);
        al_destroy_timer(frame_timer);
        return false;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(frame_timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0, 0, 0));
    return true;
}