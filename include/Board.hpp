#pragma once

#include <Player.hpp>
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

  private:
  std::vector<std::vector<char>> board_;
  // char board[][10];
};
} /* DataModels */
