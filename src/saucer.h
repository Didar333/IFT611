#ifndef SAUCER_FILE
#define SAUCER_FILE

#include "object.h"

class Saucer : public Object
{
public:
    Saucer(int x, int y, float speed);
    ~Saucer();
    void Draw();
    bool Update();
private:
};
#endif