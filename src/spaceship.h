#ifndef SPACESHIP_FILE
#define SPACESHIP_FILE

#include "object.h"

// Foward declaration for fire
class Bullet;

class Spaceship : public Object
{
public:
    Spaceship(float x, float y, float speed);
    ~Spaceship();
    void draw();
    void update();
    void accelerate();
    void moveLeft();
    void moveRight();
    void hyperspace();
    Bullet* fire();

    int lives;
    float hitboxradius;
   //bool invinsible;
    //int age;
private:
    const float THRUST = 0.1f;
    const float MAXSPEED = 4.0f;
    const float TURNSPEED = 0.05f;
};

#endif
