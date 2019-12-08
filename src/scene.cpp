#include "scene.hpp"

/* Contructor */
Scene::Scene() { }

/* Copy constructor */
Scene::Scene(const Scene& scene) {
    *this = scene;
}

/* Copy assignment operator */
Scene& Scene::operator=(const Scene& scene) {
    map_ = scene.map_;
    background_ = scene.background_;
    player_ = scene.player_;
    projectiles_ = scene.projectiles_;

    return *this;
}

/* Destructor */
Scene::~Scene() {
    main_window->clear();
    for(GameObject* obj : map_.objects) {
        delete obj;
    }
    for(Projectile* p : projectiles_) {
        delete p;
    }
    delete player_;
}

/* Run and setup singleplayer scene */
void Scene::Init() {
    map_ = MapLoader::LoadMap("src/Maps/map1.txt");
    CharacterSpurdo* spurdo = new CharacterSpurdo();
    player_ = new Player(spurdo, map_.player_location);

    if (!background_.loadFromFile(map_.background_file)) {
        // Error handling
    }

    main_window->setFramerateLimit(g_fps);
    Render();
    Loop();
}

/* Handle player movement and events, update these to the scene */
void Scene::Loop() {
    while (main_window->isOpen()) {
        sf::Event event;

        main_window->pollEvent(event);
        switch (event.type) {
            case sf::Event::Closed:
                main_window->close();
                break;
            default:
                break;
        }

        std::vector<Projectile*> projectiles = player_->Action(GetObjects());

        if (projectiles.empty() == false) {
            for (Projectile* p : projectiles) {
                projectiles_.push_back(p);
            }
        }

        Update();
        Render();
    }
}

/* Update game logic (bullets etc.) */
void Scene::Update() {
    // Handle projectiles
    for (auto p = projectiles_.begin(); p != projectiles_.end(); p++) {
        if ((*p)->GetVelocity().Length() == 0) {
            projectiles_.erase(p);
            p--;
        }
        else {
            (*p)->CheckCollisions(map_.objects);
            (*p)->SetPosition((*p)->GetPosition() + (*p)->GetVelocity());
            (*p)->Decelerate((*p)->GetSlowRate());
        }
    }
    // Handle objects
    for (auto o = map_.objects.begin(); o != map_.objects.end(); o++) {
        // Enemies
        if ((*o)->GetType() == ENEMY) {
            if ((*o)->dead_ == false) {
                (*o)->Action(player_->GetPosition(), map_.objects);
            }

            if ((*o)->GetHitPoints() <= 0 && !(*o)->dead_) {
                (*o)->dead_ = true;
                (*o)->collidable_ = false;
                (*o)->deadtimer_.restart();
                (*o)->GetTexture()->loadFromFile("src/Textures/dead_zombie.png");
            }
            else if ((*o)->deadtimer_.getElapsedTime().asMilliseconds() > 20000 && (*o)->dead_) {
                map_.objects.erase(o);
                o--;
            }
        }
    }
}

/* Render the game and update graphics */
void Scene::Render() {
    main_window->clear();

    sf::Sprite background;
    background.setTexture(background_);
    background.setScale(sf::Vector2f(0.8f, 0.8f));
    background.setPosition(sf::Vector2f(-400.f, -400.f));
    main_window->draw(background);

    for(GameObject* o : map_.objects) {
        main_window->draw(o->GetSprite());
        if (!o->dead_) {
            o->UpdateHP();
            main_window->draw(o->GetHPBackground());
            main_window->draw(o->GetHPBar());
        }
        //main_window->draw(o->GetHitbox());
    }
    if (projectiles_.empty() == false) {
        for(Projectile* p : projectiles_) {
            main_window->draw(p->GetSprite());
            main_window->draw(p->GetHitbox());
        }
    }
    main_window->draw(player_->GetSprite());
    main_window->draw(player_->GetHPBackground());
    main_window->draw(player_->GetHPBar());
    main_window->draw(player_->GetHitbox());

    main_window->display();
}
