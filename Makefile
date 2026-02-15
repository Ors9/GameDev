# הגדרות כלליות
PG_PATH = C:/Program Files/PostgreSQL/18
# משתמשים ב-g++ כי הוא יודע לקמפל גם C וגם C++ ולבצע Link לספריות הסטנדרטיות של CPP
CXX = g++
TARGET = game.exe
SRC_DIR = src
LIB_DIR = lib

# כאן השינוי המרכזי: מחפשים גם קבצי .c וגם קבצי .cpp
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/*/*.c) $(wildcard $(SRC_DIR)/*/*.cpp) \
       $(wildcard $(SRC_DIR)/*/*/*.c) $(wildcard $(SRC_DIR)/*/*/*.cpp)

# Includes
INCLUDES = -Iinclude -Ideps/raylib/include -Ideps/libsodium-win64/include -Ideps/raygui -I"$(PG_PATH)/include"

# Libraries
LIBRARIES = -L. -L$(LIB_DIR) -L"$(PG_PATH)/lib" -Ldeps/libsodium-win64/lib -Ldeps/raylib/lib -lraylib -lpq -lsodium -lws2_32 -lsecur32 -lshlwapi -lwldap32 -lopengl32 -lgdi32 -lwinmm

# דגלים ל-C++ (אופציונלי, עוזר למצוא טעויות)
CXXFLAGS = -std=c++17 -Wno-write-strings

all:
	$(CXX) $(SRCS) -o $(TARGET) $(INCLUDES) $(LIBRARIES) $(CXXFLAGS)

clean:
	rm -f $(TARGET) *.o src/*.o src/*/*.o src/*/*/*.o