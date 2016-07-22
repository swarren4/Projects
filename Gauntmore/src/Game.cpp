#include <iostream>
#include <list>

#include "tmx/Tmx.h"

#include "Game.h"
#include "res.h"
#include "KeyboardInput.h"

#include "Player.h"

#include "Creature.h"

#include "Slime.h"
#include "Snake.h"
#include "Bat.h"
#include "Worm.h"
#include "Eyeball.h"
#include "Ghost.h"

#include "Chest.h"
#include "Gold.h"
#include "Potion.h"
#include "Armor.h"
#include "Weapon.h"

#include "Map.h"

#include "HealthBar.h"
#include "GoldCount.h"
#include "ArmorCount.h"
#include "WeaponCount.h"

#include "PauseMenu.h"
#include "StartMenu.h"
#include "DeathMenu.h"


/**
 * Constructor.
 */
Game::Game():_isPaused(false), _isFirstRun(true) {
    // Set the size of the scene to the size of the display.
    setSize(getStage()->getSize());

    _createMap();

    _createUnits();

    _createHud();

    // Keyboard handler
    _move = new KeyboardInput(this);
}


/**
 * Destructor.
 */
Game::~Game() {}


/**
 * Switches viewable map to the room in the direction of the given edge in the maze.
 *
 * @edge is the edge the player existed on.
 */
void Game::switchRoom(int edge) {
    // Change to a new room in the maze.
    _map->changeRoom(edge);
    _renderMap();

    // Get player entrance position
    int playerCol = 1;
    int playerRow = 1;

    switch (edge) {
        case 0: // top
            playerCol = _map->getRoom()->getBottom() * tileSize;
            playerRow = (_map->getRoom()->getSize() - 2) * tileSize;
            break;
        case 1: // right
            playerCol = tileSize;
            playerRow = _map->getRoom()->getLeft() * tileSize;
            break;
        case 2: // bottom
            playerCol = _map->getRoom()->getTop() * tileSize;
            playerRow = tileSize;
            break;
        case 3: // left
            playerCol = (_map->getRoom()->getSize() - 2) * tileSize;
            playerRow = _map->getRoom()->getRight() * tileSize;
            break;
        default:
            break;
    }

    // Setup units
    list<spUnit> *units = _map->getRoom()->getUnits();
    for (list<spUnit>::iterator i = units->begin(); i != units->end(); ) {
        spUnit unit = *i;
        if (unit->getType() == "player") {
            i = units->erase(i);
        } else {
            unit->init(unit->getLocation(), this);
            ++i;
        }
    }
    
    // Setup player
    _player->attachUnit();
    _player->addSprite();
    _player->setPosition(Vector2(playerCol, playerRow));
    _map->getRoom()->getUnits()->push_back(_player);
    
    // Update UI
    _healthBar->render();
    _goldCount->render();
    _armorCount->render();
    _weaponCount->render();

    _createCollisionRects();
}


/**
 * Checks if the given position is an exit. Used to switch rooms.
 *
 * @position is the position to check.
 */
bool Game::isExit(Vector2 position) {
    bool isExit = false;
    int size = getMap()->getRoom()->getSize() * tileSize;
    
    if (!(position.x > 0 && position.x < size - tileSize && position.y > 0 && position.y < size - tileSize)) {
        isExit = true;
        int edge = 0;
        
        if (position.y <= 0) {
            // top
            edge = 0;
        } else if (position.x >= size - tileSize) {
            // right
            edge = 1;
        } else if (position.y >= size - tileSize) {
            // bottom
            edge = 2;
        } else if (position.x <= 0) {
            // left
            edge = 3;
        }
        
        switchRoom(edge);
    }
    
    return isExit;
}


/**
* Updates the health bar.
*
* @num is the value to update by.
*/
void Game::updateHealth(double value) {
    _healthBar->updateHealth(value);
}


/**
* Updates the gold counter.
*
* @num is the value to update by.
*/
void Game::updateGoldCount(int value) {
    _goldCount->updateGoldCount(value);
}

/**
 * Updates the armor counter.
 *
 * @num is the value to update by.
 */
void Game::updateArmorCount(int value) {
    _armorCount->updateArmorCount(value);
}

/**
 * Updates the weapon counter.
 *
 * @num is the value to update by.
 */
void Game::updateWeaponCount(int value) {
    _weaponCount->updateWeaponCount(value);
}


