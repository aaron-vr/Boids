#ifndef ARROW_H
#define ARROW_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdint>

//A bird-oid object
class Arrow {
private:

  //denotes the range of the boid's field of view from 1 to 360
  static constexpr unsigned int fov_spread = 315;

  //max and min speed i.e.min and max multiplier of step
  static constexpr unsigned int max_speed = 30;
  static constexpr unsigned int min_speed = 10;
  static constexpr float step = 0.4f;

  //max rate at which the speed of boid can change per frame
  static constexpr float max_accel = 1.01f;
  static constexpr float max_deccel = 0.9f;

  //surface area of shape's collision box in its starting rotation (length = width)
  unsigned int scale = 10;
  //denote the radius of the boid's field of view
  unsigned int fov_length = 3*scale;
  //denotes the size (length) of step per frame; can be negative denoting movement in the opposite cardinal direction
  float speed = 5;

  sf::Color fill_color;



  sf::ConvexShape body;

public:

  //default constructor (the boid is not placed anywhere as the window dimensions are not known yet)
  Arrow() {
    //pass the variables to the sf::ConvexShape
    this->body.setPointCount(4);
    this->body.setPoint(0, sf::Vector2f(0,0));
    this->body.setPoint(1, sf::Vector2f(this->scale,this->scale*3/8));
    this->body.setPoint(2, sf::Vector2f(0,this->scale*3/4));
    this->body.setPoint(3, sf::Vector2f(this->scale/4,this->scale*3/8));

    this->body.setOrigin(this->scale*3/8, this->scale*3/8);

    //initialized and passed random direction/rotation
    this->body.setRotation(rand() % 360);
    this->body.setOutlineColor(sf::Color(255,255,255,100));
    this->body.setOutlineThickness(1);

  }
  //initialize each boid after it's constructed
  void boidInit(const unsigned int& scale, const sf::Color& fill_color, const unsigned int& x, const unsigned int& y) {

    //only modify the parameters that are not equal to their default values

    if (this->scale != scale) {
      this->scale = scale;

      this->fov_length = 3*scale;

      this->body.setPoint(0, sf::Vector2f(0,0));
      this->body.setPoint(1, sf::Vector2f(this->scale,this->scale*3/8));
      this->body.setPoint(2, sf::Vector2f(0,this->scale*3/4));
      this->body.setPoint(3, sf::Vector2f(this->scale/4,this->scale*3/8));

      this->body.setOrigin(this->scale*3/8, this->scale*3/8);

    }
    this->body.setFillColor(this->fill_color = fill_color);

    //place the boid in a random location within the sf::RenderWindow whose dimensions are passed as parameters
    this->body.setPosition(rand() % x, rand() % y);
  }

  //destructor
  //~Boid(); no need for one because heap-allocated memory is not used

  //getters
  sf::Vector2f getPosition() const{
    return this->body.getPosition();
  }
  float getSpeed() const{
    return this->speed;
  }
  unsigned int getRotation() const {
    return this->body.getRotation();
  }
  unsigned int getFovLen() const {
    return this->fov_length;
  }
  unsigned int getFovSpr() const {
    return this->fov_spread;
  }

  //setters
  void rotate(const int& i) {
    this->body.rotate(i);
  }
  void setRotation(const int& i) {
    this->body.setRotation(i);
  }
  void accelerate(const float& factor) {
    if (!factor)
      return;
    if (factor > this->max_accel)
      this->speed *= max_accel;
    else if (factor < this->max_deccel)
      this->speed *= max_deccel;
    else
      this->speed *= factor;

    if (this->speed > this->max_speed)
      this->speed = this->max_speed;
    else if (this->speed < this->min_speed)
      this->speed = this->min_speed;
  }
  //instance methods

  bool checkTop() {
    return this->body.getGlobalBounds().top < 0;
  }
  bool checkBot(const unsigned int& y) {
    return this->body.getGlobalBounds().top + this->body.getGlobalBounds().height > y;
  }
  bool checkRight(const unsigned int& x) {
    return this->body.getGlobalBounds().left + this->body.getGlobalBounds().width >= x;
  }
  bool checkLeft() {
    return this->body.getGlobalBounds().left <= 0;
  }

  //makes the boid bounce against the screen borders if it is intersecting any one or two of them; otherwise does nothing
  void bounce(const unsigned int& x, const unsigned int& y) {

    if (this->checkTop()) {
      this->body.setRotation(360-(int)this->body.getRotation());
      this->body.move(0.f, -this->body.getGlobalBounds().top);
    }

    else if(this->checkBot(y)) {
      this->body.setRotation(360-(int)this->body.getRotation());
      this->body.move(0.f, -this->body.getGlobalBounds().top - this->body.getGlobalBounds().height + y);
    }
//            (540 - x)%360
    if (this->checkLeft()) {
      this->body.setRotation(180-(int)this->body.getRotation()%360);
      this->body.move(-this->body.getGlobalBounds().left,0.f);
    }
    else if(this->checkRight(x)) {
      this->body.setRotation(270-((int)this->body.getRotation()+90)%360);
      this->body.move(-this->body.getGlobalBounds().left - this->body.getGlobalBounds().width+x,0.f);
    }
  }
  //alternative to the above function; instead of bouncing against them, the boid will avoid the screen borders in advace (provded they are within its FOV)
  void avoidWalls();

  //returns an integer value denoting the distance between two boids; returns 0 if the foreign boid is not within the FOV of this->boid
  unsigned int distBetween(const Arrow& arrow) {
    unsigned int diff = static_cast<int>(sqrt(pow(this->body.getPosition().x - arrow.getPosition().x, 2) + pow(this->body.getPosition().y - arrow.getPosition().y, 2)));
    return diff < this->fov_length ? diff : 0;
  }
  //boid moves in the direction it is currently facing at the speed it's currently moving at
  void fly() {
    this->body.move(cos(this->body.getRotation()*M_PI/180)*this->speed*this->step, sin(this->body.getRotation()*M_PI/180)*this->speed*this->step);
    this->accelerate(this->speed*1.01);
  }
  void draw(sf::RenderWindow* const window) {
    window->draw(this->body);
  }
};

#endif
