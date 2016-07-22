#pragma once
#include "oxygine-framework.h"
#include "Game.h"
#include "res.h"

using namespace oxygine;

class Game;
DECLARE_SMART(ArmorCount, spArmorCount);
class ArmorCount : public Object {
    
public:
    ArmorCount(Game *game);
    
    /**
     * Renders the on screen gold counter on room changes.
     */
    void render();
    
    /**
     * Updates the string count and sets the on screen gold count.
     *
     * @count is the value to add to the count.
     */
    void updateArmorCount(int value);
    
private:
    Game *_game;
    spTextField _armorCount;
    spSprite _shield;
    int _count;
};
