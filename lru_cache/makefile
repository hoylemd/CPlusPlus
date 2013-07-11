CC = gcc
CPC = g++ -g

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

CACHE_HEADER = cache.h
CACHE_CODE = cache.cpp
CACHE_OUT = cache.o
CACHE_DATA = food_data.txt

UNIT_TESTS_CODE = unit_tests.cpp
UNIT_TESTS_OUT = unit_tests
UNIT_TEST_REQS = $(HASH_OUT) $(DEBUG_OUT) $(HASHMAP_OUT) $(CACHE_OUT)

all : $(UNIT_TESTS_OUT)

$(DEBUG_OUT) : $(DEBUG_HEADER) $(DEBUG_CODE)
	$(CPC) -c $(DEBUG_CODE) -o $(DEBUG_OUT)

$(HASH_OUT) : $(HASH_CODE) $(HASH_HEADER)
	$(CPC) -c $(HASH_CODE) -o $(HASH_OUT)

$(HASHMAP_OUT) : $(HASHMAP_CODE) $(HASHMAP_HEADER)
	$(CPC) -c $(HASHMAP_CODE) -o $(HASHMAP_OUT)

$(CACHE_OUT) : $(CACHE_CODE) $(CACHE_HEADER)
	$(CPC) -c $(CACHE_CODE) -o $(CACHE_OUT)

test-hashmap : $(HASHMAP_OUT) $(HASH_OUT)
	$(CPC) $(HASHMAP_OUT) $(HASH_OUT) -o

test-hash : $(HASH_OUT) $(HASH_TEST)
	$(CPC) $(HASH_OUT) $(HASH_TEST) -o $(HASH_TEST_EXE)
	./$(HASH_TEST_EXE) 15 hot_dog salt_pork bacon eggs

$(UNIT_TESTS_OUT) : $(UNIT_TESTS_CODE) $(UNIT_TEST_REQS)
	$(CPC) $(UNIT_TESTS_CODE)  $(UNIT_TEST_REQS) -o $(UNIT_TESTS_OUT)

test : $(UNIT_TESTS_OUT)
	./$(UNIT_TESTS_OUT)

memcheck : $(UNIT_TESTS_OUT)
	valgrind ./$(UNIT_TESTS_OUT)

clean :
	rm -f $(HASH_OUT) $(DEBUG_OUT) $(HASHMAP_OUT) $(UNIT_TESTS_OUT) $(HASH_TEST_EXE) *.o
