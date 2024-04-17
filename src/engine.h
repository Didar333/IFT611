#ifndef ENGINE_FILE
#define ENGINE_FILE


// Custome event pour Allegro
// Ceci sert d'identifiant pour les customs events
// voir https://liballeg.org/a5docs/trunk/events.html#allegro_get_event_type
#define BASE_EVENT ALLEGRO_GET_EVENT_TYPE('T', 'S', 'F', 'G')
#define QUIT_EVENT (BASE_EVENT + 0)

#include <vector>
#include <list>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "object.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"

class Engine {
public:
    Engine(ALLEGRO_EVENT_QUEUE* event_queue);
    ~Engine();
    void startGame();
    void resetGame();
    void nextLevel();
    void update();
    void drawHud();
    void playerDead();
    void gameOver();
    void gereKeyboard();

    Spaceship* player;
    std::vector<Asteroid*> asteroids;
    std::vector<Bullet*> bullets;

private:
    void cleanup();

    // Indicated if debug mode is activated
    bool debug;

    // Current level
    int level;

    // Score of player
    int score;

    // Keys trigger for keyboard.
    bool pressed_keys[ALLEGRO_KEY_MAX];

    // Use for saving and checking keyboard state
    ALLEGRO_KEYBOARD_STATE keyboardState;
    ALLEGRO_EVENT_SOURCE engine_event_source;
    ALLEGRO_FONT* font;
};
#endif

