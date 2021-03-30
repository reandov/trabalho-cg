echo "> Compiling source code"
g++ main_alt.cpp functions.h functions.c -lGL -lglut -o bin/main

echo "> Running file"
cd bin && ./main