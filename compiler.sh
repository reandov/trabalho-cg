echo "> Compiling source code"
g++ main.cpp source/ObjLoader.cpp source/Vetor.cpp headers/ObjLoader.h headers/Vetor.h -lGL -lGLU -lglfw -o bin/main

echo "> Running file"
cd bin && ./main