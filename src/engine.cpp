/*
* File: engine.cpp
* Author: Marc-Olivier Bergeron
* Description: S'occupe du déroulement du jeu allant à la gestion du clavier
*   jusqu'au redraw des objets dans le jeu.
*/
#include "engine.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include "config.h"
#include "physics.h"


Engine::Engine(ALLEGRO_EVENT_QUEUE* event_queue)
{
    debug = false;
    score = 0;
    level = 1;

    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        pressed_keys[i] = false;
    }

    player = NULL;

    // For Keyboard
    al_get_keyboard_state(&keyboardState);

    // For custome event from engine to event_queue in main
    al_init_user_event_source(&engine_event_source);
    al_register_event_source(event_queue, &engine_event_source);

    font = al_load_ttf_font("assets/Font.ttf", 20, 0);
}

Engine::~Engine() 
{
    cleanup();
    al_destroy_font(font);
}

void Engine::startGame()
{
    score = 0;
    level = 1;
    // Create player
    player = new Spaceship(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    resetGame();
}

void Engine::resetGame()
{
    player->x = SCREEN_WIDTH / 2;
    player->y = SCREEN_HEIGHT / 2;
    player->angle = 0.0f;
    player->dx = 0.0f;
    player->dy = 0.0f;
    for (size_t i = 0; i < 6; i++) {
        float x = rand() % (SCREEN_WIDTH - 20) + 20;
        float y = rand() % (SCREEN_HEIGHT - 20) + 20;
        float angle = ((float)rand() / (float)RAND_MAX) * 6.283185f;
        float size = rand() % 3 + 1;
        asteroids.push_back(new Asteroid(x, y, 2.3f, angle, size));
    }
}

void Engine::update()
{
    // Clear screen
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (player != NULL) {
        player->update();
        player->draw();
        // Check for collision player vs asteroid
        for (auto& a : asteroids) {
            if (physics::isPointsInsideCercle(a, player)) {
                playerDead();
                return;
            }
        }
    }

    for (auto& a : asteroids) {
        a->update();
        a->draw();
    }

    std::vector<Asteroid*> temp_a;

    for (auto& b : bullets) {
        b->update();
        b->draw();
        // Check for collision with asteroids
        for (auto& a : asteroids) {
            if (physics::isPointsInsideCercle(a, b)) {
                // Asteroid hit
                // Add score
                if (a->size == Asteroid::BIG)
                    score += 20;
                else if (a->size == Asteroid::MEDIUM)
                    score += 50;
                else
                    score += 100;
                // erase bullet
                bullets.erase(std::remove(bullets.begin(), bullets.end(), b));
                // split asteroid
                if(a->size > Asteroid::SMALL)
                    a->split(temp_a);
                auto i = std::remove(asteroids.begin(), asteroids.end(), a);
                if(i != asteroids.end())
                    asteroids.erase(i);
            }
        }
        // Bullet is beyond the display, remove it
        if (b->x * b->y < 0 || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT) {
            // erase bullet
            bullets.erase(std::remove(bullets.begin(), bullets.end(), b));
        }
    }
    for(auto a : temp_a)
        asteroids.push_back(a);

    if (asteroids.empty() && debug == false)
        resetGame();

    drawHud();
    al_flip_display();
}

void Engine::drawHud()
{ 
    // Drawing lives
    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
    ALLEGRO_TRANSFORM transform;
    if (player != NULL)
    {
        for (int i = 0; i < player->lives; i++) {
            al_identity_transform(&transform);
            //al_rotate_transform(&transform, 0);
            al_translate_transform(&transform, 20 * (i + 1), 20);
            al_use_transform(&transform);
            al_draw_line(-8, 9, 0, -11, color, 3.0f);
            al_draw_line(0, -11, 8, 9, color, 3.0f);
            al_draw_line(8, 9, 0, 5, color, 3.0f);
            al_draw_line(0, 5, -8, 9, color, 3.0f);
        } 
    }
    // Drawing Score
    al_identity_transform(&transform);
    //al_rotate_transform(&transform, 0);
    al_translate_transform(&transform, 0, 0);
    al_use_transform(&transform);

    char buffer[40];
    sprintf_s(buffer, "Score: %d", score);
    al_draw_text(font, al_map_rgb(255, 255, 255),
        SCREEN_WIDTH - 60, 20, ALLEGRO_ALIGN_RIGHT,
        buffer);
    // Debug
    if (debug == true)
    {
        sprintf_s(buffer, "debug");
        al_draw_text(font, al_map_rgb(255, 255, 255),
            SCREEN_WIDTH - 40, 700, ALLEGRO_ALIGN_RIGHT,
            buffer);
    }
}

void Engine::playerDead() 
{
    if (debug == false)
    {
        // sleep for 3 secondes
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (player->lives == 0) {
            this->cleanup();
            this->gameOver();
        }
        else {
            player->lives--;
            asteroids.clear();
            bullets.clear();
            resetGame();
        }
    }
}

void Engine::gameOver()
{
    // Clear screen
    al_clear_to_color(al_map_rgb(0, 0, 0));

    drawHud();

    char buffer[40];
    font = al_load_ttf_font("assets/Font.ttf", 40, 0);

    sprintf_s(buffer, "GameOver");
    al_draw_text(font, al_map_rgb(255, 255, 255),
        SCREEN_WIDTH /2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER,
        buffer);

    font = al_load_ttf_font("assets/Font.ttf", 30, 0);

    sprintf_s(buffer, "Press escape to quit");
    al_draw_text(font, al_map_rgb(255, 255, 255),
        SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTER,
        buffer);

    al_flip_display();
    do {
        // Wait for Escape or Space to be pressed
        al_get_keyboard_state(&keyboardState);
    } while (!al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE));
    if (!al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) {
        ALLEGRO_EVENT customEvent;
        customEvent.type = QUIT_EVENT;
        al_emit_user_event(&engine_event_source, &customEvent, NULL);
    }
}

