/* Michael D. Hoyle
0553453
rbs.c
Source code for the red Blues problem. does everything.
*/

#include <stdio.h>
#include <stdlib.h>
#include <specstrings.h> //huh? VS turnes strings.h into this...
#include <string.h>
#include <math.h>
#include <time.h>
#include "wallclock.h"

#define DEBUGMODE 0 //0 is none, 1 is all, others...

/*TODO:
clean up code
implement openmp
eliminate false sharing

*/

/* stuff to do to improve this:
	more efficiently encode the cells
	parallelize the cell shifting (?)
	generate a "blue location" list to speed up the blue shifting
	pad the parallelized data
	board structure
*/

// structure tohold the data for a thread to work on
typedef struct{
	int width;
	int rows;
	char ** data;
	char symbol;
} job;

/* function to generate a blank board */
// remember to call freeBoard()
char ** blankBoard(int width)
{
	int i = 0, j = 0;
	char ** board = (char**)malloc(sizeof(char*) * width);

	// debug level 1 message
	if (DEBUGMODE == 1) printf("blankBoard(width = %d)\n", width);

	// instantiate the board
	for(i=0;i<width;i++) board[i] = (char*)malloc(sizeof(char) * width);

	// ensure it's blank
	for(i=0;i<width;i++)
	{
		for(j=0;j<width;j++)
			board[i][j] = 0;
	}

	return board;
}

/* function to generate a random board */
// this might be able to be parallelized
// remember to call freeBoard()
char ** newBoard(int width)
{
	int i = 0, j = 0, value = 0;
	char ** board = blankBoard(width);

	// debug level 1 message
	if (DEBUGMODE == 1) printf("newBoard(, width = %d)\n", width);

	// populate the board randomly
	for(i=0; i<width; i++)
	{
		for(j=0; j<width; j++)
		{
			value = rand()%3;		// generate a random value

			if(value == 1)			// on 1 insert red
				board[i][j] = 'R';
			else if(value == 2)		// on 2 insert blue
				board[i][j] = 'B';
		}
	}

	// kick the new board back
	return board;
}

/* function to free a board*/
/* intended to be called as board = free(board, width);*/
char ** freeBoard(char** board, int width) //freebird?
{
	int i = 0;

	// debug level 1 message
	if (DEBUGMODE == 1) printf("freeBoard(board = %d, width = %d)\n", board, width);

	if (board != NULL)								// if the board ain't null
	{
		for(i=0; i<width; i++)						// iterate through the rows
		{
			if (board[i] != NULL) free(board[i]);	// free em if they're not null
		}
		free(board);								// free the whole board!
		return NULL;									// return NULL so there's no double freeing monkeybusiness
	}
	else
		return board;
}

/* function to invert the board so blues can be computed the same way as the reds */
char ** rotateBoard(char ** board, int width)
{
	int i = 0, j = 0, value = 0;
	char ** rotation = (char**)malloc(sizeof(char*) * width);

	// debug level 1 message
	if (DEBUGMODE == 1) printf("rotateBoard(board = %d, width = %d)\n", board, width);

	// instantiate the board
	for(i=0;i<width;i++) rotation[i] = (char*)malloc(sizeof(char) * width);

	for(i = 0; i < width; i++)
	{
		for(j = 0; j < width; j++)
			rotation[i][j] = board[j][i];
	}

	return rotation;
}

/* function to print out a board */
void printBoard(char ** board, int width)
{
	int i = 0, j = 0;

	// debug level 1 message
	if (DEBUGMODE == 1) printf("printBoard(board = %d, width = %d)\n", board, width);

	for(i=0;i<width;i++)printf("%d",i);
	printf("\n");

	if (board != NULL)											// if the board exists
	{
		for(i = 0; i<width; i++)								// iterate through the rows
		{
			if (board[i] != NULL)								// if the row exists								
			{
				printf("%2d:",i);
				for (j=0; j<width; j++)							// print it out
				{
					if( board[i][j] == 0 ) printf(" ");
					else if (board[i][j] == 'R') printf(">");
					else if (board[i][j] == 'B') printf("v");
					else printf("?");

				}
				printf("%2d\n",i);									// next line
			}
		}
	}

	for(i=0;i<width;i++)printf("%d",i);
	printf("\n");
}

