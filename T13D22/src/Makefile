# Имя и путь
NAME = cipher
SRC = cipher.c
TARGET_DIR = ../build
TARGET = $(TARGET_DIR)/$(NAME)

# Правило по умолчанию
all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -Wall -Wextra -Werror -o $@ $<

clean:
	rm -rf $(TARGET_DIR)

re: clean all

.PHONY: all clean re
