echo "> Compiling source code"
g++ main.cpp ObjLoader.cpp ObjLoader.h Vetor.cpp Vetor.h Material.cpp Material.h -lGL -lGLU -lglfw -o main

echo "> Running file"
./main