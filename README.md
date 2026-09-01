# Pong Game – LabWindows/CVI

A classic Pong game developed in **C using LabWindows/CVI**, with the core game logic separated into a dynamically linked library (DLL).

The project was developed as an academic programming project and focuses on event-driven programming, collision handling, keyboard input, GUI interaction, AI behavior, and modular C software design.

## Features

- Player vs. Computer and Player vs. Player game modes
- Four AI difficulty levels: Easy, Medium, Hard, and Impossible
- Collision-based ball movement and paddle interaction
- Variable bounce angles based on the ball's impact position on the paddle
- Gradual increase in ball speed during gameplay
- Main menu, game mode selection, difficulty selection, and victory screen
- Pause, restart, and return-to-menu controls
- First player to reach 5 points wins

## Software Architecture

The project separates the user interface and application flow from the core gameplay calculations.

**LabWindows/CVI Application**
- UI panels and events
- Keyboard input
- Game flow
- Canvas rendering

↓

**Game DLL**
- Ball movement
- Collision detection
- AI paddle logic
- Gameplay calculations

## Project Structure

- `PongProject.c` – Main application file handling game execution, UI events, keyboard input, and canvas rendering
- `PongProject.h` – Main application declarations
- `pong_dll.c` – Core gameplay logic and calculations
- `pong_dll.h` – DLL interface and declarations
- `PongProject.uir` – LabWindows/CVI user interface resource file
- `PongProject.prj` – Main LabWindows/CVI project file
- `pong_dll.prj` – LabWindows/CVI project file for the DLL

## Controls

### Player 1

- `Up Arrow` – Move paddle up
- `Down Arrow` – Move paddle down

### Player 2

- `W` – Move paddle up
- `S` – Move paddle down

The graphical interface also includes controls for pausing, restarting, and returning to the main menu.

## How to Run

1. Clone or download the repository.
2. Open `PongProject.prj` using the LabWindows/CVI IDE.
3. Build the project and the DLL.
4. Run the application from LabWindows/CVI.

## Technologies

- C
- LabWindows/CVI
- Dynamically Linked Libraries (DLL)
- Event-driven GUI programming

## Key Engineering Topics

This project provided hands-on experience with:

- Modular C software design
- DLL integration
- Event-driven programming
- Keyboard and UI event handling
- Collision detection and game logic
- AI opponent behavior
- Multi-file project organization
- Debugging in LabWindows/CVI
