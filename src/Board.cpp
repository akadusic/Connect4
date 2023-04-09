#include <Board.hpp>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

using namespace DataModels;

void Board::checkBellow(Player& player, const int dropChoice){
  int length, turn;
  length = 6;
  turn = 0;

  do{
    if(board_[length][dropChoice] != 'X' && board_[length][dropChoice] != 'O'){
      board_[length][dropChoice] = player.getPlayerId();
      turn = 1;
    } else {
      --length;
    }

  } while(turn != 1);
}

void Board::displayBoard(){
  int rows{6};
  int columns{7};

  for(auto i = 1; i <= rows; ++i){
    std::cout << "|";
    for(auto j = 1; j <= columns; ++j){
      if(board_[i][j] != 'X' && board_[i][j] != 'O'){
        board_[i][j] = '*';
      }
      std::cout << board_[i][j];
    }
    std::cout << "|" << std::endl;
  }
}

int Board::fullBoard(){
  int full{0};
  for(auto i = 1; i <= 7; ++i){
    if(board_[1][i] != '*') ++full;
  }

  return full;
}

int Board::checkFour(Player& player){
  char XO = player.getPlayerId();
  int win{0};

  for(auto i = 8; i >= 1; --i){
    for(auto j = 9; j >= 1; --j){
      if(board_[i][j] == XO && board_[i-1][j-1] == XO && board_[i-2][j-2] == XO && board_[i-3][j-3] == XO) {
        win = 1;
      }

      if(board_[i][j] == XO && board_[i][j-1] == XO && board_[i][j-2] == XO && board_[i][j-3] == XO) {
        win = 1;
      }

      if(board_[i][j] == XO && board_[i-1][j] == XO && board_[i-2][j] == XO && board_[i-3][j] == XO) {
        win = 1;
      }

      if(board_[i][j] == XO && board_[i-1][j+1] == XO && board_[i-2][j+2] == XO && board_[i-3][j+3] == XO) {
        win = 1;
      }

      if(board_[i][j] == XO && board_[i][j+1] == XO && board_[i][j+2] == XO && board_[i][j+3] == XO) {
        win = 1;
      }
    }
  }

  return win;
}

int Board::restart(){
  int restart;

  std::cout << "Would you like to restart? Yes(1) No(2): ";
  std::cin >> restart;
  if(restart == 1){
    for(auto i{1}; i <= 6; ++i){
      for(auto j{1}; j <= 7; ++j){
        board_[i][j] = '*';
      }
    }
  } else {
    std::cout << "Goodbye!" << std::endl;
  }
  return restart;
}

int Board::dropPlayer(Player& player){
  int dropChoice{0};
  do{
    std::cout << player.getPlayerName() << "'s Turn " << std::endl;
    if(choice_ == 1){
      std::cout << "Please enter a number between 1 and 7: ";
      std::cin >> dropChoice;
    } else if(choice_ == 2){
      srand(time(nullptr));
      dropChoice = rand()%7 + 1; 
      sleep(2); 
    }
    player.increaseNumberOfMoves();
    std::cout << "Number of moves of player " << player.getPlayerName() << " is " << +player.numberOfMoves() << "." << std::endl;

    while(board_[1][dropChoice] == 'X' || board_[1][dropChoice] == 'O'){
<<<<<<< HEAD
      std::cout << "That row is full, please enter a new row: ";
      if(choice_ == 1){
=======
      if(choice_ == 1){
        std::cout << "That row is full, please enter a new row: " << std::endl;
>>>>>>> e064683 (2023/4/9, Random players final changes)
        std::cin >> dropChoice;
      } else if (choice_ == 2){
        srand(time(nullptr));
        dropChoice = rand()%7 + 1;
      }
    }
  } while(dropChoice < 1 || dropChoice > 7);
  return dropChoice;
}

void Board::displayMenu(){
  std::cout << "======================================" << std::endl;
  std::cout << "Type your choice of game: " << std::endl;
  std::cout << "Both human players (1)" << std::endl;
  std::cout << "Both random players (2)" << std::endl;
  std::cout << "======================================" << std::endl;
  std::cout << "Choice: ";
  std::cin >> choice_;
}
