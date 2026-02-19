# Mathematical Function Visualizer (C++)

## Overview
This application is a mathematical engine designed for real-time function visualization and analysis. Developed in C++ using the **graphics.h (WinBGIm)** library, the project focuses on manual coordinate mapping and algorithmic expression parsing.

## Key Features
* **Custom Expression Parser:** A dual-stack implementation that evaluates complex mathematical strings by handling operator precedence and nested parentheses .
* **Function Support:** Native handling of variables (`x`) and transcendent functions such as `sin`, `cos`, `exp`, `ln`, `abs`, and `sqrt`.
* **Interactive Viewport:** Real-time navigation allowing users to pan via arrow keys and zoom using the `=` and `-` keys.
* **Numerical Analysis:** Integrated module for scanning intervals to automatically detect global minima and maxima.
* **Persistence:** Support for saving and loading function lists and color configurations to/from external text files.

## Technical Details
* **Paradigm:** Procedural Programming .
* **Graphics Library:** WinBGIm (graphics.h).
* **Input Handling:** Manual event management for mouse interaction and keyboard buffer polling .
* **Safety Logic:** Robust error checking for mathematical undefined states (e.g., division by zero) using `NaN` verification .

## Academic Context
Developed as a final project for the **Introduction to Programming** course at the **Faculty of Computer Science, UAIC (FII)**. This project serves as a demonstration of fundamental algorithmic logic and manual UI rendering before transitioning to Object-Oriented Programming.
