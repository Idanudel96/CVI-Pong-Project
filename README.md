# 🏓 Pong Game – LabWindows/CVI

> A classic Pong game developed in **C using LabWindows/CVI**, with modular game logic separated into a dynamically linked library (DLL).

The project was developed as an academic programming assignment focusing on event-driven programming, game-state management, collision handling, keyboard input, GUI interaction, and modular C software design.

---

## ✨ Features
* **Game Modes:** Player vs. Computer & Player vs. Player (1v1).
* **AI Difficulty:** Four distinct levels (Easy, Medium, Hard, Impossible).
* **Physics Engine:** Collision-based ball movement with variable bounce angles depending on the paddle impact position.
* **Dynamic Gameplay:** Gradual increase in ball speed to increase difficulty over time.
* **Custom UI:** Panel-based main menu, mode selection, difficulty settings, and victory screens.
* **Game Flow:** In-game controls for Pause, Restart, and Return-to-Menu.
* **Win Condition:** First player to reach 5 points wins the match.

---

## 🏗️ Software Architecture
The architecture enforces a strict separation between the User Interface (Application Flow) and the Core Gameplay Logic (DLL).

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
