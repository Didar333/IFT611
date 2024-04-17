/*
* File: starship.cpp
* Author: Marc-Olivier Bergeron
* Description:
*   Ce fichier contient la classe de l'objet spaceship. Permet de dessiner et
*   updater les objets spaceships présent dans le jeu.
*/
#include "spaceship.h"
#include <cmath>
#include <typeinfo>
#include "bullet.h"
#include "config.h"
#include "engine.h"
#include "physics.h"

#define SPACESHIP_VERTICES 8

Spaceship::Spaceship(float x, float y, float speed) : Object(x, y, speed)
{
    this->angle = 0.0f;
    this->color = al_map_rgb(255, 255, 255);
    this->lives = STARTING_LIVES;
    this->hitboxradius = 15.0f;
}

Spaceship::~Spaceship() {}

void Spaceship::draw()
{
    float spaceshipVertices[SPACESHIP_VERTICES][2] = {
    {0.0f, -11.0f},{-8.0f, 9.0f},
    {-8.0f, 9.0f}, {0.0f, 5.0f},
    {0.0f, 5.0f}, {8.0f, 9.0f},
    {8.0f, 9.0f}, {0.0f, -11.0f}
    };

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, angle);
    al_translate_transform(&transform, x, y);
    al_use_transform(&transform);

    float scale = 1.2f;
    for (int i = 0; i < SPACESHIP_VERTICES - 1; i++) {
        al_draw_line(spaceshipVertices[i][0] * scale, spaceshipVertices[i][1] * scale,
            spaceshipVertices[i + 1][0] * scale, spaceshipVertices[i + 1][1] * scale, this->color, 3.0f);
    }
}

void Spaceship::update()
{
    this->x += dx;
    this->y += dy;

    // wraparound
    if (x > SCREEN_WIDTH + 1.5f * 20)
        x = -1.5f * 20;
    else if (x < -1.5f * 20)
        x = SCREEN_WIDTH + 1.5f * 20;
    if (y > SCREEN_HEIGHT + 1.5f * 20)
        y = -1.5f * 20;
    else if (y < -1.5f * 20)
        y = SCREEN_HEIGHT + 1.5f * 20;
}

void Spaceship::accelerate()
{
    dx += THRUST * sin(angle);
    dy += -THRUST * cos(angle);
    speed = sqrtf((dx * dx) + (dy * dy));  // sqrt(dx^2 + dy^2)
    if (speed > MAXSPEED) {
        dx *= MAXSPEED / speed;
        dy *= MAXSPEED / speed;
    }
}

void Spaceship::moveLeft()
{
    this->angle -= TURNSPEED;
}

void Spaceship::moveRight()
{
    this->angle += TURNSPEED;
}

void Spaceship::hyperspace()
{
    this->speed = 0;
    this->dx = 0;
    this->dy = 0;
    this->x = rand() % SCREEN_WIDTH;
    this->y = rand() % SCREEN_HEIGHT;
}

Bullet* Spaceship::fire()
{
    // Décale la bullet pour qu'elle spawn sur la pointe du vaisseau (pt (0.0f, -11.0f))
    float x = this->x + 11.0f * sin(this->angle);
    float y = this->y - 11.0f * cos(this->angle);
    Bullet* b = new Bullet(x, y, 7.0f, this->angle);
    return b;
}
