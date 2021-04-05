echo "



    +===========================+
    |                           |
    |      PROTO-MINECRAFT      |
    |                           |
    +===========================+

    ~> Evandro Rodrigues
    ~> Leonardo David
    ~> Jordão Rodrigues


"

echo "  > Compiling source code..."
g++ Game.cpp ./sources/stb_image.cpp ./sources/Camera.cpp ./sources/ObjLoader.cpp ./sources/Texture.cpp ./sources/Vector.cpp ./sources/Material.cpp -lGL -lGLU -lglfw -o Game

echo "  > Source code compiled. Running game!\n"
./Game