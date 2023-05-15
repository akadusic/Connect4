#include <Board.hpp>
#include <cstdint>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <cmath>
#include <vector>

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
  
  auto minmaxHuman = [&]{
    if(player.getPlayerId() == 'X') humanPlayers();
    else minimax();
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
    else if(choice_ == 5) minmaxHuman();
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
  std::cout << "Human vs minimax (5)" << "\n";
  std::cout << "======================================" << std::endl;
  std::cout << "Choice: ";
  std::cin >> choice_;
}

int Board::minMax(unsigned int depth, bool maximizingPlayer, Player& play) {
   auto id = play.getPlayerId(); 
   auto possiblePositions = checkPositions();
  
    if (depth == MAX_DEPTH || fullBoard()) {
        return calculateScore(id, possiblePositions);
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

int Board::minMaxAlphaBeta(unsigned int depth, bool maxPlayer, Player& player, int alpha, int beta){
   auto id = player.getPlayerId(); 
   auto possiblePositions = checkPositions();
  
    if (depth == MAX_DEPTH || fullBoard()) {
        return calculateScore(id, possiblePositions);
    }

    if (maxPlayer) {
        int maxScore = INT32_MIN;
        // std::clog << "Maximazing called!" << "\n";
        auto bestMove = -1;
        for (auto [col, row] : possiblePositions) {
                if (board_[row][col] == '*') {
                    board_[row][col] = id;
                    int currScore = minMax(depth + 1, false, player);
                    maxScore = std::max(maxScore, currScore);
                    board_[row][col] = '*';
                    alpha = std::max(alpha, currScore);
                    if(beta <= alpha){
                      break;
                    }
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
                    int currScore = minMax(depth + 1, true, player);
                    minScore = std::min(minScore, currScore);
                    board_[row][col] = '*';
                    beta = std::min(beta, currScore);
                    if(beta <= alpha){
                      break;
                    }
                }
        }
        return minScore;
    } 
}

int Board::calculateScore(const char id, const std::unordered_map<unsigned, unsigned>& possibleMoves) {
    int score = 0;
    auto blockScore = 0;
    auto middlIncremented{false};
    auto opponent = (id == 'X') ? 'O' : 'X';

    std::vector<int> helper;
    for(auto row{ROWS}; row >= 1; --row){
      helper.push_back(board_[row][4]);

    }

    auto countt = std::count(helper.begin(), helper.end(), id);
    
    score += countt * 3; 

    for (int row = ROWS; row >= 1; --row) {
        std::vector<char> els;
        for (int col = 1; col <= COLS; col++) {
            els.push_back(board_[row][col]); 
        }

        auto count = std::count(els.begin(), els.end(), id);
        auto opponentCount = std::count(els.begin(), els.end(), opponent);
        auto emptys = std::count(els.begin(), els.end(), '*');

        if(count == 4){
          score += 100;
        } else if(count == 3 && emptys == 1){
          score += 5;
        } else if(count == 2 && emptys == 2){
          score += 2;
        } 
        
        if (opponentCount == 3 && emptys == 1){
          score -= 1000;
        } 
    }

    for (int col = 1; col <= COLS; ++col) {
        std::vector<int> els;
        for (int row = ROWS; row >= 1; --row) {
          els.push_back(board_[row][col]);
        }

        auto count = std::count(els.begin(), els.end(), id);
        auto opponentCount = std::count(els.begin(), els.end(), opponent);
        auto emptys = std::count(els.begin(), els.end(), '*');
        
        if(count == 4){
          score += 100;
        } else if(count == 3 && emptys == 1){
          score += 5;
        } else if(count == 2 && emptys == 2){
          score += 2;
        } 
        
        if (opponentCount == 3 && emptys == 1){
          score -= 1000;
        }
    }


    for (int row = ROWS-3; row >= 1; --row) {
        std::vector<std::vector<int>> vecOfDiagonals;
        std::vector<int> els;

        for (int col = 1; col <= COLS-3; col++) {
          for(auto i{0}; i < 4; ++i){
            els.push_back(board_[row + i][col + i]);
          }
        }
        vecOfDiagonals.emplace_back(els);
        
        for(const auto& el : vecOfDiagonals){
          auto count = std::count(el.begin(), el.end(), id);
          auto opponentCount = std::count(el.begin(), el.end(), opponent);
          auto emptys = std::count(el.begin(), el.end(), '*');
          
          if(count == 4){
            score += 100;
          } else if(count == 3 && emptys == 1){
            score += 5;
          } else if(count == 2 && emptys == 2){
            score += 2;
          } 
        
          if (opponentCount == 3 && emptys == 1){
            score -= 1000;
          }
        }
    }

   for (int row = ROWS-3; row >= 1; --row) {
     std::vector<std::vector<int>> vecOfDiagonals; 
     std::vector<int> els;

     for (int col = COLS; col >= COLS - 3; --col) {
        for(auto i{0}; i < 4; ++i){
            els.push_back(board_[row + i][col - i]);
          } 
        }
     vecOfDiagonals.push_back(els);

    for(const auto& el : vecOfDiagonals){
          auto count = std::count(el.begin(), el.end(), id);
          auto opponentCount = std::count(el.begin(), el.end(), opponent);
          auto emptys = std::count(el.begin(), el.end(), '*');
          
          if(count == 4){
            score += 100;
          } else if(count == 3 && emptys == 1){
            score += 5;
          } else if(count == 2 && emptys == 2){
            score += 2;
          } 
        
          if (opponentCount == 3 && emptys == 1){
            score -= 1000;
          }
        }
   }

   return std::abs(score);    
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
  auto bestScore = INT32_MIN;
  auto playerId = player.getPlayerId(); 
  auto possiblePositions = checkPositions();

  for(auto& [col, row] : possiblePositions){
      if(board_[row][col] == '*'){
        board_[row][col] = playerId;

        auto score = minMax(2, true, player); 

        // auto score = minMaxAlphaBeta(0, true, player, INT8_MIN, INT8_MAX);
        std::clog << "Minimax values is: " << score << " in column " << col << "\n";
        

        board_[row][col] = '*';

        if(score > bestScore){
          bestScore = score;
          bestMove = col;
        }
      }
  }
  return bestMove;
}
