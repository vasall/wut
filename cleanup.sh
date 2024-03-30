
#
# Find all object files and delete them.
#
OBJ_FILES=($(find ./ -type f -name "*.o"))

for i in "${OBJ_FILES[@]}"
do
    echo "Delete $i"
    rm $i
done


#
# Find all library files and delete them.
#
LIB_FILES=($(find ./ -type f -name "*.a"))

for i in "${LIB_FILES[@]}"
do
    echo "Delete $i"
    rm $i
done
