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
mkdir objdir
for i in ${SUBMODULES_LIBS};
do
	$(ar --output objdir x $i);
done
$(ar r lwut.a objdir/*.o)
$(ranlib lwut.a)
rm -rf objdir
