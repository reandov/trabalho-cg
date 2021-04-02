echo "> Compiling source code"
g++ main.cpp stb_image.cpp stb_image.h Camera.cpp Camera.h ObjLoader.cpp ObjLoader.h Textura.cpp Textura.h Vetor.cpp Vetor.h Material.cpp Material.h MaterialSamples.h -lGL -lGLU -lglfw -o main
./main