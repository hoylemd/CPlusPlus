#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <omp.h>
#include "wallclock.h"

/*pseudocode
 *
 * forest: list of trees
 * tree: list of verticies and edges; an MST of the verts
 * branches: list of edges leading outwards from a tree
 * 
 * for each tree in forest
 * {
 *		add the shortest branch
 * }
 *
 * for each tree in forest
 *		unify all connected trees
 */

// struct prototypes
struct treeStruct;

typedef struct{
	unsigned vert1;
	unsigned vert2;
	unsigned id;
	int length;
} edge;
typedef struct listitem{
	struct treeStruct * tre;
	edge * edg;
	struct listitem * next;
	struct listitem * prev;
} listItem;
typedef struct{
	unsigned length;
	listItem * list;
} ArrayList;
typedef struct treeStruct{
	ArrayList * extEdges;
	ArrayList * intEdges;
	unsigned id;
	int length;
} tree;
typedef struct {
	tree * t1;
	tree * t2;
	edge * edg;
} mergeJob;

// function prototypes
tree * freeTree(tree * t);
void printTree(tree * t);
void printMergeJob(mergeJob * mj);
void printArrayList(ArrayList * a);
unsigned countElements(ArrayList * a);

// edge stuff
// edge constructor
edge * createEdge(unsigned vert1, unsigned vert2, unsigned id, int length)
{
	// contstruct edge
	edge * ed = (edge*)malloc(sizeof(edge));

	// insert data
	ed->vert1 = vert1;
	ed->vert2 = vert2;
	ed->id = id;
	ed->length = length;

	// return pointer
	return ed;
}
// edge printer
void printEdge(edge * ed)
{
	//printf("pe %p \n", ed);
	if (ed != NULL)
		printf("edge %u: %u %u %d (%p)\n", ed->id, ed->vert1, ed->vert2, ed->length,ed);
	else
		printf("NULL edge!\n");
}
//edge to string
char * edgeToString(edge *ed)
{
	char * ret = (char*)malloc(64);
	if (ed)
	{
		sprintf(ret, "edge %u: %u %u %d (%p)\n", ed->id, ed->vert1, ed->vert2, ed->length,ed);
		return ret;
	}
	return NULL;

}
// edge deconstructor
edge * freeEdge(edge * ed)
{
	if (ed != NULL)
		free(ed);

	return NULL;
}
// function to compare 2 edges
int edgeEquals(edge * ed1, edge * ed2)
{
	if (ed1->id == ed2->id)
		return 1;
	else
		return 0;
}

// listItem stuff
// listItem Constructor
listItem * newListItem(tree * t, edge * e)
{
	//printf("nla\n");
	listItem * li = (listItem*)malloc(sizeof(listItem));
	//printf("li dimensioned\n");
	li->tre = t;
	li->edg = e;
	li->next = NULL;
	li->prev = NULL;

	return li;
}
// listItem destructor
listItem * freeListItem(listItem * li)
{
	if (li)
	{
		li->next = NULL;
		li->prev = NULL;
		if (li->tre) freeTree(li->tre);
		li->tre = NULL;
		if (li->edg) freeEdge(li->edg);
		li->edg = NULL;

		free(li);
	}

	return NULL;
}
// listItem disconnector
// deconstructs a listitem without destroying content
listItem * disconnectListItem(listItem *li)
{
	if (li)
	{
		// clear the pointers
		li->edg = NULL;
		li->tre = NULL;

		// destroy the object;
		freeListItem(li);
	}

	return NULL;
}
// List deconstructor 
listItem * destroyList(listItem * list)
{
	// initialize pointers
	listItem * curr = list;
	listItem * next = NULL;
	if (curr) next = list->next;
	// free each pointer
	while (curr != NULL)
	{
		freeListItem(curr);

		// advance pointers
		curr = next;
		if(curr)next = curr->next;

	}

	return NULL;
}
// List printer
void printList(listItem * list)
{
	listItem * curr = list;

	// print each list item
	while (curr != NULL)
	{
		if (curr->edg)printEdge(curr->edg);
		if (curr->tre)printTree(curr->tre);
		curr = curr->next;
	}
}

