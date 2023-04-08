#pragma once

#include <Player.hpp>
#include <cstdint>
#include <vector>

namespace DataModels {

class Board {
  public:
  Board(int row, int column) : board_(row, std::vector<char>(column, '.')){}
  void checkBellow(Player&, const int);
  void displayBoard();
  int fullBoard();
  int checkFour(Player&);
  int restart();
  int dropPlayer(Player&);
  void displayMenu();

  inline void increaseNumberOfMoves(uint_fast8_t firstPlayerNum, uint_fast8_t secondPlayerNum){ 
      sumOfMoves_ = firstPlayerNum + secondPlayerNum;
  }
  inline uint_fast8_t numberOfMoves(){ return sumOfMoves_; }

  private:
  std::vector<std::vector<char>> board_;
  uint_fast8_t sumOfMoves_{0};
  uint_fast8_t choice_{0};
};
} /* DataModels */
