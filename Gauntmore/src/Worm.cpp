#include "res.h"
#include "Worm.h"
#include <cmath>


/**
 * Constructor. Initializes the stats
 */
Worm::Worm() {
    _hp = 8;
    _attack = 10;
    _defense = 0;
    _speed = 30;
    _attackSpeed = 4;
    _lastTimeAttack = time(0);
}


/**
 * Returns the bounds of the Creature
 *
 * @return SDL_Rect that is the bounds for Creature
 */
SDL_Rect Worm::getBounds() {
    Vector2 unitPosition = getPosition();
    _bounds.x = unitPosition.x + 20;
    _bounds.y = unitPosition.y + 15;
    _bounds.h = 30;
    _bounds.w = 20;
    
    return _bounds;
}


/**
 * Puts a Creature sprite in the game
 */
void Worm::addSprite() {
    _sprite = new Sprite;
    _sprite->setResAnim(resources.getResAnim("worm_down"));
    _sprite->attachTo(_view);
    _sprite->setAnchor(Vector2(0.5f, 0.5f));
    move();
}


/**
 * Plays the move animation.
 */
void Worm::move() {
    switch (_facing) {
        case up:
            _moveTween = _sprite->addTween(TweenAnim(resources.getResAnim("worm_down")), 500, -1);
            break;
        case right:
            _moveTween = _sprite->addTween(TweenAnim(resources.getResAnim("worm_right")), 500, -1);
            break;
        case down:
            _moveTween = _sprite->addTween(TweenAnim(resources.getResAnim("worm_up")), 500, -1);
            break;
        case left:
            _moveTween = _sprite->addTween(TweenAnim(resources.getResAnim("worm_left")), 500, -1);
            break;
        default:
            break;
    }
}


/**
 * Initializes a creatures position and sprite. Called by Unit's init() method.
 */
void Worm::_init() {
    addSprite();
    _setContents();
    findPath.setGame(_game);
}


/**
 * Updates the creature every frame. Called by Unit::update.
 *
 * @us is the UpdateStatus sent by Unit's update method.
 */
void Worm::_update(const UpdateState &us) {
    
    Vector2 direction = moveMe();
    Vector2 position = getPosition();
    
    Facing prevFacing = _facing;
    _facing = getFacing(direction);
    if(_facing != prevFacing){
        move();
    }
    
    position += direction * (us.dt / 1000.0f) * _speed;
    setPosition(position);
    if((abs(position.x - _game->getPlayer()->getPosition().x) <= 70) && (abs(position.y - _game->getPlayer()->getPosition().y) <= 70)){
        time_t _current = time(0);
        if(_current >= _lastTimeAttack + _attackSpeed){
            _lastTimeAttack = time(0);
            damage();
            //cout << "attacking" << endl;
        }
    }
    
}