# הגדרות כלליות
PG_PATH = "C:/Program Files/PostgreSQL/18"

CC = gcc
TARGET = game.exe

# נתיבים
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib

# קבצי מקור (מוצא את כל קבצי ה-c בתיקיית src)
SRCS = $(wildcard $(SRC_DIR)/*.c)

INCLUDES = -I$(INC_DIR) -I$(PG_PATH)/include

LIBRARIES = -L$(LIB_DIR) -L$(PG_PATH)/lib -lraylib -lpq -lopengl32 -lgdi32 -lwinmm

# חוק ברירת המחדל - קימפול התוכנה
all:
	$(CC) $(SRCS) -o $(TARGET) $(INCLUDES) $(LIBRARIES)

# חוק לניקוי קבצים מיותרים
clean:
	del $(TARGET)