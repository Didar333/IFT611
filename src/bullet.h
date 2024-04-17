#ifndef BULLET_FILE
#define BULLET_FILE

#include "object.h"

class Bullet : public Object
{
public:
    Bullet(int x, int y, float speed, float angle);
    ~Bullet();
    void draw();
    void update();
};
#endif