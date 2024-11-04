# MineSweeper

Simple minesweeper game using C++ and SFML.

## Building

1. Install Git and CMake.
2. Clone this repository: `git clone https://github.com/FinleyConway/Minesweeper.git`.
3. If you use Linux, install SFML's dependencies using your system package manager. Debian-based distributions can use the following commands:
````
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
````
4. Using CMake from the command line; making sure you are in the root directory of the project, use the following commands:
````    
cmake -B build
cmake --build build
````
5. Open build/bin and run the executable.

## License