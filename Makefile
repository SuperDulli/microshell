NAME = microshell

CFLAGS = -Wall -Werror -Wextra -g

SRC = microshell.c

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(NAME).dSYM

fclean: clean
	rm -f $(NAME)

debug: $(NAME)

.PHONY: clean fclean debug
