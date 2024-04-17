/*
* File: physics.cpp
* Author: Marc-Olivier Bergeron
* Description:
*   Ce fichier s'occupe de la logique des collisions entre les objets. Cet algorithme 
*   fonctionne en prenant le point central de deux cercles puis il vérifie que la 
*   distance entre ces deux points est inférieure à la somme des rayons de ces deux cercles.
*   Voir https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
*/
#include "physics.h"
#include <math.h>

bool physics::isPointsInsideCercle(Object* o1, Object* o2)
{
    float radius = o1->hitboxRadius + o2->hitboxRadius;
    // true = collision and false = no collision
    return sqrtf((o2->x - o1->x) * (o2->x - o1->x) + (o2->y - o1->y) * (o2->y - o1->y)) < radius;
}