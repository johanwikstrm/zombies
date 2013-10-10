#! /bin/sh

echo " "
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%"
echo "%%% Check memory leaks %%%"
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%"
echo " "

for unitTest in unitTest_*.cpp
do
    name="${unitTest%.cpp}"
    echo $name " "
    valgrind --leak-check=full --log-file=log_valgrind ./"$name"
    grep "no leaks are possible" log_valgrind > log_valgrind_out
    if test -s "log_valgrind_out"
    then
        echo "OK"
    else 
        echo "Memory leak"
        rm -f log_valgrind
        rm -f log_valgrind_out
        exit 1
    fi
done

echo " "
echo "       No memory leak"
echo " "

rm -f log_valgrind
rm -f log_valgrind_out
