CC = gcc
CPC = g++
HASH_CODE = djb2.c
HASH_OUT = djb2

MAIN_CODE = $(HASH_CODE)
MAIN_OUT = $(HASH_OUT)

all : $(MAIN_OUT)

$(MAIN_OUT) : $(MAIN_CODE)
	$(CC) $(MAIN_CODE) -o $(MAIN_OUT)

test : $(MAIN_OUT)
	./$(MAIN_OUT) 15 hot_dog salt_pork pizza

clean :
	rm -f $(HASH_OUT)
