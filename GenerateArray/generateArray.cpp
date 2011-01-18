#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

int main(int argc, char** argv)
{
	int * array;
    int i(0), num(0), max(0), min(0);
    ofstream output;
	// seed the random number generator
	srand((unsigned)time(0));
	
	// check and confirm args
	if (argc == 4)
	{
		cout 	<< "Number of Elements to generate: " << argv[1] << "\n"
				<< "Lower Boundary: " << argv[2] << "\n"
				<< "Upper Boundery: " << argv[3] << "\n";
	}
	else
	{
		cout 	<< "Error. incorrect arguments\n"
				<< "Useage: ./GenerateArray numElements lowerBound upperBound\n";
		return 0;
	}
	
	// parse out the args
	if((stringstream(argv[1]) >> num).fail())
	{
		cout 	<< "Number of elements not a number.\n";
		return 0;
	}
	if((stringstream(argv[2]) >> min).fail())
	{
		cout 	<< "Lower Boundary not a number.\n";
		return 0;
	}
	if((stringstream(argv[3]) >> max).fail())
	{
		cout 	<< "Upper Boundary not a number.\n";
		return 0;
	}
		
	//allocate the array
	array = new int[num];	
		
	// populate the array
	for(i=0; i<num; i++)
	{
		array[i] = rand() % (max-min) + min;
	}
	
	// sort the array
	// welp. what kinda sort should i use? nto bubble... mergesort?
	
	
	// print the array
	// for(i=0; i<num; i++)
		//cout << array[i] << "\n";
	
	// save the array
	output.open("MyArray.txt", ios::trunc);
	for(i=0; i<num; i++)
		output << array[i] << "\n";
	output.close();
	
	// free the array
	delete array;
	return 0;
}