#pragma once

#include <string>

namespace DataModels {
class Player {
  public:
  int playerDrop();
  void playerWin();
  inline int getPlayerId(){ return playerId_; }
  inline std::string getPlayerName(){ return playerName_; }
  inline void setName(const std::string& name){ playerName_ = name; }
  inline void setId(const int id){ playerId_ = id; }
  inline void increaseNumberOfMoves(){ numberOfMoves_ += 1; }
  uint_fast8_t inline numberOfMoves(){ return numberOfMoves_; }

  private:
  std::string playerName_;
  int playerId_;
  uint_fast8_t numberOfMoves_{0};
};
} /* PlayerData */