// ArrayList stuff
// Initializer
ArrayList * newArrayList()
{
	// allocate the list
	ArrayList * ret = (ArrayList*)malloc(sizeof(ArrayList));

	// initialize fields
	ret->list = NULL;
	ret->length = 0;

	// return it
	return ret;
}
// get by index <<< deprecated?
listItem * ArrayListGetLI(ArrayList *a, unsigned index)
{
	listItem * li = a->list;
	unsigned i = 0;
	
	if(index < a->length)
	{
		for(i=0; i<index; i++)
			li = li->next;
		return li;
	}
	else
		return NULL;
}
//get by id
listItem * ArrayListGetById(ArrayList *a, unsigned id)
{
	// get the list
	listItem * li = a->list;

	// find the listitem with the specified id
	while (li)
	{
		if(li->tre->id == id) return li;
		li = li->next;
	}

	// return null on failure
	return NULL;
}
// deconstructor
ArrayList * freeArrayList(ArrayList * a)
{
	// if the list exists
	if (a)
	{
		// destroy it all
		a->list = destroyList(a->list);
		free(a);
	}

	return NULL;
}
// disconnector
// deconstructs a list without destroying content
ArrayList * disconnectArrayList(ArrayList *a)
{
	// list pointer
	listItem * li = NULL, *next = NULL;

	// if the list exists
	if (a)
	{
		// get the list
		li = a->list;

		// if there are any entries
		if(li)
		{
			// while there are still etries
			while(li)
			{
				// get the next entry
				next = li->next;

				// disconnect all pointers
				li->edg = NULL;
				li->tre = NULL;
				li->next = NULL;
				li->prev = NULL;

				// free the list entry
				free(li);

				// advance
				li = next;
			}

			//clear the list pointer
			a->list = NULL;

			// destroy the list
			free(a);
		}
	}

	return NULL;
}
// appender
void ArrayListAdd(listItem *it, ArrayList *a)
{
	listItem * curr = a->list;
	a->length++;

	// find the end of the list
	if (curr)
	{
		while(curr->next != NULL)
			curr = curr->next;

		// add the new item
		curr->next = it;
		it->prev = curr;
		it->next = NULL;
	} 
	else
	{
		a->list = it;
	}
}
// function to add a list item to an arraylist, sorted by edge length
ArrayList * ArrayListSortAdd(listItem *it, ArrayList* a)
{
	// list item pointers
	listItem * li = a->list, *prev = NULL;
	edge * currEdge = NULL;

	// if the list has contents
	if(li)
	{
		// find the place for this new element
		while(li)
		{
			// if the current edge element is > the one to insert, ibreak
			// (we will insert here)
			if(li->edg->length > it->edg->length) break;

			// if this edge is already in the list nuke the new element.
			if (li->edg == it->edg)
			{
				disconnectListItem(it);
				return a;
			}
			// remember this element as the previous one
			prev = li;

			// get next element
			li = li->next;
		}
		
		// we've found where to insert
		// there is more of the list to go after
		if (li)
		{
			// point the new element to the right places
			it->next = li;
			it->prev = li->prev;
			
			// connect it into the list

			// if this is not the top
			if (li->prev) li->prev->next = it;
			// if it is
			else a->list = it;
			li->prev = it;

			//update the length
			countElements(a);
		}
		// if this is the end of the list
		else
		{
			// append it
			prev->next = it;
			it->prev = prev;

			// update length
			a->length++;
		}
	}
	// list is empty
	else	
	{
		// ensure new element's pointers are null
		it->next = NULL;
		it->prev = NULL;

		// point the list to it
		a->list = it;

		// update length
		a->length++;
		return a;
	}

	return a;
}
// printer
void printArrayList(ArrayList* a)
{
	// list pointer
	listItem * curr = NULL;
	int i = 0;

	// get the list
	curr = a->list;

	// print summary data
	printf("Arraylist:\n length: %d\n Edges:\n",a->length);

	// print out elements
	while(curr)
	{
		if (curr->edg)
		{
			printf("%d: ", i);
			printEdge(curr->edg);
		}
		if (curr->tre)
		{
			printf("%d: ", i);
			printTree(curr->tre);
		}
		curr = curr->next;
		i++;
	}
}
// remover
listItem * ArrayListRemove(listItem *it, ArrayList* a)
{
	// if the item has a previous
	if(it->prev)
		// point it to the next one
		it->prev->next = it->next;
	else
		// if not, make the next one the head
		a->list = it->next;

	// if the item has a next
	if(it->next)
	{
		// point it at the previous one
		it->next->prev = it->prev;

		// if this was the head, advance the head pointer
		if (!it->prev) a->list = it->next;
	}

	// update the length
	countElements(a);

	// return a pointer to the removed item
	return it;
}
// element counter
unsigned countElements(ArrayList * a)
{
	// get the list
	listItem * li = a->list;

	// initialize length
	a->length = 0;

	// increment the lenth field for every entry
	while(li)
	{
		a->length++;
		li= li->next;
	}

	// return the length
	return a->length;
}
// ArrayList cleaner
ArrayList * ArrayListClean(ArrayList * a)
{
	// get the list
	listItem *li = a->list;

	// look at each element
	while(li)
	{
		// ensure edge counts are accurate
		countElements(li->tre->extEdges);
		countElements(li->tre->intEdges);
		if(li->tre)
		{
			// if the tree has no edges, it's been absorbed
			if(li->tre->extEdges->length == 0 && li->tre->intEdges->length == 0)
			{
				// cut it
				ArrayListRemove(li,a);
			}
		}
		// advance
		li = li->next;
	}

	// update the array's length
	countElements(a);

	return a;
}

