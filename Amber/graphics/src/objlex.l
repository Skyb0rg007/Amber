%top {
    union AB_objloader_token {
        unsigned natural;
        char *string;
        float floating;
    };

    #define YYSTYPE union AB_objloader_token

    /* Keep yycolumn updated */
    #define YY_USER_ACTION do { \
        yycolumn += yyleng; \
    } while (0);
}

%{
    #include <Amber/util/common.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "objparse.h"
%}

/* Make a reentrant lexer */
%option reentrant
/* Do not wrap the input */
%option noyywrap
/* Use yylval to pass values of tokens */
%option bison-bridge
/* Increment the yylineno variable for me */
%option yylineno
/* Remove dependency on unistd.h (isatty) - assume non-interactive */
%option nounistd never-interactive
/* Export objloader_lex() */
%option prefix="objloader_"

NUMBER  [+-]?[0-9]*\.?[0-9]+
NATURAL [1-9][0-9]*

/* Matches the rest of a line after an 'o' directive */
%x STATE_OBJECT_NAME
/* After the first 'f' or 'l', only 'f' and 'l' are valid */
%s FACE_MODE

%%

[ \t]                 { /* Whitespace - do nothing */ }
"\n"                  { return NEWLINE; }
{NATURAL}             { yylval->natural = atoi(yytext); return NATURAL; }
{NUMBER}              { yylval->floating = strtod(yytext, NULL); return NUMBER; }
<INITIAL>^"vt "       { return TEXTURE_COORD; }
<INITIAL>^"vn "       { return VERTEX_NORMAL; }
<INITIAL>^"v "        { return GEO_VERTEX; }
<INITIAL>^"s "[ \t]*1 { return SMOOTHING_ENABLE; }
<INITIAL>^"o"[\t ]+   { BEGIN(STATE_OBJECT_NAME); return NAMED_OBJECT; }
<STATE_OBJECT_NAME>.* { BEGIN(INITIAL);
    yylval->string = AB_STRDUP(yytext); return OBJECT_NAME;
}
^"f "                 { BEGIN(FACE_MODE); return POLY_FACE; }
<FACE_MODE>"/"        { return SLASH; }
"#".*                 { /* Comment - do nothing */ }
.                     {
    AB_LOG_WARN("[Objloader] Unknown character on line %d:%d - '%s'\n",
        yylineno, yycolumn, yytext);
}

%%

