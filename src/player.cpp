#include "player.hpp"

/* Constructor. Get parameter for what character player chose. */
Player::Player(Character* character, sf::Vector2f pos) :
GameObject(pos, character->GetTextureFile(), character->GetHitBox(), PLAYER, character->GetMaxSpeed(), character->GetAcceleration(),
character->GetDamage(), character->GetHP(), true, character->GetAttackDelay()) {
    SetOrigin(character->GetOrigin().x, character->GetOrigin().y);
    player_cam_.setCenter(GetPosition());
    if (!buffer_.loadFromFile("src/Audio/Sound/sound_gun.ogg")) {
        std::cout << "ERROR: Loading gun sound failed!" << std::endl;
    }
    gunshot_.setBuffer(buffer_);
    gunshot_.setVolume(80);
    weapon_ = blaster;
}

/* Handle keypress and their effects on player character */
std::vector<Projectile*> Player::Action(std::vector<GameObject*> objects) {

    PhysicsVector dir_vector = PhysicsVector(0.0f, 0.0f);
    std::vector<Projectile*> projectiles;

    // Handle bullet spawning and setting of initial speed
    sf::Time time = attack_timer_.getElapsedTime();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && time.asMilliseconds() > weapon_.shoot_delay) {
        attack_timer_.restart();
        Shoot();

        if (sound_on) {
            gunshot_.play();
        }

        projectiles = Shoot();
    }
    else {
        // Movement to the left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            dir_vector += PhysicsVector(-1.0f, 0.0f);
        }

        // Movement to the right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            dir_vector += PhysicsVector(1.0f, 0.0f);
        }

        // Movement Projectile* bullet = new Projectile(GetPosition());up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            dir_vector += PhysicsVector(0.0f, -1.0f);
        }

        // Movement down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            dir_vector += PhysicsVector(0.0f, 1.0f);
        }

        //Switch weapons from number buttons
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            SwitchWeapon(BLASTER);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            SwitchWeapon(SHOTGUN);
        }

    }

    Move(dir_vector);
    CheckCollisions(objects);
    SetPosition(GetPosition() + GetVelocity());
    Rotate();

    player_cam_.setCenter(GetPosition());
    main_window->setView(GetView());

    return projectiles;
}

std::vector<Projectile*> Player::Shoot() {
    std::vector<Projectile*> projectiles;
    if (weapon_.type == BLASTER) {
        Projectile* bullet = new Projectile(GetPosition(), GetType(), weapon_.bullet);
        PhysicsVector direction = GetCurrentCursorDirection();

        PhysicsVector vel = GetVelocity().UnitVector();
        SetVelocity(vel.Scale(0.5f));
        direction = PhysicsVector(direction.x * bullet->GetMaxSpeed()/sqrt(2), direction.y * bullet->GetMaxSpeed()/sqrt(2));
        bullet->SetVelocity(direction + GetVelocity());
        projectiles.push_back(bullet);
    }
    else if (weapon_.type == SHOTGUN) {
        for (int i = -2; i < 3; i++) {
            Projectile* bullet = new Projectile(GetPosition(), GetType(), weapon_.bullet);
            PhysicsVector direction = GetCurrentCursorDirection();
            direction.Rotate(i*15);

            PhysicsVector vel = GetVelocity().UnitVector();
            SetVelocity(vel.Scale(0.01f));

            direction = PhysicsVector(direction.x * bullet->GetMaxSpeed()/sqrt(2), direction.y * bullet->GetMaxSpeed()/sqrt(2));
            bullet->SetVelocity(direction + GetVelocity());
            projectiles.push_back(bullet);
        }
    }

    return projectiles;
}

void Player::SwitchWeapon(int weapon_type) {
    if (weapon_type == BLASTER) {
        weapon_ = blaster;
    }
    else if (weapon_type == SHOTGUN) {
        weapon_ = shotgun;
    }
}

/* Rotate player */
void Player::Rotate() {
    sf::Vector2f direction = GetCurrentCursorDirection(); // Get current mouse direction, relative to the player.
    SetRotation(direction); // Rotate the player sprite into new position.
}

/* Function that calculates current mousewise direction of the player sprite
   Returns the direction of the player */
PhysicsVector Player::GetCurrentCursorDirection() {
    sf::Vector2i cursor = sf::Mouse::getPosition(*main_window); // Get the mouse position on main window in pixels
    sf::Vector2f worldCursor = main_window->mapPixelToCoords(cursor); // Get the mouse position in world coordinates
    sf::Vector2f direction = worldCursor - GetPosition(); // Get the relative direction
    return PhysicsVector(direction).UnitVector();
}
