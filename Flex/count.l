%{
	int charcount, linecount;
%}

%%

.	charcount++;
\n	linecount++;

%%

int main()
{
	yylex();
	printf("%d characters on %d lines\n", charcount, linecount);
	return 0;
}