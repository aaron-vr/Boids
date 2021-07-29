#ifndef BOIDS_H
#define BOIDS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "circle.h"

using size_type = std::size_t;
//a pointer to a dynamically allocated array of Boid objects; also provides methods for crowd control

template<typename T>
class Boids {

private:

  T* values = nullptr;
  size_type sz = 0;
  sf::Color fill_color = sf::Color::Red;

public:

  //initialize with custom variables
  void boidsInit(const unsigned int& size, const size_type& count, const sf::Color& fill_color, sf::Window* const window, bool rand_color) {

    //heap-allocate the Boid array which initializes all the boids with their default constructors
    this->values = new T[sz = !count ? 1 : count];

    //overwrite each boid within the array with custom parameters and subsequently draw it to the screen

    for (size_type i = 0; i < this->sz; ++i) {

      //compute color
      uint8_t rgb_arr[] = {fill_color.r, fill_color.g, fill_color.b};
      //randomize the color's saturation
      if (rand_color) *std::min_element(rgb_arr, rgb_arr+3) = rand() % 255;
      //call the init method for each boid
      values[i].boidInit(size, sf::Color(*rgb_arr, *(rgb_arr+1), *(rgb_arr+2), (uint8_t)255), window->getSize().x, window->getSize().y);
    }
    this->values[0].setFillColor(sf::Color::Red);

  }

  //destructor
  ~Boids() {
    delete[] values;
    values = nullptr;
    sz = 0;
  }

  //visualize the boid array for each frame
  void visualize(sf::RenderWindow* const window) {

    unsigned int fov_length = values[0].getFovLength();

    for (size_type i{0}; i < this->sz; i++) {

      sf::Vector2f centre_of_mass(0,0);
      sf::Vector2f avg_velocity(0,0);

      float x_res = values[i].getVelocity().x;
      float y_res = values[i].getVelocity().y;


      for (size_type j{0}; j < this->sz; j++) {

        if (j != i && values[i].distBetween(values[j])) {

          avg_velocity+=values[j].getVelocity();
          centre_of_mass += values[j].getPosition();

          x_res+=values[i].getPosition().x-values[j].getPosition().x;
          y_res+=values[i].getPosition().y-values[j].getPosition().y;
        }
      }

      centre_of_mass.x/=(sz-1);
      centre_of_mass.y/=(sz-1);

      avg_velocity.x/=(sz-1);
      avg_velocity.y/=(sz-1);


      values[i].setVelocity(sf::Vector2f(
        x_res
        +(centre_of_mass.x-values[i].getVelocity().x)/50
        +(avg_velocity.x-values[i].getVelocity().x)/20
        +(950-values[i].getPosition().x)/400
        ,
        y_res
        +(centre_of_mass.y-values[i].getVelocity().y)/50
        +(avg_velocity.y-values[i].getVelocity().y)/20
        +(500-values[i].getPosition().y)/400
      ));

      values[i].wrap(window->getSize().x, window->getSize().y);
      //values[i].bounce(window->getSize().x, window->getSize().y);
      values[i].fly();
      values[i].draw(window);
    }

  }

};

#endif
