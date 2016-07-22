#include "Potion.h"
#include "Game.h"
#include "KeyboardInput.h"
#include "res.h"
#include "tmx/Tmx.h"
#include "SDL.h"


/**
 * Constructor.
 */
Potion::Potion() {
    setType("potion");
}


/**
 *  Gets bounds of the unit Potion.
 *  @return: SDL_Rect _bounds which is the bounds of Potion.
 */
SDL_Rect Potion::getBounds() {
    Vector2 unitPosition = getPosition();
    _bounds.x = unitPosition.x + 20;
    _bounds.y = unitPosition.y + 15;
    _bounds.h = 30;
    _bounds.w = 20;
    
    return _bounds;
}


/**
 *  Adds Sprite and attaches it to the game.
 */
void Potion::addSprite() {
    _sprite = new Sprite;
    _sprite->setResAnim(resources.getResAnim("potion"));
    _sprite->attachTo(_view);
    _sprite->setAnchor(Vector2(0.5f, 0.5f));
}


/**
 * Initializes position and sprite of Potion. Called by Unit's init() method.
 */
void Potion::_init() {
    _isPotion = true;
    addSprite();
}


/**
 * Interaction method for Potion.
 */
void Potion::_interact() {
    if (!isDead() && _game->getPlayer()->isDamaged()) {
        // add health to player
        _game->getPlayer()->updateHealth(10);
        
        // stop collision detection
        _dead = true;
        
        // hide it with an alpha tween.
        _view->addTween(Actor::TweenAlpha(0), 300)->setDetachActor(true);        
    }
}


/**
 * Updates the Potion every frame. Called by Units update() method.
 *
 * @us is the UpdateStatus sent by Unit's update method.
 */
void Potion::_update(const UpdateState &us) {
    
}