// tree stuff
// initializer
tree * newTree()
{
	// allocate the tree
	tree * newTree = (tree*)malloc(sizeof(tree));

	// initialize fields
	newTree->extEdges = newArrayList();
	newTree->intEdges = newArrayList();
	newTree->length = 0;

	// return pointer
	return newTree;
}
// deconstructor
tree * freeTree(tree * t)
{
	// if it exists
	if (t!=NULL)
	{
		// destroy fields
		t->extEdges = freeArrayList(t->extEdges);
		t->intEdges = freeArrayList(t->intEdges);

		// destroy struct
		free(t);
	}

	return NULL;
}
// edge redirector
void redirectEdge(listItem * li, unsigned origin, unsigned target)
{
	//redirect the edge verts
	if (li->edg->vert1 == origin)
		li->edg->vert1 = target;

	if (li->edg->vert2 == origin)
		li->edg->vert2 = target;
}
// edge mover
void moveEdge(listItem * li, ArrayList * dest)
{
	// clear pointers
	li->next = NULL;
	li->prev = NULL;

	// Add it to the list
	ArrayListSortAdd(li,dest);
}
// edge internalizer
void cycleEliminator(listItem * li, ArrayList * a)
{
	// next pointer
	listItem * next = NULL;

	// for each member of the list
	while(li)
	{
		// store the next
		next = li->next;

		// check for a cycle
		if(li->edg->vert1 == li->edg->vert2)
		{
			// remove sycles
			ArrayListRemove(li,a);
		}

		// advance
		li =next;
	}
}
void internalizeEdges(tree * target)
{
	// list pointers
	listItem * intern = NULL, * ext= NULL;

	// get the internal list
	intern = target->intEdges->list;

	// for each internal
	while(intern)
	{
		// get the external list
		ext = target->extEdges->list;

		// for each external
		while(ext)
		{
			// on match, remove the eternal
			if (edgeEquals(intern->edg, ext->edg))
			{
				// free it too (?)
				freeListItem(ArrayListRemove(ext,target->extEdges));
			}

			// advance
			ext = ext->next;
		}

		// advance
		intern = intern->next;
	}
}
// calculate Minimum Spanning Tree Length
int calcMSTLength(tree* t)
{
	// list pointer
	listItem * curr = t->intEdges->list;
	// sum of interior edges
	int sum = 0;

	// add the length of all internal edges
	while(curr)
	{
		sum += curr->edg->length;
		curr = curr->next;
	}
	return sum;
}
// printer
void printTree(tree * t)
{
	if (t)
	{
		// calculate the MST length
		t->length = calcMSTLength(t);

		// print the summary data
		printf("Tree #%u:\n %d MST\nExterior Edges:\n", t->id, t->length);

		// print the edges
		printArrayList(t->extEdges);
		printf("\nInterior Edges:\n");
		printArrayList(t->intEdges);
		printf("\n");
	}
	else
		printf("NULL tree!\n");
}

