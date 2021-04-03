echo "> Compiling source code..."
g++ Game.cpp ./sources/stb_image.cpp ./sources/Camera.cpp ./sources/ObjLoader.cpp ./sources/Texture.cpp ./sources/Vector.cpp ./sources/Material.cpp -lGL -lGLU -lglfw -o ./bin/Game

echo "> Source code compiled. Running game!"
./bin/Game