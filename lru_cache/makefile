CC = gcc
CPC = g++

DEBUG_HEADER = debug.h
DEBUG_CODE = debug.cpp
DEBUG_OUT = debug

HASH_HEADER = djb2.h
HASH_CODE = djb2.cpp
HASH_OUT = djb2.o
HASH_TEST = test_hash.cpp
HASH_TEST_EXE = test_hash

HASHMAP_HEADER = hashmap.h
HASHMAP_CODE = hashmap.cpp
HASHMAP_OUT = hashmap.o

UNIT_TESTS_CODE = unit_tests.cpp
UNIT_TESTS_OUT = unit_tests

all : $(UNIT_TESTS_OUT)

$(DEBUG_OUT) : $(DEBUG_HEADER) $(DEBUG_CODE)
	$(CPC) -c $(DEBUG_CODE) -o $(DEBUG_OUT)

$(HASH_OUT) : $(HASH_CODE) $(HASH_HEADER)
	$(CPC) -c $(HASH_CODE) -o $(HASH_OUT)

$(HASHMAP_OUT) : $(HASHMAP_CODE) $(HASHMAP_HEADER)
	$(CPC) -c $(HASHMAP_CODE) -o $(HASHMAP_OUT)

test-hashmap : $(HASHMAP_OUT) $(HASH_OUT)
	$(CPC) $(HASHMAP_OUT) $(HASH_OUT) -o

test-hash : $(HASH_OUT) $(HASH_TEST)
	$(CPC) $(HASH_OUT) $(HASH_TEST) -o $(HASH_TEST_EXE)
	./$(HASH_TEST_EXE) 15 hot_dog salt_pork bacon eggs

$(UNIT_TESTS_OUT) : $(UNIT_TESTS_CODE) $(HASH_OUT) $(DEBUG_OUT) $(HASHMAP_OUT)
	$(CPC) $(UNIT_TESTS_CODE) $(DEBUG_OUT) $(HASH_OUT) $(HASHMAP_OUT) -o $(UNIT_TESTS_OUT)

test : $(UNIT_TESTS_OUT)
	./$(UNIT_TESTS_OUT)

clean :
	rm -f $(HASH_OUT) $(DEBUG_OUT) $(HASHMAP_OUT) $(UNIT_TESTS_OUT) $(HASH_TEST_EXE) *.o
