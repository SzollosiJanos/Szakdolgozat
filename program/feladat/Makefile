all:
	gcc -Iinclude/ src/app.c src/main.c src/scene.c src/load.c src/model.c src/collision_triangle.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm -o collision_detection.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/app.c src/main.c src/scene.c src/load.c src/model.c src/collision_triangle.c -lSDL2 -lSDL2_image -lGL -lm -o collision_detection -Wall -Wextra -Wpedantic
