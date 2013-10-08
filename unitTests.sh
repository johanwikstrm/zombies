#! /bin/sh

echo " "
echo "%%%%%%%%%%%%%%%%%%"
echo "%%% Unit tests %%%"
echo "%%%%%%%%%%%%%%%%%%"
echo " "

for unitTest in unitTest_*.cpp
do
    name="${unitTest%.cpp}"
    echo $name " "
    if ! mpirun -mca btl ^openib -np 4 --mca orte_base_help_aggregate 0 ./$name
    then
        echo "Error in the unit test"
        exit 1
    fi
done

echo " "
echo "      No error detected"
echo " "
