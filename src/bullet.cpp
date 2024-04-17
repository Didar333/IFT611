/*
* File: bullet.cpp
* Author: Marc-Olivier Bergeron
* Description: Ce fichier contient la classe de l'objet bullet. Permet de
*   dessiner et updater les objets bullet présent dans le jeu.
*/
#include "bullet.h"
#include "engine.h"
#include <cmath>
#include <typeinfo>
#include "config.h"
#include "physics.h"

Bullet::Bullet(int x, int y, float speed, float angle) : Object(x, y, speed)
{
    this->angle = angle;
    this->color = al_map_rgb(255, 0, 0);
    this->hitboxRadius = 5.0f;
}

Bullet::~Bullet() {}

void Bullet::draw()
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, angle);
    al_translate_transform(&transform, x, y);
    al_use_transform(&transform);
    al_draw_line(0.0f, 10.0f, 0.0f, 5.0f, color, 2.0f);
}

void Bullet::update()
{
    dx = speed * sin(angle);
    dy = -speed * cos(angle);
    x += dx;
    y += dy;
}

/*
void Bullet::collide()
{
    // Check for collision with asteroid
    for (size_t i = 0; i < Engine::entities.size(); i++) {
        if (typeid(*Engine::entities[i]) == typeid(Asteroid)) {
            Asteroid* a = dynamic_cast<Asteroid*>(Engine::entities[i]);
            // Collision occured
            if (physics::isPointsInsideCercle(a->x, a->y, a->hitboxRadius, this->x, this->y)) {
                // Attribution des points ainsi que split les asteroids
                if (a->size == Asteroid::SMALL)
                    Engine::score += 100;
                else if (a->size == Asteroid::MEDIUM) {
                    Engine::score += 50;
                    a->split();
                }
                else {
                    Engine::score += 20;
                    a->split();
                }
                // Détruit l'asteroid ainsi que la bullet
                Engine::removeList.push_back(
                    std::find(Engine::entities.begin(), Engine::entities.end(), this));
                Engine::removeList.push_back(
                    std::find(Engine::entities.begin(), Engine::entities.end(), a));
                // Aucune raison de continuer à continuer dans la boucle for
                return;
            }
        }
    }
}
*/