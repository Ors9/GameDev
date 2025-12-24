# הגדרות כלליות
CC = gcc
TARGET = game.exe

# נתיבים
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib

# קבצי מקור (מוצא את כל קבצי ה-c בתיקיית src)
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/player.c

# דגלים לקומפיילר
# -I אומר איפה לחפש קבצי .h
# -L אומר איפה לחפש ספריות .lib / .a
CFLAGS = -I$(INC_DIR) -L$(LIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm

# חוק ברירת המחדל - קימפול התוכנה
all:
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS)

# חוק לניקוי קבצים מיותרים
clean:
	del $(TARGET)