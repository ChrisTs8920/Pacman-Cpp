# Pacman game made with C++ and PDcurses.
This game was made during my C++ course in University.
## How to Run
1. Compile using: ```g++ *.cpp -o main.exe -lpdcurses```
2. Run using: ```./main.exe```

main.exe requires map.txt and HiScore.dat files in same directory.
## How to Play
1. Use arrow keys to move, spacebar to skip turn (if using turn based movement).
2. i. The player 'P' needs to collect 10 jewels 'J' scattered around the map.\
ii. Once the player collects all jewels, a parchment 'M' spawns on the map.\
iii. If the player collects the parchment, he wins.\
iv. If the monsters 'G' and 'T' catch the player, the game is over.

Scores are kept in the HiScore.dat file.
