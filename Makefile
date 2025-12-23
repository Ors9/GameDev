game: main.c 
	gcc -o game.exe main.c player.c -I./include -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm