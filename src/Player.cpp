#include <Player.hpp>
#include <iostream>

using namespace DataModels;

void Player::playerWin(){
  std::cout << std::endl << playerName_ << " Connected Four, You Win!" << std::endl; 
}
