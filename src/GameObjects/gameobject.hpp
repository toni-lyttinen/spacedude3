#pragma once

#include <iostream>
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include "../movement.hpp"

extern sf::RenderWindow* main_window;

enum Type {
    OBJECT,
    ENEMY,
    PLAYER,
    WALL,
    BULLET,
    GOAL
};

/* Parent class for all game objects */
class GameObject : public Movement {
public:
    /* Default constructor */
    GameObject() = default;

    /* Constructor */
    GameObject(sf::Vector2f pos, std::string file, RectHitbox hitbox, int type = OBJECT, double max_speed = 5.f,
               double acceleration = 1.f, int damage = 0, float hitpoints = 69, bool shootable = false, float attack_delay = 1000);

    /* Copy constructor */
    GameObject(const GameObject&);

    /* Copy assignment operator */
    GameObject& operator=(const GameObject&);

    /* Destructor */
    virtual ~GameObject();

    /* Sets position of the object and its sprite */
    void SetPosition(PhysicsVector new_pos);

    /* Set object origin */
    void SetOrigin(float x, float y);

    /* Set object rotation */
    void SetRotation(PhysicsVector direction);

    /* Set new hitbox for GameObject */
    void SetHitbox(RectHitbox hitbox);

    /* Handle hpbar updating with hitpoints member */
    void UpdateHP();

    /* Function resposible for updating player character position */
    void Move(PhysicsVector direction);

    /* Check if player is colliding with items and change movement according to that */
    virtual bool CheckCollisions(std::vector<GameObject*> objects);

    /* Take damage */
    void TakeDamage(float damage);

    virtual void DeathSound() { }

    /* Set object sprite */
    void SetSprite(std::string file);

    PhysicsVector GetPosition() { return pos_; }
    sf::Sprite GetSprite() { return sprite_; }
    sf::FloatRect GetRect() { return hitbox_.getGlobalBounds(); }
    PhysicsVector GetRectPosition() { return hitbox_.getPosition() - hitbox_.getOrigin(); }
    sf::RectangleShape GetHitbox() { return hitbox_; }
    sf::Texture* GetTexture() { return texture_; }
    sf::RectangleShape GetHPBar() { return hpbar_; }
    sf::RectangleShape GetHPBackground() { return hpbarbackground_; }
    int GetDamage() { return damage_; }
    float& GetHitPoints() { return hitpoints_; }
    float GetMaxHitPoints() { return max_hitpoints_; }
    bool IsShootable() { return shootable_; }
    int GetType() { return type_; }
    float GetAttackDelay() { return attack_delay_; }

    sf::Clock attack_timer_;
    sf::Clock deadtimer_; // Timer for tracking how long has object been dead
    bool dead_ = false; // Indicates if the object is dead or alive
    bool collidable_ = true;

private:
    PhysicsVector pos_;
    sf::Sprite sprite_;
    sf::Texture* texture_;
    RectHitbox hitbox_;
    sf::RectangleShape hpbar_;
    sf::RectangleShape hpbarbackground_;
    int damage_;
    float attack_delay_;
    float hitpoints_;
    float max_hitpoints_;
    bool shootable_;
    int type_;
};

/* Overload << operator for printing */
std::ostream& operator<<(std::ostream& os, PhysicsVector vector);
