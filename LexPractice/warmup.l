%{
#include <iostream>
#include "globals.h"
using namespace std;
%}

letter		[a-zA-Z]
digit		[0-9]
verbal		({letter}|{digit})
word		{digit}*{letter}{verbal}*
cword		{verbal}+
tag		({hyphenated}|{cword}){whitespace}*[^<>]*
opentag 	<{tag}>
closetag	<\/{tag}>
number		-?{digit}+\.?{digit}*
hyphenated	{cword}(-{cword})+
crazy		({hyphenated}|{cword})
apostrophized	{crazy}('{crazy})+
whitespace 	[ \t\n]+

%%

{word}		{return WORD;}
{opentag}	{return OPENTAG;}
{closetag}	{return CLOSETAG;}
{number}	{return NUMBER;}
{apostrophized}	{return APOSTROPHIZED;}
{hyphenated}	{return HYPHENATED;}
{whitespace}	{printf("whitespace\n");/*skip whitespace*/}
.		{return PUNCTUATION;}

%%

tokenStruct * getToken()
{
    int placeholder = 0;
    static int firstTime = TRUE;
    if (firstTime)
    {
	firstTime = false;
	yyin = stdin;
	yyout = stdout;
    }
    tokenStruct * returner = (tokenStruct*)malloc(sizeof(tokenStruct));
    placeholder = yylex();
    returner->id = (TokenType)placeholder;
    //printf("placeholder: %d\n", placeholder);
    returner->val = (char*)yytext;
    return returner;
}

int yywrap ()
{
    return 1;
}