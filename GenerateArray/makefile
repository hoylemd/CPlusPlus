all : GenerateArray

GenerateArray : generateArray.cpp
	g++ generateArray.cpp -o GenerateArray
	
run : GenerateArray
	./GenerateArray 64 0 100

clean : 
	remove -f -r GenerateArray