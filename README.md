# GPAssignment
![image](https://github.com/DesmondTZX/GPAssignment/blob/main/screenshot.png)

This project is based on "Elite Battle Robot" (https://sketchfab.com/3d-models/elite-battle-robot-02b9587184b746178c85fe9ced67ccd7) by NicolasCalaba (https://sketchfab.com/NicolasCalaba) licensed under CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)

# Setup Guide
## Windows
1. Install Microsoft Visual Studio at https://visualstudio.microsoft.com/downloads/.
2. Go to the project folder and run GPAssignment.sln using Visual Studio. Or open Visual Studio and then the project solution in the project folder.
3. If running the project contained an error saying "Cannot open include file: 'freeglut.h': No such file or directory.", please install the FreeGLUT package following the instructions here: https://www.nuget.org/packages/freeglut.3.2.2.v140.

## macOS
1. Install Homebrew using the terminal by typing /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
2. Install g++ using Homebrew by typing brew install gcc
3. Type g++ main.cpp -o main -lGL -lGLU -lglut into the terminal.
4. Run the executable called “main” via the terminal or double-clicking.

## Linux
1. Install G++ using the terminal.

Ubuntu: `sudo apt install g++`

Fedora: `sudo dnf install gcc-c++`

Arch Linux: `sudo pacman -S g++`

2. Navigate to the project folder that contains “main.cpp”.
3. Type g++ main.cpp -o main -lGL -lGLU -lglut into the terminal.
4. Run the executable called “main” via the terminal or double-clicking.

# Keymaps and Controls
| **Keys**  | **Functions**                                                                 |
|-----------|-------------------------------------------------------------------------------|
| ESC       | Exit the program.                                                             |
| Tab       | Toggle camera transformation (on or off). Will reset transformation when off. |
| Backspace | Toggle viewport between Orthogonal and Projection.                            |
| Space     | Toggle coordinate lines (on or off).                                          |
| [         | Toggle polygon draw mode between LINE and FILL mode.                          |
| ]         | Toggle texture view (on or off).                                              |
| \         | Toggle lighting view (on or off).                                             |
| 1         | Rotate the entire arm in the negative X direction.                            |
| 2         | Rotate the entire arm in the positive X direction.                            |
| 3         | Rotate the entire arm in the negative Z direction.                            |
| 4         | Rotate the entire arm in the positive Z direction.                            |
| 5         | Rotate the lower arm in the negative X direction.                             |
| 6         | Rotate the lower arm in the positive X direction.                             |
| 7         | Rotate the lower arm in the negative Y direction.                             |
| 8         | Rotate the lower arm in the positive Y direction.                             |
| c         | Bend the finger.                                                              |
| o         | Release the finger.                                                           |
| n         | Start walking.                                                                |
| m         | Start running.                                                                |
| x         | Stop walking or Stop running.                                                 |
| p         | Start shooting.                                                               |
| l         | Continue shooting.                                                            |
| v         | Open the Sword.                                                               |
| b         | Switch between the sword and scythe.                                          |
| j         | Swing the sword or scythe.                                                    |
| r         | Reset the animation.                                                          |
| w         | (If camera transformation is on.) Move the camera up.                         |
|           | {If lighting mode is on.) Move the light source up.                           |
| a         | (If camera transformation is on.) Move the camera left.                       |
|           | {If lighting mode is on.) Move the light source left.                         |
| s         | (If camera transformation is on.) Move the camera down.                       |
|           | {If lighting mode is on.) Move the light source down.                         |
| d         | (If camera transformation is on.) Move the camera right.                      |
|           | {If lighting mode is on.) Move the light source right.                        |
| -         | (If camera transformation is on.) Move the camera to the far plane.           |
|           | {If lighting mode is on.) Move the light source to the far plane              |
| =         | (If camera transformation is on.) Move the camera to the near plane.          |
|           | {If lighting mode is on.) Move the light source to the near plane.            |

| **Mouse Controls** | **Functions**                                        |
|--------------------|------------------------------------------------------|
| Scroll wheel up    | (If camera transformation is on.) Zoom in.           |
| Scroll wheel down  | (If camera transformation is on.) Zoom out.          |
| Click and drag     | (If camera transformation is on.) Rotate the camera. |
