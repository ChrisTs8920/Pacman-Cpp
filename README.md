# Pacman game made with C++ and PDcurses.
This game was made during my C++ course in University.
##How to Run
Windows only.
Compile using: g++ *.cpp -o main.exe -lpdcurses
Run using: ./main.exe
main.exe requires map.txt and HiScore.dat files in same directory.
##How to Play
The player 'P' needs to collect 10 jewels 'J' scattered around the map.
Once the player collects all jewels, a parchment 'M' spawns on the map.
If the player collects the parchment, he wins.
If the monsters 'G' and 'T' catch the player, the game is over.