/**
 * Pauses the game when switching to the pause menu.
 */
void Game::pauseGame() {
    setPaused(true);
    getClock()->pause();
    PauseMenu::instance->setGame(this);
    PauseMenu::instance->show();
}


/**
 * Pauses the game when switching to the start menu.
 */
void Game::startGame() {
    setFirstRun(false);
    setPaused(true);
    getClock()->pause();
    StartMenu::instance->show();
}


/**
 * Pauses the game when switching to the death menu.
 */
void Game::killPlayer() {
    setPaused(true);
    getClock()->pause();
    DeathMenu::instance->setGame(this);
    DeathMenu::instance->show();
}


/**
 * Creates a new game on player death.
 */
void Game::createNewGame() {
    _createMap();
    
    _createUnits();
    
    _createHud();
    
    setPaused(false);
    
    // Keyboard handler
    _move->resetDir();
}


/**
 * Gets the player.
 */
spPlayer Game::getPlayer() {
    return _player;
}


/**
 * Gets the map.
 */
spMap Game::getMap() {
    return _map;
}


/**
 * Gets the list of collision rects.
 */
vector<SDL_Rect> Game::getTiles() {
    return _tiles;
}


/**
 * Gets the keyboard input handler.
 */
spKeyboardInput Game::getMove() {
    return _move;
}


/**
 * Gets the keyboard input handler.
 */
spHealthBar Game::getHealthBar() {
    return _healthBar;
}

/**
 * Gets the tile size.
 */
int Game::getTileSize() {
    return tileSize;
}


/**
 * Sets the game state to paused.
 */
void Game::setPaused(bool isPaused) {
    _isPaused = isPaused;
}


/**
 * Checks if the game is paused.
 */
bool Game::isPaused() {
    return _isPaused;
}


/**
 * Sets the games state to first run.
 */
void Game::setFirstRun(bool isFirstRun) {
    _isFirstRun = isFirstRun;
}


/**
 * Checks if this is th games first run.
 */
bool Game::isFirstRun() {
    return _isFirstRun;
}


/**
 * Gets the list of chest contents.
 */
list<spThing>* Game::getContentsList() {
    return &_things;
}


/**
 * Updates the Units each frame. A virtual method of Actor called each frame.
 *
 * @us is the UpdateStatus sent by the global update method.
 */
void Game::doUpdate(const UpdateState &us) {
    if (isFirstRun()) {
        startGame();
    }
    
    list<spUnit> *units = _map->getRoom()->getUnits();
    // Iterate through the unit list and call their update method. Then check for death.
    for (list<spUnit>::iterator i = units->begin(); i != units->end(); ) {
        spUnit unit = *i;
        unit->update(us);
        if (unit->isDead()) {
            // If it is dead remove it from list.
            i = units->erase(i);
        } else {
            ++i;
        }
    }
    
    if (_player->isDead()) {
        killPlayer();
    }

    _createCollisionRects();
}


/**
 * Creates the map, renders it, and creates the collision list.
 */
void Game::_createMap() {
    // Size is the number of tiles
    int size = 13;
    
    // Generate things for chests
    _things.push_back(new Armor(2));
    _things.push_back(new Armor(3));
    _things.push_back(new Weapon(2));
    _things.push_back(new Weapon(3));
    
    for (int i = 0; i < 38; i++) {
        _things.push_back(new Gold);
        _things.push_back(new Potion);
        
    }
    
    // Create map
    _map = new Map(size);
    _renderMap();
    _createCollisionRects();
}

/**
 * Create the creatures and the player.
 */
void Game::_createUnits() {
    // Create Creatures and Things
    list<spUnit> *units = _map->getRoom()->getUnits();
    for (list<spUnit>::iterator i = units->begin(); i != units->end(); ++i) {
        spUnit unit = *i;
        if (unit->getType() != "player") {
            unit->init(unit->getLocation(), this);
        }
    }
    
    // Create player
    _player = new Player(40, 1, 1);
    _player->init(_getEntrance(), this);
    _map->getRoom()->getUnits()->push_back(_player);
}


/**
 * Create the health, gold, defense, and attack HUD elements.
 */
void Game::_createHud() {
    // Health bar
    _healthBar = new HealthBar(this);
    
    // Gold count
    _goldCount = new GoldCount(this);
    
    // Armor Count
    _armorCount = new ArmorCount(this);
    
    // Weapon Count
    _weaponCount = new WeaponCount(this);
}


