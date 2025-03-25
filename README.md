# Iceman Game

## Overview

Iceman is a C++ game where players control a miner (the Iceman) who must dig through an oil field to collect barrels of oil while avoiding obstacles and protesters. This project demonstrates object-oriented programming principles, inheritance, and GUI development in C++.

## Key Features

- Multiple levels with increasing difficulty
- Player-controlled Iceman character
- Regular and Hardcore Protesters as opponents
- Various collectible items: Oil Barrels, Gold Nuggets, Sonar Kits, and Water
- Obstacles like Boulders and Ice
- Squirt gun mechanic for deterring protesters

## Game Objects

- Iceman (player character)
- Regular Protesters
- Hardcore Protesters
- Boulders
- Ice
- Oil Barrels
- Gold Nuggets
- Sonar Kits
- Water Pools
- Squirts (from the squirt gun)

## Technical Highlights

- Object-Oriented Design: Utilizes classes and inheritance for game objects
- GraphObject Base Class: All game objects derive from this class for graphical representation
- StudentWorld Class: Manages the game world and object interactions
- Tick-based Gameplay: Game logic updates on each tick
- File I/O: Saves and loads game data
- Sound Effects: Implements various sound cues for game events

## How to Play

1. Use arrow keys to move the Iceman
2. Collect all Oil Barrels to complete a level
3. Use the squirt gun (spacebar) to deter protesters
4. Collect Gold Nuggets to bribe protesters
5. Use Sonar Kits to reveal hidden objects
6. Avoid or strategically use Boulders

## Building the Game

### Prerequisites
- C++ compiler
- Required libraries (details in the full documentation)

### Steps
1. Clone the repository
2. Navigate to the project directory
3. Compile the source files
4. Run the executable

## Project Structure

- `Actor.h` and `Actor.cpp`: Declarations and implementations of actor classes
- `StudentWorld.h` and `StudentWorld.cpp`: Game world class declaration and implementation
- `GameConstants.h`: Game-wide constants
- `GraphObject.h`: Base class for graphical objects

## Acknowledgments

This project was developed as part of a programming course, demonstrating advanced C++ concepts and game development techniques.
