## How to install

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