/* function to write out a board to disk*/
// maybe i should have the file opening in here
// ooh i bet this can also be parallelized
void saveBoard(char ** board, int width, FILE * fp)
{
	int i = 0, j = 0;
	int len = width + 1;
	char * string = (char*)malloc(sizeof(char) * len);
	
	// debug level 1 message
	if (DEBUGMODE == 1) printf("saveBoard(board = %d, width = %d, file)\n", board, width);

	for(i=0;i<len;i++)string[i]=0;
	if (board != NULL)											// if the board exists
	{
		for(i = 0; i<width; i++)								// iterate through the rows
		{
			if (board[i] != NULL)								// if the row exists								
			{
				for (j=0; j<width; j++)							// print it out
				{
					if( board[i][j] == 0 ) string[j] = ' ';
					else if (board[i][j] == 'R') string[j] = '>';
					else if (board[i][j] == 'B') string[j] = 'v';
					else string[j] = '?';

				}
				string[width] = '\n';								// next line
				fwrite(string, sizeof(char),len,fp);

			}
		}
	}
	

}

/* fucntion to create a job structure to hold data for a thread to compute */
job * createJob(char** board, int width, int rows, int index, char symbol)
{
	int myRows = rows;
	int i = 0, j = 0;
	job * newJob = (job*)malloc(sizeof(job));

	// debug level 1 message
	if (DEBUGMODE == 1) printf("createJob(board = %d, width = %d, rows = %d, index = %d, symbol = %d)\n", board, width, rows, index, symbol);
	
	// record the job's information in the struct
	newJob ->width = width;
	newJob ->rows = myRows;
	newJob ->symbol = symbol;

	// allocate the data rows
	newJob ->data = (char**)malloc(sizeof(char*) * myRows);
	for (i=0; i< myRows; i++) newJob->data[i] = (char*)malloc(sizeof(char)*width);

	//populate the rows
	for(i=0; i< myRows; i++)
	{
		for(j=0;j<width;j++) newJob->data[i][j] = board[index+ i][j];
	}
	
	return newJob;
}

/* function to free a job struct */
// intended to be used like jobName = freeJob(jobName);
job * freeJob(job * jobby)
{
	int i = 0;

	// debug level 1 message
	if (DEBUGMODE == 1) printf("freeJob(jobby = %d)\n", jobby);

	if (jobby != NULL)				// if the job exists
	{
		if (jobby->data != NULL)		// if it's data exists
		{	//free the data rows;
			for(i=0; i< jobby->rows; i++)
			{	
				if (jobby->data[i] != NULL) 
				{
					free(jobby->data[i]);
					jobby->data[i] = NULL;
				}
			}
			//free the data pointer
			free(jobby->data);
			jobby->data = NULL;
		}
		free(jobby);
		return NULL;
	} else
		return jobby;
}

/* function to print out a job */
void printJob(job * jobby)
{
	int i = 0, j = 0;

	// debug level 1 message
	if (DEBUGMODE == 1) printf("printJob(jobby = %d)\n", jobby);
	
	if (jobby->data != NULL)
	{
		for(i=0; i<jobby->rows; i++)
		{
			if (jobby->data[i] != NULL)								// if the row exists								
			{
				for (j=0; j<jobby->width; j++)							// print it out
				{
					if( jobby->data[i][j] == 0 ) printf("[ ]");
					else if (jobby->data[i][j] == 'R') printf("[>]");
					else if (jobby->data[i][j] == 'B') printf("[v]");
					else printf("[%c]", jobby->data[i][j]);

				}
				printf("\n");									// next line
			}
		}
	}
}


/* function to give to a thread.
	computes one half-step of the provided chunk */
void* thread(void * arg)
{
	int row = 0, i = 0;
	char * targetRow = NULL;
	char * currRow = NULL;
	char ** data = NULL;
	job * myJob = (job *) arg;
	int rows = 0, width = 0;
	char symbol = 0;
	
	// debug level 1 message
	if (DEBUGMODE == 1) printf("thread(arg)\n");

	// localize the variables
	rows = myJob->rows;
	width = myJob->width;
	symbol = myJob->symbol;
	data = myJob->data;

	for(row = 0; row < rows; row++)				// look at each row 
	{
		//reinstantiate row buffer
		currRow = (char*)malloc(sizeof(char)*width);
		for (i=0; i<width; i++)
			currRow[i] = 0;

		targetRow = data[row];
		for(i = 0; i< width; i++)					// look at each cell sequentially
		{
			if(targetRow[i] == symbol)				// if it's the right symbol...
			{
				if (i < width-1)						// if this is before the last one
				{
					if(targetRow[i+1] == 0)				// and the next one's blank
					{
						currRow[i+1] = symbol;			// move it ahead
						i++;							// skip the next one (just filled in)
					}
					else								// if it's not blank
						currRow[i] = symbol;			// just copy the symbol'd cell. it aint movin'!
				} else									// if it is the last one
					if(targetRow[0] == 0)				// and the first one's blank
					{
						currRow[0] = symbol;			// wrap it around
					}
					else								// if it's not blank
						currRow[i] = symbol;			// just copy the symbol'd cell. it aint movin'!
			} else 									// if the cell is non-symbol
				currRow[i] = targetRow[i];					
		}

		//put the new row back into the job structure
		if (data[row]) free(data[row]);
		data[row] = currRow;
		currRow = NULL;
	}
	return NULL;
}

