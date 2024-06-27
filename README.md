# Traveler's Toolkit

This is a level viewer for Crash Bandicoot: The Wrath of Cortex.
It's built in C++ using ImGui and OpenGL, with SFML used as a window library.
SFML is not in the repo, it gets cloned as part of the CMake generation process. I know that's cursed - might replace the library with something that has a sensible build system like GLFW eventually.

One day I'd like for this to become a fully fledged level-editor, but a level-viewer is an important milestone.
Currently only handles scene files (.gsc on disc, compressed, or .nus in raw format.)
Object scene files (.hgo, object .gsc) might load after decompression but I doubt they'll display anything. Or that they won't crash.

You will need command line usage to make use of this tool. It doesn't have any command line arguments itself, but it relies on external tooling to do anything.

## Currently supported builds
- E3 Demo

Will expand once more start getting supported.

## Building the thing
1. Get CMake and install git. The GitHub desktop app isn't enough, you need to be able to open a command prompt, type in `git --version` and get a sensible output.
2. [LINUX ONLY] You need to grab the required dependencies for SFML. Check [this SFML repo](https://github.com/SFML/cmake-sfml-project), in the "How to Use" section.
3. Clone this repo recursively `git clone --recursive https://github.com/Open-Travelers/travelers-toolkit.git`. If you don't use the --recursive flag, you will not get a full repo due to submodules.
4. Configure using cmake `cmake -B build/`, this will create a build/ directory with your default generator.
5. Compile using cmake `cmake --build build/`, this should invoke whichever default generator is set on your platform.

## Using the thing
1. Extract the contents of an ISO for the version of the game you wish to poke around in.
2. Find a .gsc or .nus file you're interested in. They're all over the place in the disc's filesystem.
3. Decompress your target .gsc file using an RNC decompressor. I like using [rnc_propack](https://github.com/lab313ru/rnc_propack_source/tree/master), as it's pretty simple to install and use. Eventually I want to bundle my own implementation of an RNC decompressor but currently that is not yet implemented. **This doesn't apply to the Gamecube version, they conveniently forgot to compress anything when making that one.**
4. Open your target decompressed .gsc file using the tool.

### Camera movement
- Left/Right turns your camera left and right.
- Up/Down moves your camera forward and back.
- W/S moves your camera up and down.

A better camera system is WIP.
