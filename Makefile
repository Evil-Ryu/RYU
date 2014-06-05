NAME=run

CC = g++
CFLAGS = -std=gnu++0x -g -c
INCLUDES = -I/usr/local/include -I/usr/include
LIBS = -L/usr/X11R6/lib -lX11 -L/usr/local/lib -lSDL2 -lGL -lGLU -lEGL -lm -lpthread

all: $(NAME)

$(NAME): src/main.cpp src/ryu_pathtracer.cpp src/ryu_algebra.cpp src/ryu_raymarcher.cpp src/ryu_render.cpp src/ryu_image.cpp src/ryu_mandelbulb.cpp \
		 src/ryu_mandelbox.cpp src/ryu_kifs.cpp src/ryu_context.cpp
	$(CC) $(CFALGS)  $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm $(NAME)
