# הגדרות כלליות
PG_PATH = C:/Program Files/PostgreSQL/18
CC = gcc
TARGET = game.exe
SRC_DIR = src
LIB_DIR = lib
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Includes - הוספתי גרשיים מסביב לכל נתיב שיש בו פוטנציאל לרווחים
INCLUDES = -Iinclude -Ideps/raylib/include -Ideps/libsodium-win64/include -Ideps/raygui -I"$(PG_PATH)/include"

# Libraries - הכל בשורה אחת כדי למנוע טעויות לוכסן
LIBRARIES = -L. -L$(LIB_DIR) -L"$(PG_PATH)/lib" -Ldeps/libsodium-win64/lib -Ldeps/raylib/lib -lraylib -lpq -lsodium -lws2_32 -lsecur32 -lshlwapi -lwldap32 -lopengl32 -lgdi32 -lwinmm

all:
	$(CC) $(SRCS) -o $(TARGET) $(INCLUDES) $(LIBRARIES)

clean:
	rm -f $(TARGET)