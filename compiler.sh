echo "> Compiling source code..."
g++ main.cpp stb_image.cpp Camera.cpp ObjLoader.cpp Texture.cpp Vector.cpp Material.cpp -lGL -lGLU -lglfw -o main
echo "> Source code compiled. Running game!"
./main