void treeToFile(char * filename, tree * t, double wTime, int nProc)
{
	FILE * fp = fopen(filename,"w");
	listItem * li = NULL;
	char * temp;
	if (t)
	{
		// calculate the MST length
		t->length = calcMSTLength(t);

		// print the summary data
		fprintf(fp,"MST:\nLength %u \nEdges:\n",t->length);

		// print the edges
		//li = t->intEdges->list;
		//while(li)
		//{
			//temp = edgeToString(li->edg);
			//fprintf(fp,"%s\n",temp);
			//free(temp);
			//li = li->next;
		//}

		fprintf(fp,"Time : %f, procs: %d\n",wTime,nProc);
	}
	else
		printf("NULL tree!\n");

	fclose(fp);
}

// mergeJob stuff
// constructor
mergeJob * newMergeJob(tree * t1, tree * t2, edge * edg)
{
	// new struct pointer
	mergeJob * mj = NULL;

	// dimension the structure
	mj = (mergeJob*)malloc(sizeof(mergeJob));

	// populate it
	mj->t1 = t1;
	mj->t2 = t2;
	mj->edg = edg;

	return mj;
}
// disconnector
mergeJob * disconnectMergeJob(mergeJob* mj)
{
	if(mj)
	{
		// disconnect pointers
		mj->t1 = NULL;
		mj->t2 = NULL;
		mj->edg = NULL;

		//destroy structure
		free(mj);
		mj = NULL;
	}
	return NULL;
}
// printer
void printMergeJob(mergeJob * mj)
{
	// if the job exists
	if (mj)
	{
		// print it out
		printf("mergeJob connecting trees %u and %u via ", mj->t1->id, mj->t2->id);
		printEdge(mj->edg);
	}
	else
		printf("NULL mergeJob!\n");
}

// parallelize
// analyze
// report
// optimize?
// report?
// submit

