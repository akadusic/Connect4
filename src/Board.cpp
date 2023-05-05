#include <Board.hpp>
#include <cstdint>
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
  
  auto humanPlayers = [&](){
    std::cout << "Please enter a number between 1 and 7: ";
    std::cin >> dropChoice;
  };
  
  // ovdje napraviti lambdu za minimax poziv funkcije
  
  auto minimax = [&]{
   dropChoice = minMax(2, true, player);
   std::cout << "Drop choice is: " << dropChoice << std::endl;
  };

  auto randomPlayers = [&]{
    srand(time(nullptr));
    dropChoice = rand()%7 + 1;
    sleep(2);
  };

  auto humanVsMachine = [&]{
    if(player.getPlayerId() == 'X') humanPlayers();
    else randomPlayers();
  };

  auto wayOfPlaying = [&]{
    if(choice_ == 1) humanPlayers();
    else if(choice_ == 2) randomPlayers();
    else if(choice_ == 3) humanVsMachine();
    else if(choice_ == 4) minimax();
  };

  auto checkFullRow = [&]{
    while(board_[1][dropChoice] == 'X' || board_[1][dropChoice] == 'O'){
      std::cout << "That column is full, please enter a new column: " << std::endl;
      wayOfPlaying();
    }
  };

  do{
    std::cout << player.getPlayerName() << "'s Turn " << std::endl;

    wayOfPlaying();

    player.increaseNumberOfMoves();
    std::cout << "Number of moves of player " << player.getPlayerName() << " is " << +player.numberOfMoves() << "." << std::endl;

    checkFullRow(); 
  } while(dropChoice < 1 || dropChoice > 7);
  return dropChoice;
}

void Board::displayMenu(){
  std::cout << "======================================" << std::endl;
  std::cout << "Type your choice of game: " << std::endl;
  std::cout << "Both human players (1)" << std::endl;
  std::cout << "Both random players (2)" << std::endl;
  std::cout << "Human vs Machine (3)" << std::endl;
  std::cout << "Use MinMax algorithm (4)" << std::endl;
  std::cout << "======================================" << std::endl;
  std::cout << "Choice: ";
  std::cin >> choice_;
}


int Board::minMax(unsigned int depth, bool maxPlayer, Player& player){
  auto isValidMove = [&, this](int column){
    bool result{false};
    for(auto i = 1; i <= ROWS; ++i){
      bool result = (board_[i][column] == '*');
    }
    return result;
  };

  int result{0};

  std::cout << "MinMax called!" << std::endl;
  if(depth == MAX_DEPTH){
    std::cout << "Base case called!" << std::endl;
    return calculateScore(player);
  } 
  
  if(maxPlayer){
    int maxScore = INT32_MIN;
    for(auto col{0}; col < COLS; ++col){
      std::cout << "Before if!" << std::endl;
      if(isValidMove(col)){
        std::cout << "Depth is: " << depth << std::endl;
        int score = minMax(depth + 1, false, player);
        result = std::max(result, score);
        std::cout << "Result in max is: " << result << std::endl;
      } 
    }
  } else {
    int minScore = INT32_MAX;
    std::cout << "Min called: " << std::endl;
    for(auto col{0}; col < COLS; ++col){
      if(isValidMove(col)){
        
        int score = minMax(depth + 1, true, player);
        result = std::min(result, score);
        std::cout << "Result in min is: " << result << std::endl;
      }
    }
  }

  return result;
}

int Board::calculateScore(Player& player) {
    int score{0};
    char XO = player.getPlayerId();
    
    for(auto i = 8; i >= 1; --i){
      for(auto j = 7; j >= 1; --j){
        if(board_[4][5] == XO){
          score += 100;
        }
        if(board_[i][j] == XO && board_[i-1][j-1] == XO && board_[i-2][j-2] == XO) {
          score += 100;
        }

        if(board_[i][j] == XO && board_[i][j-1] == XO) {
          score += 10;
        }

        if(board_[i][j] == XO && board_[i-1][j] == XO && board_[i-2][j] == XO) {
          score += 100;
        }

        if(board_[i][j] == XO && board_[i-1][j+1] == XO) {
          score += 10;
        }

        if(board_[i][j] == XO && board_[i][j+1] == XO && board_[i][j+2] == XO) {
          score += 100;
        } 
      }
    }
    
    return score;
}

