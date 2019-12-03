#include <iostream>
#include <vector>
#include "scene.hpp"

/* Contructor */
Scene::Scene() {
    Player* player = new Player(sf::Vector2f(300.0f, 100.0f),"src/Textures/duderinosmall.png", RectHitbox(64.f, 64.f));
    Wall* wall = new Wall(sf::Vector2f(300.f,400.f), "src/Textures/wall.png", RectHitbox(128.f, 128.f));
    player_ = player;

    objects_.push_back(wall);
}

 /* Destructor */
Scene::~Scene() {
    for(GameObject* obj : objects_) {
        delete obj;
    }

}

/* Run and setup singleplayer scene. */
void Scene::Init() {
    main_window->setFramerateLimit(g_fps);
    
    //do initial drawing and rendering of objects
    main_window->clear();
    for(GameObject* obj : objects_) {
        main_window->draw(obj->GetSprite());
    }
    main_window->draw(player_->GetSprite());

    main_window->setView(player_->GetView());
    Render();

    Loop();

    End();
}

/* Deletes objects on the scene and closes it */
void Scene::End() {
    main_window->clear();
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

        player_->Action();
        
        Render();      
    }
}



/* Update game logic. Bullets etc. */
void Scene::Update() {

}

/* Render the game and update graphics */
void Scene::Render() {

    main_window->clear();
    
    for(GameObject* obj : objects_) {
        main_window->draw(obj->GetSprite());
        main_window->draw(obj->GetHitbox());
    }
    main_window->draw(player_->GetSprite());
    main_window->draw(player_->GetHitbox());
    
    main_window->display();
}
