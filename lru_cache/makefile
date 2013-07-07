CC = gcc
CPC = g++

DEBUG_HEADER = debug.h
DEBUG_CODE = debug.cpp
DEBUG_OUT = debug

HASH_HEADER = djb2.h
HASH_CODE = djb2.cpp
HASH_OUT = djb2.o

HASHMAP_HEADER = hashmap.h
HASHMAP_CODE = hashmap.cpp
HASHMAP_OUT = hashmap.o

MAIN_CODE = main.cpp
MAIN_OUT = hash

all : $(MAIN_OUT)

$(DEBUG_OUT) : $(DEBUG_HEADER) $(DEBUG_CODE)
	$(CPC) -c $(DEBUG_CODE) -o $(DEBUG_OUT)

$(HASH_OUT) : $(HASH_CODE) $(HASH_HEADER)
	$(CPC) -c $(HASH_CODE) -o $(HASH_OUT)

$(HASHMAP_OUT) : $(HASHMAP_CODE) $(HASHMAP_HEADER)
	$(CPC) -c $(HASHMAP_CODE) -o $(HASHMAP_OUT)

test-hashmap : $(HASHMAP_OUT) $(HASH_OUT)
	$(CPC) $(HASHMAP_OUT) $(HASH_OUT) -o $(MAIN_OUT)

test-hash : $(HASH_OUT)
	$(CPC) $(HASH_OUT) $(MAIN_CODE) -o $(MAIN_OUT)

$(MAIN_OUT) : $(MAIN_CODE) $(HASH_OUT) $(DEBUG_OUT) $(HASHMAP_OUT)
	$(CPC) $(MAIN_CODE) $(DEBUG_OUT) $(HASH_OUT) $(HASHMAP_OUT) -o $(MAIN_OUT)

test : $(MAIN_OUT)
	./$(MAIN_OUT) 15 hot_dog salt_pork pizza

clean :
	rm -f $(HASH_OUT) $(DEBUG_OUT) $(HASHMAP_OUT) $(MAIN_OUT)
