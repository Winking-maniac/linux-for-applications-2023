TARGET = Show
.PHONY: all clean show_yourself

all: show_yourself

show_yourself: $(TARGET) 
	./$(TARGET) Show.c

$(TARGET): $(TARGET).o
	gcc $(CFLAGS) $^ -lncurses -o $@
    
%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET)

