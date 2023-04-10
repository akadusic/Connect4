#include <iostream>
#include <Board.hpp>

using namespace std;
using namespace DataModels;

void setPlayers(Player& playerOne, Player& playerTwo){
  cout << "Player One please enter your name: ";
  std::string playerName;
  cin >> playerName;
  playerOne.setName(playerName);
  playerOne.setId('X');
	cout << "Player Two please enter your name: ";
	cin  >> playerName;
  playerTwo.setName(playerName);	
  playerTwo.setId('O');
}

int main(){
	Player playerOne, playerTwo;
  Board board(9,10);
	int trueWidth = 7; 
	int trueLength = 6; 
	int dropChoice, win, full, again;
  string nameOfPlayer;
  
  auto setPlayer = [&](const string&& which, Player& player){
    if(which == "first"){
      cout << "Player One please enter your name: ";
      player.setId('X');
    } else {
      cout << "Player Two please enter your name: ";
      player.setId('O');
    }
    cin >> nameOfPlayer;
    player.setName(nameOfPlayer);
  };

  auto setRandomPlayer = [&](const string&& which, Player& player){
    if(which == "first"){
      player.setName("Player1");
      player.setId('X');
    } else {
      player.setId('O');
      player.setName("Player2");
    }
  };

  auto gameplay = [&]{
    if(board.getChoice() == 1){
      setPlayer("first", playerOne);
      setPlayer("second", playerTwo);
    } else if(board.getChoice() == 2){
      setRandomPlayer("first", playerOne);
      setRandomPlayer("second", playerTwo);
    } else if(board.getChoice() == 3){
      setPlayer("first", playerOne);
      setRandomPlayer("second", playerTwo);
    } else{
      cout << "Something is wrong!" << endl;
    }
  };

	cout << "Let's Play Connect 4" << endl << endl;
  board.displayMenu();

  gameplay();

	full = 0;
	win = 0;
	again = 0;
  board.displayBoard();

	do{
    dropChoice = board.dropPlayer(playerOne);
    board.checkBellow(playerOne, dropChoice);
    board.displayBoard();
    win = board.checkFour(playerOne);

		if(win == 1){
      playerOne.playerWin();
			again = board.restart();
			if(again == 2){
				break;
			}
		}

    dropChoice = board.dropPlayer(playerTwo);
    board.checkBellow(playerTwo, dropChoice);
    board.displayBoard();
    win = board.checkFour(playerTwo);
		if(win == 1){
      playerTwo.playerWin();
			again = board.restart();
			if (again == 2){
				break;
			}
		}
		full = board.fullBoard();
		if(full == 7){
			cout << "The board is full, it is a draw!" << endl;
			again = board.restart();
		}
    
    
    board.increaseNumberOfMoves(playerOne.numberOfMoves(), playerTwo.numberOfMoves());
    std::cout << "Number of moves from both players: " << +board.numberOfMoves() << std::endl;
	} while(again != 2);
}
