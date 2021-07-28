#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "boids.h"


template<typename T>
/*game engine housing all the logic and concealing it from the user*/
class Game {

private:

//member variables
Boids<T> boids;
//rendering data
sf::RenderWindow* window = nullptr;
sf::Event ev;
sf::VideoMode videomode;

public:
//constructor and destructor
  Game(const unsigned int& size, const size_type& count, sf::Color fill_color, bool rand_saturation, unsigned int x = 1900, unsigned int y = 1000) {

    //initialize window
    this->videomode.width = x;
    this->videomode.height = y;

    this->window = new sf::RenderWindow(this->videomode, "Flocking Behaviour", sf::Style::Close | sf::Style::Resize);
    this->window->setPosition(sf::Vector2i(0,0));
    this->window->setVerticalSyncEnabled(true);

    //initialize variables i.e. boids
    this->boids.boidsInit(size, count, fill_color, this->window, rand_saturation);
  }
  ~Game() {
    delete this->window;
  }
//accessors; the game and event loops run in main

  bool running() const {
    return this->window->isOpen();
  }

  void update() {
    this->pollEvents();
  }

  void render() {

    this->window->clear(sf::Color(20,20,30,255));

    //render all the content within this window
    this->boids.visualize(this->window);

    this->window->display();
  }

  void pollEvents() {
    while (this->window->pollEvent(this->ev))
      switch (this->ev.type) {
        case sf::Event::Closed :
          this->window->close();
          break;
        case sf::Event::Resized :
          printf("new window width: %i new window height: %i\n", this->ev.size.width, this->ev.size.height);
          break;
      }
    return;
  }

};
#endif
