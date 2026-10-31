]CC = gcc
CFLAGS = -Wall -Wextra -g

all: parallel_min_max zombie_demo process_memory parallel_sum

parallel_min_max: parallel_min_max.c find_min_max.c utils.c
	$(CC) $(CFLAGS) $^ -o $@

zombie_demo: zombie_demo.c
	$(CC) $(CFLAGS) $^ -o $@

process_memory: process_memory.c
	$(CC) $(CFLAGS) $^ -o $@

parallel_sum: parallel_sum.c libsum.c utils.c
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

clean:
	rm -f parallel_min_max zombie_demo process_memory parallel_sum
	rm -f *.o
	rm -f process1_result.txt process2_result.txt

.PHONY: all clean