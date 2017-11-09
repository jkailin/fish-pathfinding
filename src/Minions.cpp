#include "Minions.h"
#include <cassert>
#include <iostream>
const float Minions::WEAK_TIME=10.0f;
const float Minions::ATTACK_SPEED=0.5f;
const float Minions::SHOT_TIME=0.5f;
const float Minions::NORMAL_SIZE=100.0f;
const float Minions::WEAK_SIZE=NORMAL_SIZE*2.0f/3.0f;
const float Minions::ATTACK_RADIUS=1.5f*NORMAL_SIZE;
const float Minions::NORMAL_SPEED=ATTACK_SPEED/2.0f;
const float Minions::ESCAPE_SPEED=NORMAL_SPEED*2.0f;

Minions::Minions(sf::Vector2f pos)
{
    _state=NORMAL;    
    Fish::addKnot(Knot(NORMAL_SIZE,NORMAL_SIZE,pos));
    _attackRadius=ATTACK_RADIUS;
    _body=sf::RectangleShape(sf::Vector2f(NORMAL_SIZE,NORMAL_SIZE));
    _body.setOrigin(NORMAL_SIZE,0.5f*NORMAL_SIZE);
    _attackTarget=nullptr;
    _timer=0.0f;
    _speed=NORMAL_SPEED;
}

void Minions::init()
{
    assert(_texture!=nullptr);
    assert(_textureAreas!=nullptr);
    _body.setTexture(_texture.get());
    _body.setTextureRect((*_textureAreas)["Minions_normal"]);
}

Minions::State Minions::getState() const
{
    return _state;
}
float Minions::getAttackRadius() const
{
    return _attackRadius;
}
void Minions::setAttackTarget(const std::shared_ptr<Fish>& attackTarget)
{
    _attackTarget=attackTarget;
}
const std::shared_ptr<Fish>& Minions::getAttackTarget()const
{
    return _attackTarget;
}
float Minions::getSpeed() const
{
    return _speed;
}

void Minions::attacking()
{
    _state=ATTACK;
    _timer=0.0f;
    _speed=ATTACK_SPEED;
}

void Minions::shot()
{
    _state=SHOT;
    _timer=0.0f;
    _speed=NORMAL_SPEED;
    assert(_textureAreas!=nullptr);
    _body.setTextureRect((*_textureAreas)["Minions_shot"]);
}

void Minions::shocked()
{
    _state=SHOCKED;
    _speed=NORMAL_SPEED;
    assert(_textureAreas!=nullptr);
    _body.setTextureRect((*_textureAreas)["Minions_shocked"]);
}

void Minions::weaken()
{
    _state=WEAK;
    _speed=ESCAPE_SPEED;
    _timer=0.0f;
    _body.setScale(sf::Vector2f(WEAK_SIZE/NORMAL_SIZE,WEAK_SIZE/NORMAL_SIZE));
    assert(_textureAreas!=nullptr);
    _body.setTextureRect((*_textureAreas)["Minions_weak"]);
}

void Minions::recovered()
{
    _state=NORMAL;
    _speed=NORMAL_SPEED;
    _body.setScale(sf::Vector2f(1.0f,1.0f));
    assert(_textureAreas!=nullptr);
    _body.setTextureRect((*_textureAreas)["Minions_normal"]);
}

void Minions::defeated()
{
    _state=DIE;
}

void Minions::update(float deltaTime)
{
    switch(_state)
    {
        case NORMAL:
            break;
        case WEAK:
            _timer+=deltaTime;
            if(_timer>WEAK_TIME)
            {
                _timer=0;
                recovered();
            }
            break;
        case SHOCKED:
            break;
        case SHOT:
            _timer+=deltaTime;
            if(_timer>SHOT_TIME)
            {
                _timer=0;
                if(_state==NORMAL)
                    weaken();
                else if(_state==WEAK)
                    defeated();
            }
            break;
        case ATTACK:
            break;
        default:
            break;
    }
    updateShape();
}

void Minions::updateShape()
{
    assert(_knots.size()>=1);
    _body.setPosition(_knots[0].getPosition());
    _body.setRotation(_knots[0].getRotation());
}

void Minions::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
    states.transform*=getTransform();
    target.draw(_body,states);
}



