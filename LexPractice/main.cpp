// Main core code.
// Calls the parser and processes tokens
// Michael D. Hoyle
// 0553453

#include <iostream>
#include "globals.h"
#include "util.h"
using namespace std;

// normalize a string to allcaps
// takes in a string,
// outputs that string in allcaps
string normalize(string str)
{
    int i = 0;
    string norm = str;
    for(i=0; i < str.length(); i++)
    {
	norm[i] = toupper(norm[i]);
    }
    return norm;
}

// gets the first word of a string
// takes in a string, ouputs the first word
string getFirstWord(string str)
{
    //cout << str.find(" ",0) << "\n";
    return str.substr(0,str.find(" ",0)); 
}

// gets the forst word of a tag.
// takes in a string, strips it's taggyness, and outputs the first word of it'
string getFirstWordOfTag(string str)
{
    bool closing = false;
    if (str.find("</",0) != -1) closing = true;
    
    if (closing)
	return getFirstWord(str.substr(2,str.length()-3));
    else
	return getFirstWord(str.substr(1,str.length()-2));
}

// checks if a tag is relevant
// takes in a string, and outpust a boolean true if it's relevant, false if it's not''
bool isRelevant(string tag)
{
    string tagName = normalize(getFirstWordOfTag(tag));
    
   // cout << "testing ["<<tagName<<"]\n";
    if(tagName == "TEXT")
	return true;
    else if(tagName == "DATE")
	return true;
    else if(tagName == "DOC")
	return true;
    else if(tagName == "DOCNO")
	return true;
    else if(tagName == "HEADLINE")
	return true;
    else if(tagName == "LENGTH")
	return true;
    else if(tagName == "P")
	return true;
    else
	return false;
}

// prints out a token
// takes in a string, prints it out
void printToken( Token * token )
{ switch (token->getId()) {
    case ENDFILE:
	cout << "EOF(" << normalize(getFirstWord(token->getVal())) << ")\n";
	break;
    case OPENTAG:
	cout << "OPEN-" << normalize(getFirstWordOfTag(token->getVal())) << "\n";
	break;
    case CLOSETAG:
	cout << "CLOSE-" << normalize(getFirstWordOfTag(token->getVal())) << "\n";
	break;
    case NUMBER:
	cout << "NUMBER(" << getFirstWord(token->getVal()) << ")\n";
	break;
    case APOSTROPHIZED:
	cout << "APOSTROPHIZED(" << getFirstWord(token->getVal()) << ")\n";
	break;
    case HYPHENATED:
	cout << "HYPHENATED(" << getFirstWord(token->getVal()) << ")\n";
	break;
    case WORD:
	cout << "WORD(" << getFirstWord(token->getVal()) << ")\n";
	break;
    case PUNCTUATION:	
	cout << "PUNCTUATION(" << getFirstWord(token->getVal()) << ")\n";
	break;
    default:
	cout << "wtf?!\n";
  }
}

// main routine
int main ( int argc, char * argv[])
{
    int i = 0;
    Token * currentToken, * topTag;
    ListMember * lm;
    int nestLayer = 0;
    Stack * tagStack = new Stack();
    int irrelevantLayers = 0;
    
    // while there's still tokens, get 'em
    while( ((currentToken = new Token(getToken()))->getId()) != ENDFILE  )
    {
	if (irrelevantLayers < 0) irrelevantLayers = 0;
	//cout << "CURRENT TOKEN:::";
	//printToken(currentToken);
	//cout << "\n";
	i++;
	//cout << "iteration " << i << "\n";
	
	// if it's an open tag'
	if(currentToken->getId() == OPENTAG)
	{
	    // if it's relevant...
	    if (isRelevant(currentToken->getVal()) && irrelevantLayers == 0)
	    {
		// print it out
		//cout << "relevant opentag\n";
		printToken(currentToken);
		
		//if (getFirstWordOfTag(currentToken->getVal() )== "DOC") cout << "DOC START-----------\n";
	    }
	    else //otherwise, remember we're irrelevant'
		irrelevantLayers++;
	    
	    // push it to the stack
	    tagStack->MakePush(currentToken);
	    nestLayer++;
	}
	// if it's a close tag'
	else if(currentToken->getId() == CLOSETAG)
	{
	//cout << "closing tag: nesting layer " << nestLayer <<"\n";
	    //cout << "relevant closetag\n"
	    
	    // if we're nested'
	    if (nestLayer > 0)
	    {
		//cout << "relevant closetag, nestlayer > 0\n";
		
		// pop a tag off the stack
		lm = tagStack->Pop();
		topTag = lm->getVal();
		//cout << "close tag. comparing (" << getFirstWordOfTag(topTag->getVal()) << ") to (" << getFirstWordOfTag(currentToken->getVal()) << ")\n";
		
		// check if it's the right tag'
		if(getFirstWordOfTag(topTag->getVal()) == getFirstWordOfTag(currentToken->getVal()))
		{
		    // if it's relevant print er out'
		    if (isRelevant(currentToken->getVal()) && irrelevantLayers == 0)
		    {
			printToken(currentToken);
			//if (getFirstWordOfTag(currentToken->getVal()) == "DOC") cout << "DOC END-----------\n";
		    }
		    nestLayer--;
		    
		    // 
		    if (irrelevantLayers > 0) irrelevantLayers--;
		} else
		{
		    cout << "Nesting error\n";
		    // put the open tag back on the stack
		    tagStack->MakePush(topTag);
		}
		delete lm;
	    }
	    else // if we have a close tag not paired to an open
		cout << "floating close tag.\n";
	}
	else
	{
	    // if we're relevanty, print out the other tokens
	    if (irrelevantLayers == 0)
	    {
		printToken(currentToken);
		//cout << "nextLayer: " << nestLayer << "\n";
	    }
	}
    }
    //cout << "LASTTOKEN: ";
    //printToken(currentToken);
    //cout << "\n";
    if (nestLayer > 0)
    {
	cout << "There are some unclosed tags\n";
	while((lm = tagStack->Pop()) != NULL)
	{
	    topTag = lm->getVal();
	    printToken(topTag);
	}
    }
    
}
