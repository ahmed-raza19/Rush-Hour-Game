# 🚕 Rush Hour Game

**Single-Player Taxi & Delivery Simulation (C++)**  
CS-1004 Object Oriented Programming | FAST-NUCES

---

## 📌 Project Overview

**Rush Hour** is a single-player 2D game developed in **C++** using **core Object-Oriented Programming concepts**.  
The player switches between **Taxi Driver** and **Delivery Driver** roles to complete tasks, earn money, refuel the car, and avoid obstacles — all within a limited time.

This project was built as a **Semester Project** to practically implement OOP concepts studied during the course.

---

## 🎯 Objectives

- Apply **Object-Oriented Programming** in a complete game
- Use **inheritance, polymorphism, composition, and association**
- Implement file handling for persistent high scores
- Manage game logic, scoring, fuel, and time constraints
- Build an interactive game using OpenGL utilities

---

## 🎮 Game Modes

### 🚖 Taxi Driver
- Pick up passengers and drop them at destinations
- Earn points and money for successful trips
- Penalties for collisions

### 📦 Delivery Driver
- Pick up delivery packages and drop them off
- Higher reward but higher penalties
- No passengers shown in this mode

Players can **switch roles mid-game** at designated stations.

---

## 🧠 Core OOP Concepts Used

- **Inheritance:** Different driver roles and vehicles  
- **Polymorphism:** Unified handling of taxi & delivery logic  
- **Composition:** Board contains cars, passengers, obstacles  
- **Association:** Interaction between player, tasks, and board  

---

## 🗂️ Project Structure

- main.cpp              # Main game loop and logic
- Board.h               # Board class definition
- Board.cpp             # Board rendering and management
- util.h                # Graphics utility declarations
- util.cpp              # Graphics utility implementations
- install-libraries.sh  # OpenGL dependency installer
- Makefile              # Build instructions
- assets/               # Images, music, and other resources


## ⚙️ Gameplay Features

- Arrow-key based movement  
- Fuel management system  
- Dynamic obstacles and moving cars  
- Score & money tracking  
- Real-time timer (3 minutes)  
- Role switching during gameplay  
- Increasing difficulty over time  

---

## 🏁 Winning & Ending Conditions

The game ends when **any** of the following occurs:

- 3 minutes are completed  
- Fuel runs out  
- Score drops below zero  
- Player scores **100+ points** (win condition)

---

## 🙌 Acknowledgment

Special thanks to our teacher **Kainat Iqbal** for excellent teaching and  
**Ammar Masood** for guidance during lab sessions.

---

## 🚀 Final Note

This project helped solidify OOP concepts by applying them to a real, interactive game — making learning both practical and enjoyable.
