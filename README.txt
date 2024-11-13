# How to install

First you need to install the necessary libraries:
- git
- gcc
- make
- build-essential
- libsdl2-dev
- libsdl2-image-dev
- freeglut3-dev

To install all libraries at once use the following command:

        apt install git gcc make build-essential libsdl2-dev libsdl2-image-dev freeglut3-dev

Now you can clone the  repository

        git clone https://github.com/vasall/wut.git

Finally to build the library we just have to change into the cloned repository and call the build script

        cd wut
        bash build.sh

If everything went successfully you should now have the static library file **lwut.a**.


# Text Rendering

The underlying technique is explained in this paper: 
https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf

To convert a TTF-file to the SDF-format you can use this project:
https://github.com/ShoYamanishi/SDFont

To build this project, the following libraries are required:

- cmake
- libpng-dev
- libfreetype6-dev 
- libglew2.2
- libglfw3-dev
- libglm-dev

Simply clone the repository and then run the following commands to build it:

        cd SDFont
        mkdir build
        cd build
        cmake -DCMAKE_BUILD_TYPE=Release ..
        VERBOSE=1 make

After having successfully built the library, we can now convert a TTF-file to
SDF-format using the following command:

        ./sdfont_commandline -font_path <path_to_ttf> <output_name_without_ext>

If everything worked as intended you should now have a PNG- and TXT-file. These
can be passed to the WUT-framework.


