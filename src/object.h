#ifndef OBJECT_FILE
#define OBJECT_FILE

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

class Object
{
public:
    Object(float x, float y, float speed)
    {
        this->x = x;
        this->y = y;
        this->dx = 0.0f;
        this->dy = 0.0f;
        this->speed = speed;
        this->angle = 0.0f;
        this->color = al_map_rgb(0, 0, 0);
        this->hitboxRadius = 0;
    }
    virtual ~Object() {};
    virtual void draw() =0;
    virtual void update() =0;

//protected:
    float x;
    float y;
    float dx;
    float dy;
    float speed;
    float angle;
    float hitboxRadius;
    ALLEGRO_COLOR color;
};
#endif
