#include <Board.hpp>
#include <cstdint>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <utility>

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
   sleep(2);
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


/* int Board::minMax(unsigned int depth, bool maxPlayer, Player& player){
  // first column, then row in result
  auto possiblePositions = checkPositions();
  
  if(depth == MAX_DEPTH){
    std::cout << "Base case called!" << std::endl;
    int result = calculateScore(player);
    std::cout << "Result in base func: " << result << std::endl;
    return result;
  } 
  
  if(maxPlayer){
    int maxScore = INT32_MIN;
    for(auto elem : possiblePositions){
      std::cout << "Before if!" << std::endl;
      std::cout << "Depth is: " << depth << std::endl;
      int newDepth = depth + 1;
      int score = minMax(newDepth, false, player);
      maxScore = std::max(maxScore, score);
    }
    return maxScore;
  } else {
    int minScore = INT32_MAX;
    std::cout << "Min called: " << std::endl;
    for(auto elem : possiblePositions){
        int newDepth = depth + 1; 
        int score = minMax(newDepth, true, player);
        minScore = std::min(minScore, score);
    }
    return minScore;
  }
} */

int Board::minMax(unsigned int depth, bool maximizingPlayer, Player& play) {
   auto id = play.getPlayerId(); 
   int nextMove = calculateScore(play);

    if (depth == MAX_DEPTH) {
        return nextMove;
    }

    if (maximizingPlayer) {
        int maxScore = INT32_MIN;
        // std::clog << "Maximazing called!" << "\n";
        for (int i = 0; i < board_.size(); i++) {
            for (int j = 0; j < board_[i].size(); j++) {
                if (board_[i][j] == '*') {
                    board_[i][j] = id;
                    int currScore = minMax(depth + 1, false, play);
                    maxScore = std::max(maxScore, currScore);
                    board_[i][j] = '*';
                }
            }
        }
        return maxScore;
    } else {
        int minScore = INT32_MAX;
        char opponent = (id == 'X') ? 'O' : 'X';
        // std::clog << "Minimizing called!" << "\n";
        for (int i = 0; i < board_.size(); i++) {
            for (int j = 0; j < board_[i].size(); j++) {
                if (board_[i][j] == '*') {
                    board_[i][j] = opponent;
                    int currScore = minMax(depth + 1, true, play);
                    minScore = std::min(minScore, currScore);
                    board_[i][j] = '*';
                }
            }
        }
        return minScore;
    } 
}

int Board::calculateScore(Player& play) {
    int max_score = -1;
    int max_col = -1;
    auto player = play.getPlayerId();

    // Check rows for four in a row
    for (int row = 0; row < ROWS-1; row++) {
        for (int col = 0; col < COLS - 4; col++) {
            int count = 0;
            for (int i = 0; i < 4; i++) {
                if (board_[row][col + i] == player) {
                    count++;
                }
            }
            if (count > 0) {
                int score = count * count;
                if (score > max_score) {
                    max_score = score;
                    max_col = col;
                }
            }
        }
    }
    // std::clog << "Max col is " << max_col + 3 << "\n";
    return 2;
}

std::unordered_map<unsigned, unsigned> Board::checkPositions(){
  // In this function I checked possible values for game.
  possiblePositions_.clear();
  for(auto row{ROWS}; row >= 1; --row){
    if(possiblePositions_.size() != 7){
      for(auto col{1}; col <= COLS; ++col){
        if(board_[row][col] == '*'){
          auto pair = std::make_pair(row, col);
          possiblePositions_.insert(std::make_pair(col, row));
          std::cout << pair.first << pair.second;
        }
     }
    }
    std::cout << std::endl;
  }
  return possiblePositions_;
}
