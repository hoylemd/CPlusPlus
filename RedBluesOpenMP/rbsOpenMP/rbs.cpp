/* Michael D. Hoyle
0553453
Assignment 3
*/

/*Todo:
	clean up
	optimize
		false sharing
		data races
		better tile check algo
		*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include "wallclock.h"

#define LOGSTEPS 0
#define DEBUGMODE 0// 0 = NONE 1 = FUNCTION CALLS 2 = THREADING 3=BOARDS 4 = DROPPING 5 = GENERAL 6 = CHECKTILES 7 = FILE WRITING
// 8 = RED/BLUE NUMBERS 9 = SHIFTS
typedef struct{
	char ** cell;
	int width;
} Board;

// instantiates a board
Board * newBoard(int width)
{
	int i = 0;

	// function call message
	if (DEBUGMODE == 1)
	{
		printf("newBoard(width = %d)\n", width);
	}

	// instantiate the board
	Board * board = (Board*)malloc(sizeof(Board));
	board -> cell = (char**)malloc(sizeof(char*)*width);

	// record the width
	board->width = width;

	// instantiate the cells
	for (i=0; i<width;i++)
		board->cell[i] = (char*)malloc(width);

	// return the board
	return board;
}

// frees a board
Board * freeBoard(Board * board)
{
	int i = 0;

	// function call message
	if (DEBUGMODE == 1)
	{
		printf("freeBoard(width = %d)\n", board->width);
	}

	if(board != NULL)
	{
		// free the cells;
		for(i=0; i<board ->width; i++)
		{
			if (board->cell[i] != NULL) free(board->cell[i]);
		}

		board = NULL;
	}

	return board;

}

// prints a board to screen
void printBoard(Board * board)
{
	int i = 0, j = 0;
	char* string = (char*)malloc(board->width+1);
	// debug level 1 message
	if (DEBUGMODE == 1) printf("printBoard(board = %d, width = %d)\n", board, board->width);

	// null-terminate the string
	string[board->width] = 0;

	if (board->cell != NULL)											// if the board exists
	{
		for(i = 0; i<board->width; i++)								// iterate through the rows
		{
			if (board->cell[i] != NULL)								// if the row exists								
			{
				// print it out
				strncpy(string,board->cell[i],board->width);
				printf("%2d:", i);
				for(j = 0; j < board->width;j++)printf("%c",board->cell[i][j]);
				printf(":%2d\n",i);
			}
		}
	}
}

// print the tiles out separately
void printTiles(Board * board, int tile)
{
	int i = 0, j = 0;
	int x = 0, y = 0;
	int numTiles = board->width / tile;
	int tilesX = 0, tilesY = 0;
	char* string = (char*)malloc(board->width+1);
	// debug level 1 message
	if (DEBUGMODE == 1) printf("printBoard(board = %d, width = %d)\n", board, board->width);

	string[board->width] = 0;

	if (board->cell != NULL)											// if the board exists
	{
		for(i = 0; i< numTiles; i++)									// for each tile
		{
			for(j = 0; j < numTiles; j++)
			{
				printf("Tile %d, %d:\n", i, j);							// print it
				for(x=0;x<tile;x++)
				{
					printf("%2d:", x);
					for(y=0;y<tile;y++)
					{
						printf("%c", board->cell[i*tile+x][j*tile+y]);
					}
					printf("\n");
				}
			}
			
			printf("\n");
		}
	}
}

// randomize board
Board * randomBoard(int width)
{
	int i =0, j = 0, random = 0;

	// function call message
	if (DEBUGMODE == 1) printf("randomBoard(, width = %d)\n", width);

	Board * board = newBoard(width);

	for(i=0; i<width; i++)
	{
		for(j=0;j<width;j++)
		{
			random = rand()%3;

			if(random == 1)
				board->cell[i][j] = '>';
			else if(random == 2)
				board->cell[i][j] = 'v';
			else
				board->cell[i][j] = ' ';
		}
	}

	if (DEBUGMODE == 3) 
	{
		printf("Random Board:\n");
		printBoard(board);
	}

	return board;

}

// write a board to a file
void saveBoard(Board * board, char * filename)
{
	int i = 0, j = 0, width = board->width;
	char * string = (char *) malloc(width+2);
	FILE * fp = fopen(filename,"w");

	// terminate the output string
	string[width] = '\n';
	string[width+1] = 0;

	// buffer and write the board
	for (i=0; i<width; i++)
	{
		memcpy(string,board->cell[i],width);
		if (DEBUGMODE == 7) printf("writing \"%s\" to file\n", string);
		fwrite(string,width+1,1,fp);
	}

	// clean up
	free(string);
	fclose(fp);
}

// function to do all blue moves
Board * dropAll(Board * board, char symbol)
{
	int i = 0;
	char current;
	int width = board->width;

	if (DEBUGMODE == 1) printf("dropAll(board = %d, width = %d)\n", board, board->width);

	// split up by columns
	#pragma omp parallel for
	for(i=0; i<width; i++)
	{
		int j = 0, nowrap= 0;
		char current = 0;

		// do the column
		for(j=0; j<board->width; j++)
		{
			// see what we have here
			current = board->cell[j][i];
			
			// if it's the right symbol
			if (current == symbol)
			{
				if (DEBUGMODE == 9)printf("found a \'%c\' at [%d][%d]\n", current, j, i);	

				// if it's not at the end and the next one's free
				if(j < width-1 && board->cell[j+1][i] == ' ')
				{
					//drop it
					if (DEBUGMODE == 9)printf("next free( j = %d, width = %d\n", j, width);
					board->cell[j][i] = ' ';
					board->cell[j+1][i] = symbol;
					if (j == 0) nowrap = 1;
					j++;
				}
				// if it is the last one, the first one was free for this whole turn
				else if (j == width-1 &&board->cell[0][i] == ' ' && nowrap == 0)
				{
					// wrap it around
					if (DEBUGMODE == 9)printf("wrap around\n");
					board->cell[j][i] = ' ';
					board->cell[0][i] = symbol;
				}
			}
		}

	}

	return board;
}

// do all red shifts
Board * shiftAll(Board * board, char symbol)
{
	int i = 0, j = 0, nowrap= 0;;
	char current;
	int width = board->width;

	if (DEBUGMODE == 1) printf("shiftAll(board = %d, width = %d)\n", board, board->width);

	//split up by rows
	#pragma omp parallel for
	for(i=0; i<width; i++)
	{		
		int j = 0, nowrap= 0;
		char current = 0;
		
		// do the row
		for(j=0; j<board->width; j++)
		{
			// see what we have here
			current = board->cell[i][j];
			
			// if it's the right symbol
			if (current == symbol)
			{
				if (DEBUGMODE == 9)printf("found a \'%c\' at [%d][%d]\n", current, i, j);

				// if it's not at the end and the next one's free
				if(j < width-1 && board->cell[i][j+1] == ' ')
				{
					// shift it
					if (DEBUGMODE == 9)printf("next free\n");
					board->cell[i][j] = ' ';
					board->cell[i][j+1] = symbol;
					if(j==0) nowrap = 1;
					j++;
				}
				// if it's the end, and the first one is empty and hasn't been shifted
				else if (j == width-1 &&board->cell[i][0] == ' ' && nowrap == 0)
				{
					// wrap it around
					if (DEBUGMODE == 9)printf("wrap around\n");
					board->cell[i][j] = ' ';
					board->cell[i][0] = symbol;
				}
			}
		}
	}
	return board;
}

// function to check if any tiles are above the density threshold
int checkTiles(Board * board, int tile, int density)
{
	int cellsInTile = tile * tile;
	int numTiles = board->width / tile;
	int i = 0, j = 0;
	int x = 0, y = 0;
	int r = 0, b = 0;
	int totalRed = 0, totalBlue = 0;
	int rDensity = 0, bDensity = 0;
	int maxDensity = 0;
	char current = 0;

	// iterate through the tiles
	for(i=0; i<numTiles;i++)
	{
		for(j=0; j<numTiles;j++)
		{
			r = 0;
			b = 0;
			// split up the work by rows in a tile
			// use reduction to sum the r and b counters. also listen to a bit or R & B. aw yeah. that's some chill tunes, bro.
			#pragma omp parallel for private(y,current) reduction(+:r,b)
			for(x=0;x<tile;x++)
			{
				for(y=0;y<tile;y++)
				{
					// count up any red or blues found
					current = board-> cell[i*tile +x][j*tile+y];
					if(current == '>') r++;
					else if(current == 'v') b++;
				}
			}

			// increment the total counters
			// for debugging
			totalRed += r;
			totalBlue += b;

			// calculate the densities
			rDensity = (r*100)/cellsInTile;
			bDensity = (b*100)/cellsInTile;

			// see if we're done
			if (rDensity > density)
			{
				if(DEBUGMODE == 6) printf("rDensity = %d found at tile [%d,%d]. %d reds over %d in tile\n", rDensity, i,j,r,cellsInTile);
				if (rDensity > maxDensity)maxDensity = rDensity;
			}
			if (bDensity > density)
			{
				if(DEBUGMODE == 6) printf("bDensity = %d found at tile [%d,%d]. %d blues over %d in tile\n", bDensity, i,j,b,cellsInTile);
				if (bDensity > maxDensity)maxDensity = bDensity;
			}
		}
	}
	if (DEBUGMODE == 8) printf("%d red tiles, %d blue tiles, %d blank tiles out of %d\n", totalRed, totalBlue, ((board->width * board->width)-(totalRed + totalBlue)), board->width * board->width);
	
	// return highest density found
	return maxDensity;

}

void main (int argc, char** argv)
{
	int i = 0, turn = 0;
	int threads = 0, width = 0, tile = 0, density = 0, numSteps = 0, seed = 0;
	int interactive = 0, half = 0, done = 0, steps = 0, skewed = 0;
	int currentDensity = 0;
	Board * board;
	double wallTime = 0;
	char output[128], input[8], filename[32];

	// initialize strings
	for(i=0; i<128;i++)
	{
		if (i<8) input[i] = 0;
		if (i<32) filename[i] = 0;
		output[i] = 0;
	}

	// start timing
	StartTime();

	// function call message
	if (DEBUGMODE == 1) 
	{
		printf("main(");
		for(i=1; i< argc; i++) printf("%s, ", argv[i]);
		printf(")\n");
	}

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
			interactive = 1;
		else							// on parse failure, politely crash
		{
			printf("Incorrect argument \"%s\"\n", argv[i]);
			exit(0);
		}
		
		// seed the random number generator
		if (seed == 0)
			seed = (int)time(NULL);

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

	// set the threads number
	omp_set_num_threads(threads);

	// debugging: have the threads report in
	if (DEBUGMODE == 2)
	{
		#pragma omp parallel
		{
			int ID = omp_get_thread_num();
			printf("thread %d ready!\n", ID);
		}
	}

	// Generate a random board
	board = randomBoard(width);
	if (DEBUGMODE == 5 || interactive == 1)
	{
		printf("Initial:\n");
		printBoard(board);
	}

	//main loop
	for (turn = 0; turn < numSteps; turn++)
	{
		if(DEBUGMODE == 4) 
		{
			printf("Before:\n");
			printBoard(board);
		}

		// update state variables
		half = 0;
		if(skewed)turn--;

		// read in input if we're in interactive
		if ((interactive==1) && steps < 1)
		{
			for(i=0;i<8;i++)input[i]=0;
			fgets(input,7,stdin);
			
			if(input[0] == 'h') half = 1;
			else if(input[0] == 'c') interactive = 0;
			else if(input[0] == 'x')break;
			else if(input[0] == '\n');
			else if(steps = strtol(input, NULL,10));
			else 
			{
				printf("error\n");
				turn--;
			}

			// account for resynchronization
			if (half == 0 && skewed ==1) half = 1;
		}

		// do red half-step
		if (skewed == 0)
		{
			board = shiftAll(board, '>');

			if (DEBUGMODE == 5 || DEBUGMODE == 4)
			{
				printf("Half Step\n");
				printBoard(board);
			}
		}

		// do blue half-step
		if ((half == 0) || ((half == 1)&&(skewed == 1)))
		{
			board = dropAll(board, 'v');

			// print it out
			if (DEBUGMODE == 5)
			{
				printf("Full Step\n");
				printBoard(board);
			}

			// check Density
			currentDensity = checkTiles(board,tile,density);
			if(DEBUGMODE == 6) printf("Density = %d\n", currentDensity);
		}

		// set the skew flag
		if (half == 1)
		{
			if (skewed == 1) skewed = 0;
			else skewed = 1;
		}

		// print out the new board if in interactive
		if(interactive == 1)
		{
			printf("After step %d:\n", turn);
			if (DEBUGMODE == 6) printTiles(board,tile);
			else printBoard(board);
		}

		// decrement steps counter if we have some queued up
		if (steps >0)steps--;

		// debugging thing for dumping board logs
		if (LOGSTEPS == 1)
		{
			sprintf(filename, "boardLogNew%d.txt", turn);
			saveBoard(board, filename);
		}

		// if we're over density, gtfo.
		if (currentDensity != 0) break;
	}

	// write stuff to file.
	saveBoard(board, "redblue.txt");

	// stop watch
	wallTime = EndTime();

	// compose the args string;
	sprintf(output, "Arguments: ");

	for(i=1; i<argc; i++) sprintf(output,"%s %s", output, argv[i]);

	sprintf(output, "%s\nIterations: %d, Ending Density: %d, Time = %.3f\n", output, turn, currentDensity, wallTime);

	printf(output);

	FILE * fp = fopen("redblue.txt","a");
	fwrite(output, strlen(output), 1, fp);
	fclose(fp);

	// clean up
	freeBoard(board);


}