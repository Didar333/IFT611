/*
* File: asteroid.cpp
* Author: Marc-Olivier Bergeron
* Description: Classe de l'objet Asteroid. Permet de dessiner et updater
*   les objets asteroids présent dans le jeu.
*/
#include "asteroid.h"
#include <cmath>
#include "config.h"
#include "engine.h"

#define ASTEROID_VERTICES 14

Asteroid::Asteroid(float x, float y, float speed, float angle, int size) : Object(x, y, speed)
{
    this->angle = angle;
    this->size = size;
    if (size == BIG)
        hitboxRadius = 50.0f;
    else if (size == MEDIUM)
        hitboxRadius = 30.0f;
    else
        hitboxRadius = 15.0f;
    this->color = al_map_rgb(148, 148, 148);
}

Asteroid::~Asteroid() {}

void Asteroid::draw()
{
    float asteroidVertices[ASTEROID_VERTICES][2] = {
        {0.0f, -15.0f}, {20.0f, -5.0f},
        {20.0f, -5.0f}, {20.0f, 10.0f},
        {20.0f, 10.0f}, {10.0f, 20.0f},
        {10.0f, 20.0f}, {-5.0f, 20.0f},
        {-5.0f, 20.0f}, {-20.0f, 10.0f},
        {-20.0f, 10.0f}, {-20.0f, -5.0f},
        {-20.0f, -5.0f}, {0.0f, -15.0f}
    };

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_translate_transform(&transform, x, y);
    al_use_transform(&transform);

    float scale = 0.0f;
    if (size == SMALL)
        scale = 0.7f;
    else if (size == MEDIUM)
        scale = 1.7f;
    else
        scale = 2.7f;

    for (int i = 0; i < ASTEROID_VERTICES - 1; i++) {
        al_draw_line(asteroidVertices[i][0] * scale, asteroidVertices[i][1] * scale,
            asteroidVertices[i + 1][0] * scale, asteroidVertices[i + 1][1] * scale, this->color, 2);
    }
}

void Asteroid::update()
{
    dx = speed * cos(angle);
    dy = -speed * sin(angle);
    x += dx;
    y += dy;
    // wraparound
    if (x > SCREEN_WIDTH + size * 20)
        x = -size * 20;
    else if (x < -size * 20)
        x = SCREEN_WIDTH + size * 20;
    if (y > SCREEN_HEIGHT + size * 20)
        y = -size * 20;
    else if (y < -size * 20)
        y = SCREEN_HEIGHT + size * 20;
}

void Asteroid::split(std::vector<Asteroid*>& a)
{
    if (size == BIG) {
        float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
        float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
        a.push_back(new Asteroid(x, y, ASTEROID_SPEED, angle1, MEDIUM));
        a.push_back(new Asteroid(x, y, ASTEROID_SPEED, angle2, MEDIUM));
    }
    else if (size == MEDIUM) {
        float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
        float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
        a.push_back(new Asteroid(x, y, ASTEROID_SPEED, angle1, SMALL));
        a.push_back(new Asteroid(x, y, ASTEROID_SPEED, angle2, SMALL));
    }
    else
        return; // TODO throw exception here
}