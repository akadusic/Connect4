#pragma once

#include <string>

namespace DataModels {
class Player {
  public:
  int playerDrop();
  void playerWin();
  inline int getPlayerId(){ return playerId_; }
  inline std::string getPlayerName(){ return playerName_; }
  inline void setName(const std::string& name){
    playerName_ = name;
  }
  inline void setId(const int id){
    playerId_ = id;
  }

  private:
  std::string playerName_;
  int playerId_;
};
} /* PlayerData */
