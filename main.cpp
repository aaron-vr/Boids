
#include <iostream>
#include "game.h"

//write to cmd: g++ -Isrc/include -c main.cpp && g++ main.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system && main

int main () {

  //initialize game session
  Game<Circle> game(5, 100, sf::Color::Black, true);

  //game loop facillitating double buffering
  while (game.running())
  {
    //update the state of the game for this frame
    game.update();

    //render the state of the game onto the screen
    game.render();

  }
  return 0;
}
