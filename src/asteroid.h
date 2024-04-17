#ifndef ASTEROID_FILE
#define ASTEROID_FILE

#include "object.h"
#include <vector>

class Asteroid : public Object
{
public:
    Asteroid(float x, float y, float speed, float angle, int size);
    ~Asteroid();
    void draw();
    void update();
    void split(std::vector<Asteroid*>& a);

    int size;
    enum SIZE {
    SMALL = 1, MEDIUM = 2, BIG = 3
};
private:
    const float ASTEROID_SPEED = 2.0f;
};

#endif