void Engine::gereKeyboard()
{
    // Save new keyboard state
    al_get_keyboard_state(&keyboardState);

    // W is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_W)) {
        player->accelerate();
    }

    // A is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_A)) {
        player->moveLeft();
    }

    // D is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_D)) {
        player->moveRight();
    }

    // SPACE is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_SPACE)) {
        if (!pressed_keys[ALLEGRO_KEY_SPACE]) {
            Bullet* b = player->fire();
            bullets.push_back(b);
            pressed_keys[ALLEGRO_KEY_SPACE] = true;
        }
    }
    else
        pressed_keys[ALLEGRO_KEY_SPACE] = false;

    // Q is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_Q)) {
        if (!pressed_keys[ALLEGRO_KEY_Q]) {
            player->hyperspace();
            pressed_keys[ALLEGRO_KEY_Q] = true;
        }
    }
    else
        pressed_keys[ALLEGRO_KEY_Q] = false;

    // ESCAPE is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) {
        ALLEGRO_EVENT customEvent;
        customEvent.type = QUIT_EVENT;
        al_emit_user_event(&engine_event_source, &customEvent, NULL);
    }
    // R is pressed
    if (al_key_down(&keyboardState, ALLEGRO_KEY_R)) {
        if (!pressed_keys[ALLEGRO_KEY_R]) {
            if (!debug) {
                debug = !debug;
                cleanup();
            }
            else {
                debug = !debug;
                cleanup();
                startGame();
            }

        }
        pressed_keys[ALLEGRO_KEY_R] = true;
    }
    else
        pressed_keys[ALLEGRO_KEY_R] = false;

    /////////////////////////
    // FOR DEBUG
    /////////////////////////
    if (debug == true) {
        // To spawn spaceship
        if (al_key_down(&keyboardState, ALLEGRO_KEY_T)) {
            if (!pressed_keys[ALLEGRO_KEY_T]) {
                if (player == NULL) {
                    Spaceship* s = new Spaceship(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
                    player = s;
                    pressed_keys[ALLEGRO_KEY_T] = true;
                }
            }
        }
        else
            pressed_keys[ALLEGRO_KEY_T] = false;

        // To spawn a big asteroid
        if (al_key_down(&keyboardState, ALLEGRO_KEY_Y)) {
            if (!pressed_keys[ALLEGRO_KEY_Y]) {
                Asteroid* a = new Asteroid(
                    SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2.3f, 0.0f, Asteroid::SIZE::BIG);
                asteroids.push_back(a);
                pressed_keys[ALLEGRO_KEY_Y] = true;
            }
        }
        else
            pressed_keys[ALLEGRO_KEY_Y] = false;

        // To spawn a medium asteroid
        if (al_key_down(&keyboardState, ALLEGRO_KEY_U)) {
            if (!pressed_keys[ALLEGRO_KEY_U]) {
                Asteroid* a = new Asteroid(
                    SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2.3f, ALLEGRO_PI/2, Asteroid::SIZE::MEDIUM);
                asteroids.push_back(a);
                pressed_keys[ALLEGRO_KEY_U] = true;
            }
        }
        else
            pressed_keys[ALLEGRO_KEY_U] = false;

        // To spawn a small asteroid
        if (al_key_down(&keyboardState, ALLEGRO_KEY_I)) {
            if (!pressed_keys[ALLEGRO_KEY_I]) {
                Asteroid* a = new Asteroid(
                    SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2.3f, (3*ALLEGRO_PI/2), Asteroid::SIZE::SMALL);
                asteroids.push_back(a);
                pressed_keys[ALLEGRO_KEY_I] = true;
            }
        }
        else
            pressed_keys[ALLEGRO_KEY_I] = false;

        // To spawn a saucer      
        if (al_key_down(&keyboardState, ALLEGRO_KEY_O)) {
            if (!pressed_keys[ALLEGRO_KEY_O]) {
                // TODO spawn saucer
                pressed_keys[ALLEGRO_KEY_O] = true;
            }
        }
        else
            pressed_keys[ALLEGRO_KEY_O] = false;
    }
}

void Engine::cleanup()
{
    delete player;
    player = NULL;
    asteroids.clear();
    bullets.clear();
}