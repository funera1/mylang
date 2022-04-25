echo -e "[\e[34mx\e[0m] g++ -std=gnu++17 -Wall -Wextra -O2 main.cpp -o a.out"
g++ -std=gnu++17 -Wall -Wextra -O2 main.cpp -o a.out

result=$? 
if [ $result -ne 0 ]
then
    echo -e "[\e[31m-\e[0m] compile failed"
else
    echo -e "[\e[32m+\e[0m] successful complie"
    echo -e "[\e[34mx\e[0m] run ./a.out"
    ./a.out <input.txt> output.txt
fi

