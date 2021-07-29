#ifndef CIRCLE_H
#define CIRCLE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdint>

//A bird-oid object
class Circle {

private:

  static constexpr unsigned int max_velocity = 80;
  static constexpr unsigned int min_velocity = 20;
  static constexpr float step = 0.2f;
  //static constexpr unsigned int fov_spread = 315;
  static constexpr float drag = 0.998f;

  unsigned int radius = 5;
  unsigned int fov_length = 10*radius;

  sf::Color fill_color;
  sf::Vector2f velocity;
  sf::CircleShape body;

public:

//default constructor
  Circle() {
    this->body.setRadius(radius);
    this->body.setOrigin(this->radius, this->radius);
    //initialized at the top left corner of the screen and given random random velocity
    this->velocity = sf::Vector2f(rand() % this->max_velocity, rand() % this->max_velocity);

    this->body.setPosition(0,0);

    this->body.setOutlineColor(sf::Color(255,255,255,100));
    this->body.setOutlineThickness(1);
  }
//reinitialize the boid with custom parameters after it's constructed AND WITHIN THE SPECIFIED SCREEN SIZE
  void boidInit(const unsigned int& radius, const sf::Color& fill_color, const unsigned int& x, const unsigned int& y) {

    this->body.setRadius(radius);
    this->radius = radius;
    this->fov_length = radius*4;

    this->body.setOrigin(this->radius, this->radius);
    this->body.setFillColor(this->fill_color = fill_color);

    //place the boid in a random location within the sf::RenderWindow whose dimensions are passed as parameters
    this->body.setPosition(rand() % x, rand() % y);
  }

//instance methods used to determine collision with the screen borders
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
      this->velocity.y *= -1;
      this->body.move(0.f, -this->body.getGlobalBounds().top);
    }

    else if(this->checkBot(y)) {
      this->velocity.y *= -1;
      this->body.move(0.f, -this->body.getGlobalBounds().top - this->body.getGlobalBounds().height + y);
    }

    if (this->checkLeft()) {
      this->velocity.x *= -1;
      this->body.move(-this->body.getGlobalBounds().left,0.f);
    }

    else if(this->checkRight(x)) {
      this->velocity.x *= -1;
      this->body.move(-this->body.getGlobalBounds().left - this->body.getGlobalBounds().width+x,0.f);
    }
  }

//an alternative to the above method is to wrap the boids around the screen
  void wrap(const unsigned int& x, const unsigned int& y) {
    if (this->checkTop())
      this->body.setPosition(this->body.getPosition().x, y+this->body.getPosition().y);

    else if (this->checkBot(y))
      this->body.setPosition(this->body.getPosition().x, this->body.getPosition().y-y);

    if (this->checkLeft())
      this->body.setPosition(x+this->body.getPosition().x,this->body.getPosition().y);

    else if (this->checkRight(x))
      this->body.setPosition(this->body.getPosition().x-x,this->body.getPosition().y);

  }
//returns true iff the circle is in this circle's FOV
  bool distBetween(Circle circle) const {
    return abs(this->body.getPosition().x - circle.getPosition().x) < this->getFovLength() && abs(this->body.getPosition().y - circle.getPosition().y) < this->getFovLength();
  }

//getters
  sf::Vector2f getVelocity() const{
    return this->velocity;
  }
  sf::Vector2f getPosition() {
    return this->body.getPosition();
  }
  unsigned int getFovLength() const {
    return this->fov_length;
  }
//setters
  void setVelocity(const sf::Vector2f& vec) {
    this->velocity = vec;
  }
  void setFillColor(const sf::Color color) {
    this->body.setFillColor(color);
  }


//moves the boid along its velocity vector but multiplied by the STEP member variable in order to make movement seem more natural
  void fly() {

    //prevents the boid from accelerating endlessly
    if (sqrt(pow(this->velocity.x,2)+pow(this->velocity.y,2))>= this->max_velocity) {

      this->velocity.x *= this->max_velocity/sqrt(pow(this->velocity.x,2)+pow(this->velocity.y,2));

      this->velocity.y *= this->max_velocity/sqrt(pow(this->velocity.x,2)+pow(this->velocity.y,2));
    }

    else if (sqrt(pow(this->velocity.x,2)+pow(this->velocity.y,2)) < this->min_velocity) {

      this->velocity.x *= this->min_velocity/sqrt(pow(this->velocity.x,2)+pow(this->velocity.y,2));

      this->velocity.y *= this->min_velocity/sqrt(pow(this->velocity.x,2)+pow(this->velocity.y,2));

    }

    this->body.move(this->velocity*this->step);

    //applies a small drag to the velocity vector
    this->velocity.x *= drag;
    this->velocity.y *= drag;
  }

//draws the boid to the window which is passed to the method as a const pointer
  void draw(sf::RenderWindow* const window) {
    window->draw(this->body);
  }
};

#endif
