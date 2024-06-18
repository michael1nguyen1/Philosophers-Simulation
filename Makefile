NAME = philo
HEADERS	= -I ./include

CC = cc
CFLAGS = -Wall -Wextra -Werror
DEBUG_CFLAGS = -fsanitize=address -g3
SRCS = 
OBJS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Program Made"

debug: re
	$(CC) $(OBJS) $(HEADERS) $(DEBUG_CFLAGS) $(CFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

clean:
	$(RM) $(OBJS)
	@echo "Cleaned object files"

fclean: clean
	$(RM) $(NAME)
	@echo "Fully Cleaned"

re: fclean all

.PHONY: all clean fclean re debug