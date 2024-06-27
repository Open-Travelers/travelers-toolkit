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
1. [LINUX ONLY] You need to grab the required dependencies for SFML. Check [this SFML repo](https://github.com/SFML/cmake-sfml-project), in the "How to Use" section.
2. Clone this repo recursively `git clone --recursive https://github.com/Open-Travelers/travelers-toolkit.git`
3. Configure using cmake `cmake -B build/`, this will create a build/ directory with a default generator. (I like using Ninja on Linux)
4. Compile using cmake `cmake --build build/`, this should invoke whichever default generator is set on your platform.

## Using the thing
- You must first have an extracted ISO of the game you wish to poke around in.
- Find a .gsc or .nus file you're interested in. They're all over the place in the disc's filesystem.
- Then you must decompress your target .gsc file using an RNC decompressor. I like using [rnc_propack](https://github.com/lab313ru/rnc_propack_source/tree/master), as it's pretty simple to install and use. Eventually I want to bundle my own implementation of an RNC decompressor but currently that is not yet implemented.
