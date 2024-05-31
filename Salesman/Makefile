# Variables
CC = cl
CFLAGS = /W4 /D_CRT_SECURE_NO_WARNINGS /EHsc
LDFLAGS = /link
TARGET = Salesman.exe
OBJ = algorithms.obj dynarray.obj graph.obj Salesman.obj

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) /OUT:$(TARGET)

# Object files
algorithms.obj: algorithms.c algorithms.h
	$(CC) $(CFLAGS) /c algorithms.c

dynarray.obj: dynarray.c dynarray.h
	$(CC) $(CFLAGS) /c dynarray.c

graph.obj: graph.c graph.h
	$(CC) $(CFLAGS) /c graph.c

Salesman.obj: Salesman.c
	$(CC) $(CFLAGS) /c Salesman.c

# Clean up build artifacts
clean:
	del $(OBJ) $(TARGET)