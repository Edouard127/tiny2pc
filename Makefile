SOURCES = parser.c
OBJECTS = $(SOURCES:.c=.o)
TARGET  = parser
CFLAGS 	= -Wall -Werror -g \
	  -I/usr/local/include
LDFLAGS = -L/usr/local/lib \
	  -pthread
LDLIBS	=

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)


%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
