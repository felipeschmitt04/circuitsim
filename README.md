# CircuitSim - Logic Circuit Simulator
> A digital logic circuit simulator with a graphical interface and intelligent wire routing, developed in C++/Qt as the final project for the Programming Languages course.

## 📜 About the Project

CircuitSim is a desktop application that allows users to build and simulate digital logic circuits through a visual and interactive interface. The project was designed to apply and demonstrate advanced concepts of Object-Oriented Programming (OOP), Graphical User Interface (GUI) design, and, most notably, the implementation of pathfinding algorithms to solve practical routing problems.

## ✨ Key Features

* **Intuitive Graphical Interface:** Built with the Qt framework, featuring a custom dark theme, a `QToolBox` for component selection, and a main canvas with a grid for easy alignment.
* **Real-Time Simulation:** Circuit logic states are propagated and updated instantly. Toggling an input state immediately reflects the results in outputs and connections without needing a "Simulate" button.
* **A-Star (A\*) Pathfinding Routing:** The project's standout feature. Connections between components are not just straight lines; they are calculated using a custom **A-Star (A\*) algorithm** implementation that:
    * Finds the shortest path between two points.
    * **Automatically avoids** other components by treating them as obstacles.
    * Recalculates all affected routes whenever a component is moved.
    * Ensures wires connect to components using orthogonal segments for a cleaner look.
* **Direct Manipulation:**
    * **Add:** Drag and drop components from the toolbox onto the canvas.
    * **Move:** Click and drag any component across the canvas.
    * **Connect:** Create connections by dragging from an output pin to a valid input pin.
    * **Delete:** Remove components or connections using the "Trash" tool.
* **Persistence:** The complete circuit state (components, positions, and connections) can be saved to a custom file format (`.csim`) and loaded later, allowing for ongoing work.

## 🛠️ Technologies and Applied Concepts

* **Language:** C++
* **Framework:** Qt 6 (Widgets, Painter, Signals & Slots, File I/O)
* **Architecture:**
    * **Object-Oriented Programming (OOP):** Intensive use of inheritance (`Component` hierarchy) and polymorphism (`draw()` and `calculateOutput()` functions).
    * **Separation of Concerns:** Clear division between circuit logic (Core) and graphical representation (GUI).
* **Core Algorithm:**
    * **A* (A-Star):** Implemented from scratch for pathfinding on a 2D grid with obstacles.
* **Data Structures:** `std::vector` and `std::map` for managing circuit elements and A* algorithm nodes.
* **Styling:** QSS (Qt Style Sheets) for the dark theme and widget customization.

## 🚀 How to Build and Run

1.  **Prerequisites:**
    * Qt Creator with a configured C++ compiler (MinGW, MSVC, or Clang).
    * Qt 6 Framework installed.
2.  **Steps:**
    * Clone this repository: `git clone https://github.com/felipeschmitt04/circuitsim`
    * Open the `CircuitSim.pro` project file with Qt Creator.
    * Configure the build "Kit" if prompted.
    * Build and run the project (`Play` button or `Ctrl+R`).

## 👨‍💻 Developer

* **Name:** Felipe Luan Schmitt
* **Contact:** felipe.lschmitt04@gmail.com
* **GitHub:** [github.com/felipeschmitt04](https://github.com/felipeschmitt04)
