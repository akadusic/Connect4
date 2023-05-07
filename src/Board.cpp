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

// int Board::calculateScore(Player& player, const std::unordered_map<unsigned, unsigned>& possibleMoves) {
    /* int score = 0;
    auto blockScore = 0;
    auto player = play.getPlayerId();
    auto opponent = (player == 'X') ? 'O' : 'X';

    for (int col = 0; col < COLS; col++) {
        if (board_[0][col] == '*') {
            int delta = std::abs(col - COLS / 2);
            score += 10 - delta;
        }
    }

    for (const auto& [col, row] : possibleMoves) {
        // Check horizontal row
        if (col < 4) {
            int count2 = 0, count3 = 0, count4 = 0;
            auto block2 = 0, block3 = 0, block4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row][col + k] == player) {
                    if (k < 3) {
                        count2++;
                    } else if (k < 4) {
                        count3++;
                    } else if (k < 5){
                        count4++;
                    }
                }  else if(board_[row][col + k] == opponent){
                  if(k < 2){
                    block2++;
                  } else if(k < 3){
                    block3++;
                  } else if (k < 4){
                    block4++;
                  }
                }
            }
            if(count2 == 2){
              score += 20;
            } else if(count3 == 3){
              score += 200;
            } else if(count4 == 4) {
              score += 2000;
            }
            if(block2 == 2){
              blockScore -= 20; 
            } else if(block3 == 3){
              blockScore -= 200;
            } else if (blockScore == 4){
              blockScore -= 2000;
            }
        }

        // Check vertical row
        if (row < 3) {
            int count2 = 0, count3 = 0, count4 = 0;
            auto block2 = 0, block3 = 0, block4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row + k][col] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                } else if(board_[row + k][col] == opponent){
                  if(k < 2){
                    block2++;
                  } else if(k < 3){
                    block3++;
                  } else {
                    block4++;
                  }
                }
            }
            if(count2 == 2){
              score += 20;
            } else if(count3 == 3){
              score += 200;
            } else {
              score += 2000;
            }
            
            if(block2 == 2){
              blockScore -= 20;
            } else if(block3 == 3){
              blockScore -= 200;
            } else {
              blockScore -= 2000;
            }
        }

        // Check diagonal (positive slope) row
        if (row < 3 && col < 4) {
            int count2 = 0, count3 = 0, count4 = 0;
            auto block2 = 0, block3 = 0, block4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row + k][col + k] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                } else if(board_[row + k][col + k] == opponent){
                  if(k < 2){
                    block2++;
                  } else if(k < 3){
                    block3++;
                  } else {
                    block4++;
                  }
                }
            }
            if(count2 == 2){
              score += 20;
            } else if(count3 == 3){
              score += 200;
            } else {
              score += 2000;
            }
            if(block2 == 2){
              blockScore -= 20;
            } else if(block3 == 3){
              blockScore -= 200;
            } else {
              blockScore -= 2000;
            }
        }

        // Check diagonal (negative slope) row
        if (row >= 3 && col < 4) {
            int count2 = 0, count3 = 0, count4 = 0;
            auto block2 = 0, block3 = 0, block4 = 0;
            for (int k = 0; k < 4; k++) {
                if (board_[row - k][col + k] == player) {
                    if (k < 2) {
                        count2++;
                    } else if (k < 3) {
                        count3++;
                    } else {
                        count4++;
                    }
                } // else if(board_[row - k][col - k] == opponent){
                   if (k < 2){
                    block2++;
                  } else if(k < 3){
                    block3++;
                  } else {
                    block4++;
                  } */
                // }
            /* }
            if(count2 == 2){
              score += 20;
            } else if(count3 == 3){
              score += 200;
            } else {
              score += 2000;
            } */
            /* if(block2 == 2){
              blockScore -= 20;
            } else if(block3 == 3){
              blockScore -= 200;
            } else {
              blockScore -= 2000;
            } */
        /* }
    }

    score -= blockScore;

    return score; */   
// }

int Board::calculateScore(Player& play, const std::unordered_map<unsigned, unsigned>& possiblePositions) {
    int score = 0;
    // auto player = play.getPlayerId(); 
    auto player = '*';
    // Check for horizontal win
    for (auto [col, row] : possiblePositions) {
            if (board_[row][col] == player && board_[row][col+1] == player &&
                board_[row][col+2] == player && board_[row][col+3] == player) {
                score += 100;
            }
    }
    
    // Check for vertical win
    for (auto [col, row] : possiblePositions) {
            if (board_[row][col] == player && board_[row+1][col] == player &&
                board_[row+2][col] == player && board_[row+3][col] == player) {
                score += 100;
            }
    }
    
    // Check for diagonal win (top-left to bottom-right)
    for (auto [col, row] : possiblePositions) {
            if (board_[row][col] == player && board_[row+1][col+1] == player &&
                board_[row+2][col+2] == player && board_[row+3][col+3] == player) {
                score += 100;
            }
    }
    
    // Check for diagonal win (bottom-left to top-right)
    for (auto [col, row] : possiblePositions) {
            if (board_[row][col] == player && board_[row-1][col+1] == player &&
                board_[row-2][col+2] == player && board_[row-3][col+3] == player) {
                score += 100;
            }
    }
    
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
          std::clog << "Column is: " << col << "\n";
          bestScore = score;
          bestMove = col;
        }
      }
  }
  std::cout << "Best score in find best move is: " << bestScore << "\n";
  return bestMove;
}
