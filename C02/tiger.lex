%top{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

int charPos=1;
int g_comment_level=0;
void adjust(void);
}

DIGIT [0-9]
ALPHA [a-zA-Z]
IDENTIFER [0-9a-zA-Z_]

%s NORMAL COMMENT

%%
<NORMAL>" "	      {adjust(); continue;}
<NORMAL>"\t"	    {adjust(); continue;}
<NORMAL>\n	      {adjust(); EM_newline(); continue;}
<NORMAL>,	        {adjust(); return COMMA;}
<NORMAL>:	        {adjust(); return COLON;}
<NORMAL>;	        {adjust(); return SEMICOLON;}
<NORMAL>"("       {adjust(); return LPAREN;}
<NORMAL>")"       {adjust(); return RPAREN;}
<NORMAL>"["       {adjust(); return LBRACK;}
<NORMAL>"]"       {adjust(); return RBRACK;}
<NORMAL>"{"       {adjust(); return LBRACE;}
<NORMAL>"}"       {adjust(); return RBRACE;}
<NORMAL>"."       {adjust(); return DOT;}
<NORMAL>"+"       {adjust(); return PLUS;}
<NORMAL>"-"       {adjust(); return MINUS;}
<NORMAL>"*"       {adjust(); return TIMES;}
<NORMAL>"/"       {adjust(); return DIVIDE;}
<NORMAL>"="       {adjust(); return EQ;}
<NORMAL>"<>"      {adjust(); return NEQ;}
<NORMAL>"<"       {adjust(); return LT;}
<NORMAL>"<="      {adjust(); return LE;}
<NORMAL>">"       {adjust(); return GT;}
<NORMAL>">="      {adjust(); return GE;}
<NORMAL>"&"       {adjust(); return AND;}
<NORMAL>"|"       {adjust(); return OR;}
<NORMAL>":="      {adjust(); return ASSIGN;}
<NORMAL>array     {adjust(); return ARRAY;}
<NORMAL>if        {adjust(); return IF;}
<NORMAL>then      {adjust(); return THEN;}
<NORMAL>else      {adjust(); return ELSE;}
<NORMAL>while     {adjust(); return WHILE;}
<NORMAL>for       {adjust(); return FOR;}
<NORMAL>to        {adjust(); return TO;}
<NORMAL>do        {adjust(); return DO;}
<NORMAL>let       {adjust(); return LET;}
<NORMAL>in  	    {adjust(); return IN;}
<NORMAL>end       {adjust(); return END;}
<NORMAL>of  	    {adjust(); return OF;}
<NORMAL>break     {adjust(); return BREAK;}
<NORMAL>nil       {adjust(); return NIL;}
<NORMAL>function  {adjust(); return FUNCTION;}
<NORMAL>var       {adjust(); return VAR;}
<NORMAL>type      {adjust(); return TYPE;}
<NORMAL>{DIGIT}+          {adjust(); yylval.ival=atoi(yytext); return INT;}
<NORMAL>{ALPHA}{IDENTIFER}*	  {adjust(); yylval.sval=String(yytext); return ID;}
<NORMAL>\"[^"]+\"         {adjust(); yylval.sval=String(yytext); return STRING;}
<NORMAL>"/*"      {
                    charPos+=yyleng;
                    g_comment_level++;
                    BEGIN COMMENT;
                  }
<NORMAL>.	       {adjust(); EM_error(EM_tokPos,"illegal token");}
<COMMENT>"/*"     {
                    charPos+=yyleng;
                    g_comment_level++;
                  }
<COMMENT>"*/"     {
                    charPos+=yyleng;
                    if (--g_comment_level == 0) {
                      BEGIN NORMAL;
                    }
                  }
<COMMENT>.        {charPos+=yyleng;}
.	                {BEGIN NORMAL; yyless(0);}

%%

int yywrap(void)
{
 charPos=1;
 return 1;
}

void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}