/* top level function to handle thread spawning and generally just do one step */
char ** makeMoves(char ** board, int width, char symbol, int threads)
{
	int row = 0, i = 0, j=0;
	job ** jobs;
	int workers = threads;
	int totalThreads = 0;
	int parallel = 1;
	int nextRow  =0;
	int * jobSize = NULL;
	char ** shifted = blankBoard(width);
	char * currRow = NULL, *targetRow = NULL;

	// debug level 1 message
	if (DEBUGMODE == 1) printf("makeMoves(baord = %d, width = %d, symbol = %c, threads = %d)\n", board, width, symbol, threads);

	// ensure workers is not greater than rows
	if (workers > width) workers = width;

	// account for main thread becoming a worker
	totalThreads = workers;
	workers--;

	// turn off parallel if only 1 thread is available
	if(totalThreads == 1) parallel = 0;

	// calculate jobsizes
	jobSize = (int*)malloc(sizeof(int)*totalThreads);
	for(i=0; i<totalThreads; i++) jobSize[i] = 0;
	for(i=0; i<width;i++)					// Iterate through the rows and increment each worker x's
	{										// jobsize by 1 for every row it will have to deal with
		jobSize[j] = jobSize[j] + 1;
		j++;
		if(j==totalThreads) j=0;
	}

	// allocate the jobs
	jobs = (job**)malloc(sizeof(job*)*totalThreads);
	
	// generate the job objects
	nextRow = 0;
	for (i=0; i<totalThreads; i++)
	{
		jobs[i] = createJob(board, width, jobSize[i], nextRow, symbol);
		nextRow += jobSize[i];
	}

	// spawn workers
	// currently just iterate through the jobs and do them
	if (parallel)
	{
		for(i=0; i<workers; i++)
			thread((void*)jobs[i]);
	}
	// do the last job
	thread((void*)jobs[workers]);

	// wait untill everyone's done
	if (parallel)
	{
		for(i=0; i<workers; i++);
	}

	// collect the jobs
	for(i=0; i<totalThreads; i++)
	{
		for(j=0;j<jobs[i]->rows;j++)
		{
			if(shifted[row]) free(shifted[row]);
			shifted[row] = jobs[i]->data[j];
			row++;
		}
	}

	// clean up
	if(jobs != NULL)
	{
		for(i=0; i<totalThreads; i++)
		{	
			if(jobs[i] != NULL) free(jobs[i]);
		}
		free(jobs);
	}

	if(jobSize) free(jobSize);

	return shifted;
}

/*function to check all the tiles for being above the threshold*/
int checkTiles(char ** board, int width, int tile, int density)
{
	int i = 0, j = 0;
	int x = 0, y = 0;
	int r = 0, b = 0;
	int cellsInTile = tile * tile;
	char current;
	int numTiles = width / tile;
	int threshold = (cellsInTile*density)/100;

	// debug level 1 message
	if (DEBUGMODE == 1) printf("CheckTiles(board = %d, width = %d, tile = %d, density = %d)\n", board, width, tile, density);

	for(i=0; i<numTiles; i++)
	{
		for(j=0; j<numTiles; j++)
		{
			// calculate this tile's density
			for(x = 0; x<tile; x++)
			{
				for(y=0; y< tile; y++)
				{
					current = board[i*tile + x][j*tile + y];
					if(current == 'R') 
					{
						r++;
					}
					else if(current == 'B')
					{
						b++;
					}
				}
			}
			if (r > threshold) return (r*100) / (cellsInTile);
			if (b > threshold) return (b*100) / (cellsInTile);
			r = 0;
			b = 0;
		}
	}

	return 0;
}

