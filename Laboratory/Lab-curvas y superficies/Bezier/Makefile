TARGET=superficie
LIBS=-L"..\Graficos\freeglut\lib" -lfreeglut  -L"..\Graficos\glew-1.13.0\src" -lGLEW -lglu32 -lopengl32
CFLAGS=-I"..\Graficos\freeglut\include" -I"..\Graficos\glew-1.13.0\include" -I"..\Graficos\glm-0.9.7.3\glm"

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = superficie.o shader_utils.o
HEADERS =

%.o:	%.cpp $(HEADERS)
	g++ $(CFLAGS) -c $< -o $@

$(TARGET):	$(OBJECTS)
	g++ $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
