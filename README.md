# Mathematical Function Visualizer (C++)

## Overview
This application is a mathematical engine designed for real-time function visualization and analysis. Developed in C++ using the **graphics.h (WinBGIm)** library, the project focuses on manual coordinate mapping and algorithmic expression parsing.

## Key Features
* **Custom Expression Parser:** A dual-stack implementation that evaluates complex mathematical strings by handling operator precedence and nested parentheses [cite: 39-40, 56-62].
* **Function Support:** Native handling of variables (`x`) and transcendent functions such as `sin`, `cos`, `exp`, `ln`, `abs`, and `sqrt` [cite: 18-24, 45-54].
* **Interactive Viewport:** Real-time navigation allowing users to pan via arrow keys and zoom using the `=` and `-` keys [cite: 199-210].
* **Numerical Analysis:** Integrated module for scanning intervals to automatically detect global minima and maxima [cite: 72, 78-85].
* **Persistence:** Support for saving and loading function lists and color configurations to/from external text files [cite: 63-68].



## Technical Details
* **Paradigm:** Procedural Programming [cite: 1-6, 169].
* **Graphics Library:** WinBGIm (graphics.h)[cite: 1].
* **Input Handling:** Manual event management for mouse interaction and keyboard buffer polling [cite: 189, 212-213].
* **Safety Logic:** Robust error checking for mathematical undefined states (e.g., division by zero) using `NaN` verification [cite: 7-8].

## Academic Context
Developed as a final project for the **Introduction to Programming** course at the **Faculty of Computer Science, UAIC (FII)**. This project serves as a demonstration of fundamental algorithmic logic and manual UI rendering before transitioning to Object-Oriented Programming.
