#include <iostream>
#include <Board.hpp>

using namespace std;
using namespace DataModels;

int main()
{
	Player playerOne, playerTwo;
  Board board(9,10);
	int trueWidth = 7; 
	int trueLength = 6; 
	int dropChoice, win, full, again;

	cout << "Let's Play Connect 4" << endl << endl;
  board.displayMenu();
  
  switch (+board.getChoice()) {
    case 1: std::cout << "Do the job with both random players!" << std::endl; break;
    case 2: std::cout << "Do the job with one random player!" << std::endl; break;
    case 3: std::cout << "Normal game with human players!" << std::endl; break;
    default: std::cout << "Budalica!" << std::endl;
  } 

	cout << "Player One please enter your name: ";
  std::string playerName;
  cin >> playerName;
  playerOne.setName(playerName);
  playerOne.setId('X');
	cout << "Player Two please enter your name: ";
	cin  >> playerName;
  playerTwo.setName(playerName);	
  playerTwo.setId('O');

	full = 0;
	win = 0;
	again = 0;
  board.displayBoard();
	do
	{
    dropChoice = board.dropPlayer(playerOne);
    board.checkBellow(playerOne, dropChoice);
    board.displayBoard();
    win = board.checkFour(playerOne);
		if ( win == 1 )
		{
      playerOne.playerWin();
			again = board.restart();
			if (again == 2)
			{
				break;
			}
		}

    dropChoice = board.dropPlayer(playerTwo);
    board.checkBellow(playerTwo, dropChoice);
    board.displayBoard();
    win = board.checkFour(playerTwo);
		if ( win == 1 )
		{
      playerTwo.playerWin();
			again = board.restart();
			if (again == 2)
			{
				break;
			}
		}
		full = board.fullBoard();
		if ( full == 7 )
		{
			cout << "The board is full, it is a draw!" << endl;
			again = board.restart();
		}
    
    
    board.increaseNumberOfMoves(playerOne.numberOfMoves(), playerTwo.numberOfMoves());
    std::cout << "Number of moves from both players: " << +board.numberOfMoves() << std::endl;
	} while ( again != 2 );

  return 0;
}
