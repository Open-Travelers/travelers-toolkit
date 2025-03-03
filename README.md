# Traveler's Toolkit
![All builds passing](https://github.com/Open-Travelers/travelers-toolkit/actions/workflows/cmake-multi-platform.yml/badge.svg)

This is a level viewer for Crash Bandicoot: The Wrath of Cortex.
It's built in C++ using ImGui and OpenGL, with SFML used as a window library.
SFML is not in the repo, it gets cloned as part of the CMake generation process. I know that's cursed - might replace the library with something that has a sensible build system like GLFW eventually.

One day I'd like for this to become a fully fledged level-editor, but a level-viewer is an important milestone.

NO RENDERER IMPLEMENTED YET. CURRENTLY ONLY SERVES TO PARSE INTERNAL LEVEL AND CHARACTER DATA.

## Currently supported builds
- E3 Demo
- Gamceube retail

Will expand once more start getting supported.

## Building the thing
1. Get CMake and install git. The GitHub desktop app isn't enough, you need to be able to open a command prompt, type in `git --version` and get a sensible output.
2. [LINUX ONLY] You need to grab the required dependencies for SFML. Check [this SFML repo](https://github.com/SFML/cmake-sfml-project), in the "How to Use" section.
3. Clone this repo recursively `git clone --recursive https://github.com/Open-Travelers/travelers-toolkit.git`. If you don't use the --recursive flag, you will not get a full repo due to submodules.
4. Configure using cmake `cmake -B build/`, this will create a build/ directory with your default generator.
5. Compile using cmake `cmake --build build/`, this should invoke whichever default generator is set on your platform.

## Using the thing
1. Extract the contents of an ISO for the version of the game you wish to poke around in.
2. Launch the program and select the extracted folder
3. Select a level from the level window (if a level's name is in red text, its folder hasn't been found) and poke around
