EXEC_FILE = custom_find
OBJECTS = custom_find.o

CFLAGS = -c -g -Wall -Wextra -Werror# obligatoires
MEM_DEBUG = -fsanitize=address

.PHONY: all clean


all: $(EXEC_FILE)





$(OBJECTS): %.o: %.c
	$(CC) $< $(CFLAGS) $(MEM_DEBUG)

$(EXEC_FILE): $(OBJECTS)
	$(CC) $^ -o $@ $(MEM_DEBUG)

test: $(EXEC_FILE)
	./test.sh

clean:
	rm -f $(EXEC_FILE) *.o
	rm -f *.aux *.log *.out *.pdf
	rm -f moodle.tgz

