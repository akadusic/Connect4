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
   dropChoice = findBestMove(player);
   std::clog << "Minimax returns: " << dropChoice << std::endl;
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
   auto possiblePositions = checkPositions();
  
    if (depth == MAX_DEPTH || fullBoard()) {
        return calculateScore(play);
    }

    if (maximizingPlayer) {
        int maxScore = INT32_MIN;
        // std::clog << "Maximazing called!" << "\n";
        auto bestMove = -1;
        for (auto [col, row] : possiblePositions) {
                if (board_[row][col] == '*') {
                    board_[row][col] = id;
                    int currScore = minMax(depth + 1, false, play);
                    maxScore = std::max(maxScore, currScore);
                    board_[row][col] = '*';
                }
        }
        return maxScore;
    } else {
        int minScore = INT32_MAX;
        char opponent = (id == 'X') ? 'O' : 'X';
        // std::clog << "Minimizing called!" << "\n";
        auto bestMove = -1;
        for (auto& [col, row] : possiblePositions) {
                if (board_[row][col] == '*') {
                    board_[row][col] = opponent;
                    int currScore = minMax(depth + 1, true, play);
                    minScore = std::min(minScore, currScore);
                    board_[row][col] = '*';
                }
        }
        return minScore;
    } 
}

int Board::calculateScore(Player& play) {
   int score = 0;
   auto player = play.getPlayerId();
    
    // Check horizontal rows
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            int count2 = 0, count3 = 0, count4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[i][j + k] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                }
            }
            score += count2 * 2 + count3 * 4 + count4 * 16;
        }
    }

    // Check vertical rows
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            int count2 = 0, count3 = 0, count4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[i + k][j] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                }
            }
            score += count2 * 2 + count3 * 4 + count4 * 16;
        }
    }

    // Check diagonal (positive slope) rows
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            int count2 = 0, count3 = 0, count4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[i + k][j + k] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                }
            }
            score += count2 * 2 + count3 * 4 + count4 * 16;
        }
    }

    // Check diagonal (negative slope) rows
    for (int i = 3; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            int count2 = 0, count3 = 0, count4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[i - k][j + k] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                }
            }
            score += count2 * 2 + count3 * 4 + count4;
        }
    }
    // std::clog << "Score is in function: " << score << "\n";
    return score;
}

std::unordered_map<unsigned, unsigned> Board::checkPositions(){
  // In this function I checked possible values for game.
  possiblePositions_.clear();
  for(auto row{ROWS}; row >= 1; --row){
    if(possiblePositions_.size() != 7){
      for(auto col{1}; col <= COLS; ++col){
        if(board_[row][col] == '*'){
          possiblePositions_.insert(std::make_pair(col, row));
        }
     }
    }
  }
  return possiblePositions_;
}

int Board::findBestMove(Player& player){
  int bestMove = -1;  
  auto bestScore = 0;
  auto playerId = player.getPlayerId(); 
  auto possiblePositions = checkPositions();

  for(auto& [col, row] : possiblePositions){
      if(board_[row][col] == '*'){
        board_[row][col] = playerId;

        auto score = minMax(2, true, player);
        
        std::clog << "Minimax values is: " << score << "\n";

        board_[row][col] = '*';

        if(score > bestScore){
          bestScore = score;
          bestMove = col;
        }
      }
  } 
  return bestMove;
}
