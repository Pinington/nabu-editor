CC=gcc
CFLAGS=-Iheaders
LDFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

SOURCES=main.c src/editor.c 
TARGET=bin/text_editor.exe

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	del $(TARGET)