/**
 * Reads the tile map description from the .tmx file and uses it to render the map.
 */
void Game::_renderMap() {
    _tileMap = new Tmx::Map();
    
    _tileMap->ParseFile("tmx/room.tmx");
    
    for (int i = 0; i < _tileMap->GetNumLayers(); ++i) {
        // Get a layer.
        const Tmx::Layer *layer = _tileMap->GetLayer(i);
        for (int x = 0; x < layer->GetWidth(); ++x) {
            for (int y = 0; y < layer->GetHeight(); ++y) {
                int tilesetIndex = layer->GetTileTilesetIndex(x, y);
                const Tmx::Tileset *tileset = _tileMap->GetTileset(tilesetIndex);
                std::string name = tileset->GetName();
                int tileSize = tileset->GetImage()->GetWidth();
                int drawX = x * tileSize;
                int drawY = y * tileSize;
                // Draw the tile.
                spSprite sprite = new Sprite;
                sprite->setResAnim(resources.getResAnim(name));
                sprite->setX(drawX);
                sprite->setY(drawY);
                sprite->attachTo(this);
                sprite->setAnchor(Vector2(0.5f, 0.5f));
            }
        }
    }
}


/**
 * Creates a vector of rectangles called tiles that is used to detect collisions.
 */
void Game::_createCollisionRects() {
    _tiles.clear();
    // Build a vector of rectangles to represent the collidable tiles.
    for (int i = 0; i < _tileMap->GetNumLayers(); ++i) {
        // Get a layer.
        const Tmx::Layer *layer = _tileMap->GetLayer(i);
        for (int x = 0; x < layer->GetWidth(); ++x) {
            for (int y = 0; y < layer->GetHeight(); ++y) {
                
                int tilesetIndex = layer->GetTileTilesetIndex(x, y);
                const Tmx::Tileset *tileset = _tileMap->GetTileset(tilesetIndex);
                int tileSize = tileset->GetImage()->GetWidth();
                std::string name = tileset->GetName();
                
                if (    name == "wall" ||
                        name == "cave_wall" ||
                        name == "bottom_left" ||
                        name == "bottom_right" ||
                        name == "tree_bottom_left" ||
                        name == "tree_bottom_right" ||
                        name == "tree_top_left" ||
                        name == "tree_top_right" ||
                        name == "tree_top_cave_wall_left" ||
                        name == "tree_top_cave_wall_right" ||
                        name == "tree_top1" ||
                        name == "tree_top2" ||
                        name == "tree_top3" ||
                        name == "tree_trunk" ||
                        name == "tree_trunk2" ||
                        name == "tree_trunk3" ||
                        name == "cave_wall_bottom" ||
                        name == "tree_bottom_only_right" ||
                        name == "tree_bottom_only_left" ||
                        name == "cave_wall_left" ||
                        name == "cave_wall_right" ||
                        name == "tree_wall_left_tree" ||
                        name == "tree_wall_right_tree" ||
                        name == "tree_entrance_left" ||
                        name == "tree_entrance_right") {
                    int posX = x * tileSize;
                    int posY = y * tileSize;
                    SDL_Rect tile;
                    tile.x = posX;
                    tile.y = posY;
                    tile.h = tileSize;
                    tile.w = tileSize;
                    _tiles.push_back(tile);
                }
            }
        }
    }
    
    // Add Units to tiles list
    _units.clear();
    list<spUnit> *units = _map->getRoom()->getUnits();
    for (list<spUnit>::iterator it = units->begin(); it != units->end(); ++it) {
        spUnit unit = *it;
        SDL_Rect unitRect = SDL_Rect();

        unitRect.x = unit->getPosition().x + 20;
        unitRect.y = unit->getPosition().y + 15;
        unitRect.h = 30;
        unitRect.w = 20;
        if (unit->getType() != "player" && !unit->isPotion()) {
            _tiles.push_back(unitRect);
        } else if (unit->isPotion()) {
            unit->setRect(unitRect);
        }
    }
}


/**
 * Gets the maze entrance location.
 *
 * @return the entrance Vector2.
 */
Vector2 Game::_getEntrance() {
    Vector2 location = _map->getRoom()->getEntrance();
    location.x *= tileSize;
    location.y *= tileSize;
    
    return location;
}
