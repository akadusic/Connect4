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
        return calculateScore(play, possiblePositions);
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
        return calculateScore(player, possiblePositions);
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

int Board::calculateScore(Player& player, const std::unordered_map<unsigned, unsigned>& possibleMoves) {
    int score = 0;
    auto blockScore = 0;
    auto id = player.getPlayerId();
    auto opponent = (id == 'X') ? 'O' : 'X';
    auto middlIncremented{false};

    for (int row = 0; row < ROWS; row++) {
        if(board_[row][4] == id && !middlIncremented){
          score += 100;
          middlIncremented = true;
        }
        for (int col = 0; col < COLS - 2; col++) {
            int count = 0;
            auto opponentCount = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row][col + k] == id) {
                    count++;
                } else if(board_[row][col + k] == opponent){
                    opponentCount++;
                } 
            }
            score += (count == 1) ? 10 : (count == 2) ? 20 : (count == 3) ? 500 : (count == 4) ? 10000 : 0;
            blockScore += (opponentCount == 1) ? 10 : (opponentCount == 2) ? 20 : (opponentCount == 3) ? 500 : (opponentCount == 4) ? 10000 : 0;
        }
    }

    for (int row = 0; row < ROWS - 2; row++) {
        if(board_[row][4] == id){
          score += 100;
          middlIncremented = true;
        }
        for (int col = 0; col < COLS; col++) {
            int count = 0;
            auto opponentCount = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row + k][col] == id && !middlIncremented) {
                    count++;
                } else if (board_[row + k][col] == opponent){
                    opponentCount++; 
                } 
            }
            if(opponentCount == 3 && count == 0){
              blockScore += 2000;
            }
            score += (count == 1) ? 10 : (count == 2) ? 20 : (count == 3) ? 500 : (count == 4) ? 10000 : 0;
            blockScore += (opponentCount == 1) ? 10 : (opponentCount == 2) ? 20 : (opponentCount == 3) ? 500 : (opponentCount == 4) ? 10000 : 0;
        }
    }

    for (int row = 0; row < ROWS - 2; row++) {
        for (int col = 0; col < COLS - 2; col++) {
            if(col + 3 == 4 && board_[row][4] == id && !middlIncremented){
                score += 100;
                middlIncremented = true;
            }
            int count = 0;
            auto opponentCount = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row + k][col + k] == id) {
                    count++;
                } else if (board_[row + k][col + k] == opponent){
                    opponentCount++;
                } 
            }
            score += (count == 1) ? 10 : (count == 2) ? 20 : (count == 3) ? 500 : (count == 4) ? 10000 : 0;
            blockScore += (opponentCount == 1) ? 10 : (opponentCount == 2) ? 20 : (opponentCount == 3) ? 500 : (opponentCount == 4) ? 10000 : 0;
        }
    }

   for (int row = 3; row < ROWS; row++) {
        for (int col = 0; col < COLS - 2; col++) {
            if(col + 3 == 4 && board_[row][4] == id && !middlIncremented){
                score += 100;
                middlIncremented = false;
            } 
            int count = 0;
            auto opponentCount = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row - k][col + k] == id) {
                    count++;
                } else if(board_[row - k][col + k] == opponent){
                    opponentCount++;
                } 
            }
            score += (count == 1) ? 10 : (count == 2) ? 20 : (count == 4) ? 500 : (count == 4) ? 10000 : 0;
            blockScore += (opponentCount == 1) ? 10 : (opponentCount == 2) ? 20 : (opponentCount == 3) ? 500 : (opponentCount == 4) ? 10000 : 0;
        }
   }

   score += blockScore;
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

        // auto score = minMax(0, true, player);
        auto score = minMaxAlphaBeta(2, true, player, INT32_MIN, INT32_MAX); 
        std::clog << "Minimax values is: " << score << " in column " << col << "\n";

        board_[row][col] = '*';

        if(score > bestScore){
          std::clog << "Column is: " << col << "\n";
          bestScore = score;
          bestMove = col;
        }
      }
  }
  std::cout << "Best score in find best move is: " << bestScore << "\n";
  return bestMove;
}
