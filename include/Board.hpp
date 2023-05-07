#pragma once

#include <Player.hpp>
#include <cstdint>
#include <vector>
#include <optional>
#include <unordered_map>

namespace DataModels {

class Board {
  public:
  static constexpr int ROWS = 6;
  static constexpr int COLS = 7;
  static constexpr int MAX_DEPTH = 4;

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
  inline int getChoice(){ return choice_; }
  int calculateScore(Player&, const std::unordered_map<unsigned, unsigned>&); 
  int minMax(unsigned int, bool, Player&);
  std::unordered_map<unsigned, unsigned> checkPositions();
  int findBestMove(Player&);

  private:
  std::vector<std::vector<char>> board_;
  uint_fast8_t sumOfMoves_{0};
  int choice_{0};
  std::unordered_map<unsigned, unsigned> possiblePositions_; 
};
} /* DataModels */