int main(int argc, char ** argv)
{
	int width = 0, turn = 0, numSteps =0, i = 0, j=0;
	int threads = 0, tile = 0, density;
	int seed = 0, interactive = 0;
	int half = 0, skewed = 0;
	char **board, **rotation, **shifted, input[16];
	char output[128], seedMess[16], intMess[16];
	int stopDensity = 0, steps=0;
	FILE * fp;
	double wallTime;

	// debug level 1 message
	if (DEBUGMODE == 1) 
	{
		printf("main(");
		for(i=1; i< argc; i++) printf("%s, ", argv[i]);
		printf(")\n");
	}

	//setup
	StartTime();

	for(i=0; i<128;i++)
	{
		output[i] = 0;
		if(i<16)
		{
			seedMess[i] = 0;
			intMess[i] = 0;
			input[i] = 0;
		}
	}
	
	sprintf(intMess, "No");

	// check the number of arguments
	if ((argc < 5) || (argc > 8))
	{
		printf("Incorrect arguments\n");
		exit(0);
	}
	
	// parse the arguments
	for (i=1; i<argc; i++)
	{
		if (argv[i][0] == 'p')
			threads = atoi(&argv[i][1]);
		else if (argv[i][0] == 'b')
			width = atoi(&argv[i][1]);
		else if (argv[i][0] == 't')
			tile = atoi(&argv[i][1]);
		else if (argv[i][0] == 'c')
			density = atoi(&argv[i][1]);
		else if (argv[i][0] == 'm')
			numSteps = atoi(&argv[i][1]);
		else if (argv[i][0] == 's')
			seed = atoi(&argv[i][1]);
		else if (argv[i][0] == 'i')
		{
			interactive = 1;
			sprintf(intMess,"Yes");
		}
		else							// on parse failure, politely crash
		{
			printf("Incorrect argument \"%s\"\n", argv[i]);
			exit(0);
		}
		
		// seed the random number generator
		if (seed == 0)
		{
			seed = (int)time(NULL);
			sprintf(seedMess, "(Random)");
		}
		srand(seed);
	}

	// validate arguments 
	if (width < 2)
	{
		printf("invalid board size\n");
		exit(0);
	}
	if ((width % tile) != 0)
	{
		printf("invalid tile size\n");
		exit(0);
	}
	if (density < 1 || density > 100)
	{
		printf("denisty out of range\n");
		exit(0);
	}
	if (numSteps < 1)
	{
		printf("max steps too few\n");
		exit(0);
	}

	// Generate a random board
	board = newBoard(width);

	// Main loop
	for(i=0; i<numSteps;i++)
	{
		// clear the input buffer		
		for(j=0; j<16;j++) input[j] = 0;

		// reset the half-step flag
		half = 0;

		// parse the input, if we're in interacive mode and don't have a number of steps to do
		if (steps <1 && interactive)
		{
			// read the input
			if(interactive)
				fgets(input,16,stdin);

			// parse it and set flags as needed
			if(input[0] == 'c') interactive = 0;
			else if(input[0] == 'x') break;
			else if(input[0] == 'h') half = 1;
			else if(input[0] == '\n');
			{
				steps = atoi(input) - 1;
			}
		} else 
			steps--;	// decrement the steps counteri);

		// run the shifts
		if( skewed == 0) // if it's unskewed, do a red shift
		{
			// make the shifts
			shifted = makeMoves(board, width, 'R', threads);

			// update board
			board = freeBoard(board,width);
			board = shifted;
			shifted = NULL;

			// print it
			if (interactive == 1)
			{
				printf("\nafter half-step %d: h = %d s = %d steps = %d\n",i, half, skewed, steps);
				printBoard (board, width);
			}
		}

		// if we're doing a full step, or a half step after a red half step, do a blue shift
		if((half == 1 && skewed ==1) || half == 0)
		{

			// generate a rotated board
			board = rotateBoard(board, width);

			// do the next half-step
			shifted = makeMoves(board, width, 'B', threads);
			board = freeBoard(board, width);
			board = rotateBoard(shifted, width);
			shifted = freeBoard(shifted, width);
			shifted = NULL;

			// print it
			if (interactive == 1)
			{
				printf("\nafter step %d: h = %d s = %d steps = %d\n", i, half, skewed, steps);
				printBoard (board, width);
			}
		}

		// if we just did a half step
		if(half == 1)
		{
			// toggle the skewed flag, and decrement the iterator if neccesary
			if (skewed == 1) skewed = 0;
			else 
			{
				skewed = 1;
				i--;
			}
		}

		// check if any tiles have the correct density
		stopDensity = checkTiles(board, width, tile, density);

		// exit if one if a tile of sufficient density has been found
		if(stopDensity != 0)
			break;
	}

	steps = i;
	wallTime = EndTime();

	// generate the output string
	sprintf(output,"Arguments: ");
	for(i=1;i<argc;i++)sprintf(output,"%s%s ",output, argv[i]);
	sprintf(output, "%siterations: %d, density: %d, time: %.3fs\n", output, steps, stopDensity, wallTime);

	// print it
	printf(output);

	// save the board to a file
	fp = fopen("redblue.txt","w");
	saveBoard(board,width,fp);

	// append the output string to the file
	fwrite(output,strlen(output), 1, fp);

	// close the file
	fclose(fp);

	// free the boards
	board = freeBoard(board, width);
	
	// exit
	return 0;
}		