// main function
int main(int argc, char **argv)
{
	//local Variables
	// input character array
	char input[64];
	// Edge String
	char ** edgeString = NULL;
	// file pointers
	FILE * graphFile = NULL;
	// iterators
	unsigned i = 0;
	// counters
	unsigned numVerts = 0, numEdges = 0, numJobs = 0;
	// vertex & edge locals
	unsigned v1 = 0, v2 = 0;
	int length = 0;
	// vertex locks
	bool * vertLock = NULL;
	// lists
	ArrayList * edgeList = NULL, *treeList = NULL, *list =NULL;
	ArrayList * shortestList = NULL;
	// list pointers
	listItem * li = NULL, * newLI = NULL, *intern = NULL, *external = NULL, *next = NULL;
	// edge pointer
	edge * ed = NULL;
	// tree pointers
	tree * tr = NULL, *t1 = NULL, *t2 = NULL, *newRoot = NULL, *other = NULL;
	// mergeJob pointers
	mergeJob ** jobs = NULL, *mj =NULL;
	// parallelism variables
	unsigned nProcessors = 0;
	// clock
	double walltime = 0;

	StartTime();

	// initialization
	// initialize arrays
	memset(input,0,64);

	// initialize lists
	edgeList = newArrayList();
	treeList = newArrayList();
	shortestList = newArrayList();


	// input
	// check the arguments
	if  (argc != 4)
	{
		printf("Invalid arguments\n");
		exit(0);
	}

	// read in the number of processors
	sscanf(argv[3],"%d", &nProcessors);

		// set the threads number
	omp_set_num_threads(nProcessors);

	// make room for the job structures
	jobs = (mergeJob**)malloc(sizeof(mergeJob*)*nProcessors);

	// initialize their pointers
	for(i=0; i<nProcessors;i++)
		jobs[i] = NULL;

	// open the file
	graphFile = fopen(argv[1], "r");
	if (graphFile == NULL)
	{
		printf("File \"%s\"does not exist.\n", argv[1]);
		exit(0);
	}

	// decode the graph
	// read in the header
	fgets(input,64,graphFile);
	sscanf(input,"%u",&numVerts);
	memset(input,0,64);
	fgets(input,64,graphFile);
	sscanf(input,"%u",&numEdges);
	memset(input,0,64);
	// dimension the edge strings array
	edgeString = (char**)malloc(sizeof(char *)*numEdges);

	// dimension the vertex locks
	vertLock = (bool*)malloc(sizeof(bool)*numVerts);
	for(i=0;i<numVerts;i++)
		vertLock[i] = false;

	// store the edges
	for(i=0;i<numEdges;i++)
	{
		//printf("e%u / %u\n",i, numEdges);
		// create & initialize the string
		edgeString[i] = (char*)malloc(128);
		memset(edgeString[i],0,128);

		// read in the data
		fgets(edgeString[i],128,graphFile);
		// parse the data out
		sscanf(edgeString[i],"%u %u %d", &v1,&v2,&length);

		// create the edge & store it
		ed = createEdge(v1,v2,i,length);
		ArrayListSortAdd(newListItem(NULL,ed),edgeList);
	}

	// close the file
	fclose(graphFile);

	// set up trees
	// make the tree array
	for(i=0;i<numVerts;i++)
	{
		//printf("v%d\n",i);
		// initialize a tree
		tr = newTree();

		// identify it
		tr->id = i;

		// add it to the list
		ArrayListAdd(newListItem(tr,NULL),treeList);
	}

	// get the edge list
	li = edgeList->list;
	while(li)
	{
		// get the next edge
		ed = li->edg;

		// make a new list item & insert it for each vertex
		newLI = newListItem(NULL,ed);
		list = ArrayListGetLI(treeList,ed->vert1)->tre->extEdges;
		ArrayListSortAdd(newLI,list);
		newLI = newListItem(NULL,ed);
		list = ArrayListGetLI(treeList,ed->vert2)->tre->extEdges;
		ArrayListSortAdd(newLI,list);

		// advance
		li = li->next;
	}
	
	// Main Work Loop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Main Work Loop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Main Work Loop
	while(treeList->length > 1)
	{
		//printf("z       i\n");
		// Clear the jobs
		for(i=0;i<nProcessors;i++)
			jobs[i] = NULL;

		// Find the shortest Edges
		// reset the short edge list
		disconnectArrayList(shortestList);
		shortestList = newArrayList();
		
		// get the list of trees
		li = treeList->list;
		
		// for each tree

		while(li)
		{
			//li = ArrayListGetById(shortestList,i);
			// make a new list item for it
			newLI = newListItem(NULL,li->tre->extEdges->list->edg);

			// add it to the interior list
			ArrayListSortAdd(newLI,li->tre->intEdges);

			// put it in the list of edges to merge
			// make a new list item for it
			newLI = newListItem(NULL,li->tre->extEdges->list->edg);

			// add it to the interior list
			ArrayListSortAdd(newLI,shortestList);

			// advance
			li = li->next;
		}

		// print out the shortest edges
		//printf("shortlist\n");
		//printArrayList(shortestList);

		// Merge the edges
		// while there are still edges to merge
		while(shortestList->length){
			// Generate jobs
			// get the list of edges to merge
			li = shortestList->list;

			// reset jobs counter
			numJobs = 0;

			// for each one
			i = 0;

			// make as many jobs as possible
			while(li && (numJobs <= nProcessors))
			{
				// get the trees this edge connects
				t1 = ArrayListGetById(treeList,li->edg->vert1)->tre;
				t2 = ArrayListGetById(treeList,li->edg->vert2)->tre;

				// check if these verts are locked
				if(vertLock[t1->id]==false && vertLock[t2->id]==false)
				{
					// if not, lock em
					vertLock[t1->id] = true;
					vertLock[t2->id] = true;

					// make the job struct
					mj = newMergeJob(t1,t2,li->edg);

					// store it
					jobs[numJobs] = mj;
					numJobs++;

					// remove this edge entry
					next = li->next;
					ArrayListRemove(li,shortestList);
					li = next;
					next = NULL;
				} else// advance
					li = li->next;
			}

			// print out the jobs
			//for(i = 0; i< numJobs; i++)
				//printMergeJob(jobs[i]);

			// Merge the trees
			// for each job
#pragma omp parallel for private(mj,t1,t2,newRoot,other,external, next, intern)
			for( i = 0; i<numJobs; i++)
			{
				// get the job
				mj = jobs[i];

				// get the trees
				t1 = mj->t1;
				t2 = mj->t2;

				// determine who is the new root
				if(t1->id < t2->id)
				{
					newRoot = t1;
					other = t2;
				} 
				else if(t2->id < t1->id)
				{
					newRoot = t2;
					other = t1;
				}
				else
				{
					// if for some reason it tries to merge the same tree?
				}

				// redirect all edges from the other to the new root
				external = other->extEdges->list;
				while(external)
				{
					next = external ->next;

					//redirect the edge verts
					redirectEdge(external,other->id,newRoot->id);

					//move the edges from other to newRoot
					moveEdge(external,newRoot->extEdges);

					// advance
					external = next;
				}

				// disconnect other from it's external edges
				other->extEdges->list = NULL;
				other->extEdges->length = 0;

				// get other's internal edges
				intern = other->intEdges->list;
				while(intern)
				{
					next = intern ->next;

					//move the edges from other to newRoot
					moveEdge(intern,newRoot->intEdges);

					// advance
					intern = next;
				}
				// disconnect it from it's internal edges
				other->intEdges->list = NULL;
				other->intEdges->length = 0;

				// eliminate cycles in the root
				external = newRoot->extEdges->list;
				cycleEliminator(external,newRoot->extEdges);

				// unlock the root
				vertLock[newRoot->id] = false;

				// destroy this job
				jobs[i] = disconnectMergeJob(mj);
				jobs[i] = NULL;
			}

			// remove absorbed trees
			ArrayListClean(treeList);
			//printf("treeCount = %u\n", treeList->length);
			//printf("eol\n");
		}
	}
	// print out the tree list
	printf("\nRemaining Minimum Spanning Tree:\n");
	printTree(treeList->list->tre);
	walltime = EndTime();
	printf("Total Execution time: %f seconds\n",walltime);

	treeToFile(argv[2], treeList->list->tre, walltime, nProcessors);

	disconnectArrayList(treeList);
	disconnectArrayList(shortestList);
	freeArrayList(edgeList);
	for(i=0;i<numEdges;i++)free(edgeString[i]);
	free(edgeString);
	free(vertLock);




}