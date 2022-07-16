echo -e ''
g++ -o main main.cpp glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp
echo -e ''
./main
echo -e ''