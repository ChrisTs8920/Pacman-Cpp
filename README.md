# Pacman - C++ and PDcurses

## Description

This project is a pacman style game made using C++ and PDcurses library. It includes the player 'P', and 2 monsters, Traal 'T' and Gnome 'G'. The goal of the player is to find the parchment while the monsters chase him. The algorithm that is used for the monsters to track the player is the BFS (Best-first Search) algorithm.

*This game was made during my C++ course in University.*

## How to Run

1. Compile using: ```g++ *.cpp -o main.exe -lpdcurses```
2. Run using: ```./main.exe```

main.exe requires map.txt and HiScore.dat files in same directory.

## How to Play

1. Use arrow keys to move, spacebar to skip turn (if using turn based movement).
2. - The player 'P' needs to collect 10 jewels 'J' scattered around the map.
   - Once the player collects all jewels, a parchment 'M' spawns on the map.
   - If the player collects the parchment, he wins.
   - If the monsters 'G' and 'T' catch the player, the game is over.

*Scores are kept in the HiScore.dat file.*

## Screenshots

![Screenshot1](https://github.com/ChrisTs8920/Pacman-Cpp/blob/main/screenshots/Screenshot_1.jpg?raw=true)
