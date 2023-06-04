Requirements for running the game:
  - CMake at least version 3.0.0
  - C++ at least standard C++11
  - Clang or GCC compiler (in my case I have used clang++ version 15.0.7 or gcc 12.2.1)

How to build and run the game:
  - Extract game where you want
  - Go to the game directory
  - Run: mkdir build
  - Run: cmake ..
  - Run: make ..
  - ./connect4

How to play the game:
  After you start the game with ./connect4 command, you will have menu with the all options for game.
  You need to insert number to play appropriate mode of game.
  If you choose minimax algorithm or minimax with alpha beta prunning you also need to insert depth for algorithm.
  The max depth is set to 7. So, if you want to run the game with algorithm depth 7 you need to insert 0. If you want to run it with depth 6
  you need to insert 1 and so on.
  I didn't do any benchmarks since in my code I am using sleep function and I don't know would it be precise.
   
