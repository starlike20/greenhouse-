# Greenhouse Template Project

## Description

This project implements a greenhouse management simulation using C++ with associated graphical assets. It includes functionalities to handle interactions with graphical elements like characters, enemies, and items in a simulated greenhouse environment.

---

## Features

- Simulation of greenhouse activities with animated characters and enemies.
- Graphical elements for user interaction, including items, insects, and the environment.
- Organized codebase with modular components for ease of understanding and maintenance.

---

## Project Structure

The project is organized into the following components:

### 1. **Source Code**
- `gh.cpp`: Main file containing the core logic of the greenhouse simulation.
- `makefile`: Automates the build process for the project.

### 2. **Graphical Assets**
- Located in the `images/` directory, including:
  - **Characters**: Animations for characters and their actions.
  - **Enemies**: Animations for various insects like spiders, caterpillars, and wasps.
  - **Environment**: Greenhouse visuals and supplementary graphics.

### 3. **Presentation Module**
- `presentation/`: Contains code (`presentation.c` and `presentation.h`) to handle display elements and interactions.

---

## Prerequisites

- **C++ Compiler**: GCC or any compatible C++ compiler.
- **Make Utility**: Required to execute the `makefile` for building the project.
- **Image Viewer**: To view `.bmp` graphical assets if needed.

---

## Installation and Execution

### 1. **Clone the Repository**
```bash
git clone <repository-url>
```

### 2. **Build the Project**
Navigate to the project directory and run:
```bash
make
```

### 3. **Run the Application**
Execute the generated binary:
```bash
./gh
```

---

## Contribution

Contributions are welcome. To contribute:
1. Fork the repository.
2. Create a feature branch: `git checkout -b feature/NewFeature`.
3. Commit your changes: `git commit -m 'Add new feature'`.
4. Push to the branch: `git push origin feature/NewFeature`.
5. Submit a pull request.

---

## Authors
This project was developed by [Your Name/Team].

---

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

