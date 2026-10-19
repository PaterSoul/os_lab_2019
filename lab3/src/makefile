# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -g

# Исходные файлы
SEQUENTIAL_SRC = sequential_min_max.c find_min_max.c utils.c
PARALLEL_SRC = parallel_min_max.c find_min_max.c utils.c

# Целевые файлы
SEQUENTIAL_BIN = sequential_min_max
PARALLEL_BIN = parallel_min_max

# Target по умолчанию - собирает все программы
all: $(SEQUENTIAL_BIN) $(PARALLEL_BIN)

# Сборка последовательной версии
$(SEQUENTIAL_BIN): $(SEQUENTIAL_SRC)
	$(CC) $(CFLAGS) $(SEQUENTIAL_SRC) -o $(SEQUENTIAL_BIN)

# Сборка параллельной версии
$(PARALLEL_BIN): $(PARALLEL_SRC)
	$(CC) $(CFLAGS) $(PARALLEL_SRC) -o $(PARALLEL_BIN)

# Очистка
clean:
	rm -f $(SEQUENTIAL_BIN) $(PARALLEL_BIN)
	rm -f *.o
	rm -f process1_result.txt process2_result.txt

# Пересборка
rebuild: clean all

# Запуск тестов
test: $(SEQUENTIAL_BIN) $(PARALLEL_BIN)
	./$(SEQUENTIAL_BIN) 42 10
	./$(PARALLEL_BIN) 42 10 by_files
	./$(PARALLEL_BIN) 42 10

.PHONY: all clean rebuild test

