<h1>N Body Simulator</h1>
A basic 2D N-body gravity simulation using SFML. It models gravitational interaction and motion of celestial bodies under Newtonian physics.

## Setup
This project uses SFML 3 for graphics and physics visualization.<br>
### 1. SFML Installation
To run the code, make sure SFML is installed. Checkout<br>
[how to setup sfml for visual studio code and mingw by OURABIG](https://www.youtube.com/watch?v=jKbWBcVPLWQ)<br>
It shows SFML 2.5.1, but the same steps work for SFML 3.0 too.

### 2. Clone the repository
```bash
git clone https://github.com/HarshadaRaut234/N-body_simulator.git
cd N-body_simulator
```
### 3. Build and run (manually)
```bash
cd coding
g++ main.cpp -std=c++17 -I../src/include -L../src/lib -lsfml-graphics -lsfml-window -lsfml-system -o ../main.exe
../main.exe
```
## Input format
Number of bodies on 1st line.<br>
Next each line in `input.txt` represents one body with:

    mass radius x-coordinate y-coordinate velocity angle

All values are in SI units.

## Future Improvements
1. Switch to Verlet integration for better accuracy (uses Euler currently).<br>
2. Improve collision handling.
