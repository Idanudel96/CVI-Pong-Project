# Pong Game – LabWindows/CVI

A classic Pong game developed in **C using LabWindows/CVI**, with modular game logic separated into a dynamically linked library (DLL).

The project was developed as an academic programming project and focuses on event-driven programming, game-state management, collision handling, keyboard input, GUI interaction, and modular C software design.

## Features

- Player vs. Computer and Player vs. Player game modes
- Four AI difficulty levels: Easy, Medium, Hard, and Impossible
- Collision-based ball movement and paddle interaction
- Variable bounce angles based on paddle impact position
- Gradual increase in ball speed during gameplay
- Main menu, game mode selection, difficulty selection, and victory screen
- Pause, restart, and return-to-menu controls
- First player to reach 5 points wins

## Software Architecture

The project separates the user interface and application flow from the core gameplay logic.

```text
LabWindows/CVI Application
        |
        |-- UI Panels and Events
        |-- Keyboard Input
        |-- Game State Management
        |-- Canvas Rendering
        |
        v
      Game DLL
        |
        |-- Ball Movement
        |-- Collision Detection
        |-- AI Paddle Logic
        |-- Gameplay Calculations
Project Structure
PongProject.c – Main application file handling the game loop, UI events, keyboard input, and canvas rendering
PongProject.h – Main application declarations
pong_dll.c – Core gameplay logic and calculations
pong_dll.h – DLL interface and declarations
PongProject.uir – LabWindows/CVI user interface resource file
PongProject.prj – LabWindows/CVI project file
pong_dll.prj – LabWindows/CVI project file for the DLL
Controls
Player 1
Up Arrow – Move paddle up
Down Arrow – Move paddle down
Player 2
W – Move paddle up
S – Move paddle down

The graphical interface also includes controls for pause, restart, and returning to the main menu.

How to Run
Clone or download the repository.
Open PongProject.prj in the LabWindows/CVI IDE.
Build the project and the DLL.
Run the application from LabWindows/CVI.
Technologies
C
LabWindows/CVI
DLL-based modular architecture
Event-driven GUI programming
Key Engineering Topics

This project provided hands-on experience with:

Modular C software design
DLL integration
Event-driven programming
Keyboard and UI event handling
Collision detection and game logic
Multi-file project organization
Debugging in LabWindows/CVI
