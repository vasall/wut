SHELL=/bin/bash

#
# Build all submodules
#
SUBMODULES_MAKEFILES=($(find ./source -type f -name "Makefile"))
for i in "${SUBMODULES_MAKEFILES[@]}"
do
    MAKEPATH="${i%/*}"

    echo "Build ${MAKEPATH}"
    (cd "./${MAKEPATH}/" && make)
done

#
# Combine all library-files.
#
SUBMODULES_LIBS=$(find ./source -type f -name "*.a")
libtool --mode=link gcc -all-static -o lfreihand.a ${SUBMODULES_LIBS}



