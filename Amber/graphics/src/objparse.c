/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
#include <assert.h>
/************ Begin %include sections from the grammar ************************/
#line 2 "../Amber/graphics/src/objparse.y"

    #include <Amber/graphics/objloader.h>
    #include <Amber/util/vector.h>
    #include <Amber/util/common.h>
    #include <cglm/cglm.h>

    #include "objparse.h" /* Contains the terminal ids */
    #include "objlex.h"   /* Contains lexer function prototypes */

    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>

    struct AB_objloader_temp {
        char *name;
        AB_vec(vec3) vertices_tmp;
        AB_vec(vec2) tex_coords_tmp;
        AB_vec(vec3) normals_tmp;
        AB_vec(uvec3) vertex_indices, tex_coord_indices, normal_indices;
    };
#line 50 "../Amber/graphics/src/objparse.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ObjloaderTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ObjloaderTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ObjloaderARG_SDECL     A static variable declaration for the %extra_argument
**    ObjloaderARG_PDECL     A parameter declaration for the %extra_argument
**    ObjloaderARG_PARAM     Code to pass %extra_argument as a subroutine parameter
**    ObjloaderARG_STORE     Code to store %extra_argument into yypParser
**    ObjloaderARG_FETCH     Code to extract %extra_argument from yypParser
**    ObjloaderCTX_*         As ObjloaderARG_ except for %extra_context
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYNTOKEN           Number of terminal symbols
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 25
#define YYACTIONTYPE unsigned char
#define ObjloaderTOKENTYPE  union AB_objloader_token 
typedef union {
  int yyinit;
  ObjloaderTOKENTYPE yy0;
  unsigned yy12;
  struct {unsigned v, t;} yy34;
  float yy35;
  struct {unsigned v, t, n;} yy50;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ObjloaderARG_SDECL  struct AB_objloader_temp *ast ;
#define ObjloaderARG_PDECL , struct AB_objloader_temp *ast 
#define ObjloaderARG_PARAM ,ast 
#define ObjloaderARG_FETCH  struct AB_objloader_temp *ast =yypParser->ast ;
#define ObjloaderARG_STORE yypParser->ast =ast ;
#define ObjloaderCTX_SDECL
#define ObjloaderCTX_PDECL
#define ObjloaderCTX_PARAM
#define ObjloaderCTX_FETCH
#define ObjloaderCTX_STORE
#define YYNSTATE             31
#define YYNRULE              21
#define YYNTOKEN             13
#define YY_MAX_SHIFT         30
#define YY_MIN_SHIFTREDUCE   49
#define YY_MAX_SHIFTREDUCE   69
#define YY_ERROR_ACTION      70
#define YY_ACCEPT_ACTION     71
#define YY_NO_ACTION         72
#define YY_MIN_REDUCE        73
#define YY_MAX_REDUCE        93
/************* End control #defines *******************************************/
#define YY_NLOOKAHEAD ((int)(sizeof(yy_lookahead)/sizeof(yy_lookahead[0])))

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X.
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (64)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    71,   17,   19,   17,   17,   17,   17,   17,   10,    7,
 /*    10 */     5,    2,   20,   16,   14,   12,   30,   18,   30,   30,
 /*    20 */    30,   30,   30,   10,    7,    5,    2,   20,   60,   75,
 /*    30 */    59,   85,    1,   23,    3,    4,   74,    6,   73,    8,
 /*    40 */     9,   27,   29,   56,   69,   86,   78,   11,   55,   77,
 /*    50 */    13,   76,   15,   24,   21,   72,   22,   58,   25,    1,
 /*    60 */    26,   57,   72,   28,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    13,   14,   15,   16,   17,   18,   19,   20,    2,    3,
 /*    10 */     4,    5,    6,   22,   23,   24,   14,   11,   16,   17,
 /*    20 */    18,   19,   20,    2,    3,    4,    5,    6,    8,   21,
 /*    30 */    10,    0,    1,    8,   21,   21,   21,   21,   21,   21,
 /*    40 */    21,    8,    8,    8,   12,    0,   24,   24,    7,   23,
 /*    50 */    23,   22,   22,    9,    8,   25,    9,    8,    8,    1,
 /*    60 */     9,    8,   25,    9,   25,   25,   25,   25,   25,   25,
 /*    70 */    25,   25,   25,   25,   25,   25,   25,
};
#define YY_SHIFT_COUNT    (30)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (58)
static const unsigned char yy_shift_ofst[] = {
 /*     0 */     6,   21,   25,   20,   20,   20,   20,   20,   20,   20,
 /*    10 */    20,   33,   33,   34,   34,   35,   35,   31,   32,   45,
 /*    20 */    41,   44,   46,   47,   49,   44,   50,   51,   53,   54,
 /*    30 */    58,
};
#define YY_REDUCE_COUNT (16)
#define YY_REDUCE_MIN   (-13)
#define YY_REDUCE_MAX   (30)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -13,    2,   -9,    8,   13,   14,   15,   16,   17,   18,
 /*    10 */    19,   22,   23,   26,   27,   29,   30,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
 /*    10 */    70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
 /*    20 */    70,   81,   70,   80,   70,   70,   70,   70,   70,   70,
 /*    30 */    92,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ObjloaderARG_SDECL                /* A place to hold %extra_argument */
  ObjloaderCTX_SDECL                /* A place to hold %extra_context */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ObjloaderTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#if defined(YYCOVERAGE) || !defined(NDEBUG)
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  /*    0 */ "$",
  /*    1 */ "NEWLINE",
  /*    2 */ "GEO_VERTEX",
  /*    3 */ "TEXTURE_COORD",
  /*    4 */ "VERTEX_NORMAL",
  /*    5 */ "POLY_FACE",
  /*    6 */ "NAMED_OBJECT",
  /*    7 */ "OBJECT_NAME",
  /*    8 */ "NATURAL",
  /*    9 */ "SLASH",
  /*   10 */ "NUMBER",
  /*   11 */ "PARAM_SPACE_VERTICES",
  /*   12 */ "LINE_ELEMENT",
  /*   13 */ "program",
  /*   14 */ "obj_line",
  /*   15 */ "misc",
  /*   16 */ "vertex",
  /*   17 */ "tex_coord",
  /*   18 */ "normal",
  /*   19 */ "index",
  /*   20 */ "object_name",
  /*   21 */ "floating",
  /*   22 */ "v_index",
  /*   23 */ "vt_index",
  /*   24 */ "vtn_index",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "vertex ::= GEO_VERTEX floating floating floating",
 /*   1 */ "tex_coord ::= TEXTURE_COORD floating floating",
 /*   2 */ "normal ::= VERTEX_NORMAL floating floating floating",
 /*   3 */ "index ::= POLY_FACE v_index v_index v_index",
 /*   4 */ "index ::= POLY_FACE vt_index vt_index vt_index",
 /*   5 */ "index ::= POLY_FACE vtn_index vtn_index vtn_index",
 /*   6 */ "object_name ::= NAMED_OBJECT OBJECT_NAME",
 /*   7 */ "v_index ::= NATURAL",
 /*   8 */ "vt_index ::= NATURAL SLASH NATURAL",
 /*   9 */ "vtn_index ::= NATURAL SLASH NATURAL SLASH NATURAL",
 /*  10 */ "floating ::= NUMBER",
 /*  11 */ "floating ::= NATURAL",
 /*  12 */ "program ::= obj_line",
 /*  13 */ "program ::= misc",
 /*  14 */ "obj_line ::= vertex",
 /*  15 */ "obj_line ::= tex_coord",
 /*  16 */ "obj_line ::= normal",
 /*  17 */ "obj_line ::= index",
 /*  18 */ "obj_line ::= object_name",
 /*  19 */ "obj_line ::= obj_line NEWLINE obj_line",
 /*  20 */ "misc ::= PARAM_SPACE_VERTICES LINE_ELEMENT",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ObjloaderAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void ObjloaderInit(void *yypRawParser ObjloaderCTX_PDECL){
  yyParser *yypParser = (yyParser*)yypRawParser;
  ObjloaderCTX_STORE
#ifdef YYTRACKMAXSTACKDEPTH
  yypParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  yypParser->yytos = NULL;
  yypParser->yystack = NULL;
  yypParser->yystksz = 0;
  if( yyGrowStack(yypParser) ){
    yypParser->yystack = &yypParser->yystk0;
    yypParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  yypParser->yytos = yypParser->yystack;
  yypParser->yystack[0].stateno = 0;
  yypParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  yypParser->yystackEnd = &yypParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef Objloader_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Objloader and ObjloaderFree.
*/
void *ObjloaderAlloc(void *(*mallocProc)(YYMALLOCARGTYPE) ObjloaderCTX_PDECL){
  yyParser *yypParser;
  yypParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( yypParser ){
    ObjloaderCTX_STORE
    ObjloaderInit(yypParser ObjloaderCTX_PARAM);
  }
  return (void*)yypParser;
}
#endif /* Objloader_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ObjloaderARG_FETCH
  ObjloaderCTX_FETCH
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void ObjloaderFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef Objloader_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ObjloaderFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  ObjloaderFinalize(p);
  (*freeProc)(p);
}
#endif /* Objloader_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ObjloaderStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/* This array of booleans keeps track of the parser statement
** coverage.  The element yycoverage[X][Y] is set when the parser
** is in state X and has a lookahead token Y.  In a well-tested
** systems, every element of this matrix should end up being set.
*/
#if defined(YYCOVERAGE)
static unsigned char yycoverage[YYNSTATE][YYNTOKEN];
#endif

/*
** Write into out a description of every state/lookahead combination that
**
**   (1)  has not been used by the parser, and
**   (2)  is not a syntax error.
**
** Return the number of missed state/lookahead combinations.
*/
#if defined(YYCOVERAGE)
int ObjloaderCoverage(FILE *out){
  int stateno, iLookAhead, i;
  int nMissed = 0;
  for(stateno=0; stateno<YYNSTATE; stateno++){
    i = yy_shift_ofst[stateno];
    for(iLookAhead=0; iLookAhead<YYNTOKEN; iLookAhead++){
      if( yy_lookahead[i+iLookAhead]!=iLookAhead ) continue;
      if( yycoverage[stateno][iLookAhead]==0 ) nMissed++;
      if( out ){
        fprintf(out,"State %d lookahead %s %s\n", stateno,
                yyTokenName[iLookAhead],
                yycoverage[stateno][iLookAhead] ? "ok" : "missed");
      }
    }
  }
  return nMissed;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static YYACTIONTYPE yy_find_shift_action(
  YYCODETYPE iLookAhead,    /* The look-ahead token */
  YYACTIONTYPE stateno      /* Current state number */
){
  int i;

  if( stateno>YY_MAX_SHIFT ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
#if defined(YYCOVERAGE)
  yycoverage[stateno][iLookAhead] = 1;
#endif
  do{
    i = yy_shift_ofst[stateno];
    assert( i>=0 );
    /* assert( i+YYNTOKEN<=(int)YY_NLOOKAHEAD ); */
    assert( iLookAhead!=YYNOCODE );
    assert( iLookAhead < YYNTOKEN );
    i += iLookAhead;
    if( i>=YY_NLOOKAHEAD || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          j<(int)(sizeof(yy_lookahead)/sizeof(yy_lookahead[0])) &&
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static YYACTIONTYPE yy_find_reduce_action(
  YYACTIONTYPE stateno,     /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ObjloaderARG_FETCH
   ObjloaderCTX_FETCH
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
#line 30 "../Amber/graphics/src/objparse.y"

    fprintf(stderr, "Parser stack overflow!\n");
#line 762 "../Amber/graphics/src/objparse.c"
/******** End %stack_overflow code ********************************************/
   ObjloaderARG_STORE /* Suppress warning about unused %extra_argument var */
   ObjloaderCTX_STORE
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState, const char *zTag){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%s%s '%s', go to state %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%s%s '%s', pending reduce %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState - YY_MIN_REDUCE);
    }
  }
}
#else
# define yyTraceShift(X,Y,Z)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  YYACTIONTYPE yyNewState,      /* The new state to shift in */
  YYCODETYPE yyMajor,           /* The major token to shift in */
  ObjloaderTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState, "Shift");
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  {   16,   -4 }, /* (0) vertex ::= GEO_VERTEX floating floating floating */
  {   17,   -3 }, /* (1) tex_coord ::= TEXTURE_COORD floating floating */
  {   18,   -4 }, /* (2) normal ::= VERTEX_NORMAL floating floating floating */
  {   19,   -4 }, /* (3) index ::= POLY_FACE v_index v_index v_index */
  {   19,   -4 }, /* (4) index ::= POLY_FACE vt_index vt_index vt_index */
  {   19,   -4 }, /* (5) index ::= POLY_FACE vtn_index vtn_index vtn_index */
  {   20,   -2 }, /* (6) object_name ::= NAMED_OBJECT OBJECT_NAME */
  {   22,   -1 }, /* (7) v_index ::= NATURAL */
  {   23,   -3 }, /* (8) vt_index ::= NATURAL SLASH NATURAL */
  {   24,   -5 }, /* (9) vtn_index ::= NATURAL SLASH NATURAL SLASH NATURAL */
  {   21,   -1 }, /* (10) floating ::= NUMBER */
  {   21,   -1 }, /* (11) floating ::= NATURAL */
  {   13,   -1 }, /* (12) program ::= obj_line */
  {   13,   -1 }, /* (13) program ::= misc */
  {   14,   -1 }, /* (14) obj_line ::= vertex */
  {   14,   -1 }, /* (15) obj_line ::= tex_coord */
  {   14,   -1 }, /* (16) obj_line ::= normal */
  {   14,   -1 }, /* (17) obj_line ::= index */
  {   14,   -1 }, /* (18) obj_line ::= object_name */
  {   14,   -3 }, /* (19) obj_line ::= obj_line NEWLINE obj_line */
  {   15,   -2 }, /* (20) misc ::= PARAM_SPACE_VERTICES LINE_ELEMENT */
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static YYACTIONTYPE yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  ObjloaderTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
  ObjloaderCTX_PDECL                   /* %extra_context */
){
  int yygoto;                     /* The next state */
  YYACTIONTYPE yyact;             /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ObjloaderARG_FETCH
  (void)yyLookahead;
  (void)yyLookaheadToken;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    if( yysize ){
      fprintf(yyTraceFILE, "%sReduce %d [%s], go to state %d.\n",
        yyTracePrompt,
        yyruleno, yyRuleName[yyruleno], yymsp[yysize].stateno);
    }else{
      fprintf(yyTraceFILE, "%sReduce %d [%s].\n",
        yyTracePrompt, yyruleno, yyRuleName[yyruleno]);
    }
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      /* The call to yyStackOverflow() above pops the stack until it is
      ** empty, causing the main parser loop to exit.  So the return value
      ** is never used and does not matter. */
      return 0;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        /* The call to yyStackOverflow() above pops the stack until it is
        ** empty, causing the main parser loop to exit.  So the return value
        ** is never used and does not matter. */
        return 0;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* vertex ::= GEO_VERTEX floating floating floating */
#line 63 "../Amber/graphics/src/objparse.y"
{
    vec3 *vert = AB_vec_pushp(&ast->vertices_tmp); AB_ASSERT(vert);
    (*vert)[0] = yymsp[-2].minor.yy35; (*vert)[1] = yymsp[-1].minor.yy35; (*vert)[2] = yymsp[0].minor.yy35;
}
#line 951 "../Amber/graphics/src/objparse.c"
        break;
      case 1: /* tex_coord ::= TEXTURE_COORD floating floating */
#line 70 "../Amber/graphics/src/objparse.y"
{
    vec2 *coord = AB_vec_pushp(&ast->tex_coords_tmp); AB_ASSERT(coord);
    (*coord)[0] = yymsp[-1].minor.yy35; (*coord)[1] = yymsp[0].minor.yy35;
}
#line 959 "../Amber/graphics/src/objparse.c"
        break;
      case 2: /* normal ::= VERTEX_NORMAL floating floating floating */
#line 78 "../Amber/graphics/src/objparse.y"
{
    vec3 *norm = AB_vec_pushp(&ast->normals_tmp); AB_ASSERT(norm);
    (*norm)[0] = yymsp[-2].minor.yy35; (*norm)[1] = yymsp[-1].minor.yy35; (*norm)[2] = yymsp[0].minor.yy35;
    glm_vec3_normalize(*norm);
}
#line 968 "../Amber/graphics/src/objparse.c"
        break;
      case 3: /* index ::= POLY_FACE v_index v_index v_index */
#line 85 "../Amber/graphics/src/objparse.y"
{
    uvec3 *vidx = AB_vec_pushp(&ast->vertex_indices); AB_ASSERT(vidx);
    (*vidx)[0] = yymsp[-2].minor.yy12; (*vidx)[1] = yymsp[-1].minor.yy12; (*vidx)[2] = yymsp[0].minor.yy12;
}
#line 976 "../Amber/graphics/src/objparse.c"
        break;
      case 4: /* index ::= POLY_FACE vt_index vt_index vt_index */
#line 89 "../Amber/graphics/src/objparse.y"
{
    uvec3 *vidx = AB_vec_pushp(&ast->vertex_indices); AB_ASSERT(vidx);
    uvec3 *tidx = AB_vec_pushp(&ast->tex_coord_indices); AB_ASSERT(tidx);
    (*vidx)[0] = yymsp[-2].minor.yy34.v; (*vidx)[1] = yymsp[-1].minor.yy34.v; (*vidx)[2] = yymsp[0].minor.yy34.v;
    (*tidx)[0] = yymsp[-2].minor.yy34.t; (*tidx)[1] = yymsp[-1].minor.yy34.t; (*tidx)[2] = yymsp[0].minor.yy34.t;
}
#line 986 "../Amber/graphics/src/objparse.c"
        break;
      case 5: /* index ::= POLY_FACE vtn_index vtn_index vtn_index */
#line 95 "../Amber/graphics/src/objparse.y"
{
    uvec3 *vidx = AB_vec_pushp(&ast->vertex_indices); AB_ASSERT(vidx);
    uvec3 *tidx = AB_vec_pushp(&ast->tex_coord_indices); AB_ASSERT(tidx);
    uvec3 *nidx = AB_vec_pushp(&ast->normal_indices); AB_ASSERT(nidx);
    (*vidx)[0] = yymsp[-2].minor.yy50.v; (*vidx)[1] = yymsp[-1].minor.yy50.v; (*vidx)[2] = yymsp[0].minor.yy50.v;
    (*tidx)[0] = yymsp[-2].minor.yy50.t; (*tidx)[1] = yymsp[-1].minor.yy50.t; (*tidx)[2] = yymsp[0].minor.yy50.t;
    (*nidx)[0] = yymsp[-2].minor.yy50.n; (*nidx)[1] = yymsp[-1].minor.yy50.n; (*nidx)[2] = yymsp[0].minor.yy50.n;
}
#line 998 "../Amber/graphics/src/objparse.c"
        break;
      case 6: /* object_name ::= NAMED_OBJECT OBJECT_NAME */
#line 104 "../Amber/graphics/src/objparse.y"
{
    ast->name = yymsp[0].minor.yy0.string;
}
#line 1005 "../Amber/graphics/src/objparse.c"
        break;
      case 7: /* v_index ::= NATURAL */
#line 111 "../Amber/graphics/src/objparse.y"
{ yylhsminor.yy12 = yymsp[0].minor.yy0.natural; }
#line 1010 "../Amber/graphics/src/objparse.c"
  yymsp[0].minor.yy12 = yylhsminor.yy12;
        break;
      case 8: /* vt_index ::= NATURAL SLASH NATURAL */
#line 115 "../Amber/graphics/src/objparse.y"
{ yylhsminor.yy34.v = yymsp[-2].minor.yy0.natural; yylhsminor.yy34.t = yymsp[0].minor.yy0.natural; }
#line 1016 "../Amber/graphics/src/objparse.c"
  yymsp[-2].minor.yy34 = yylhsminor.yy34;
        break;
      case 9: /* vtn_index ::= NATURAL SLASH NATURAL SLASH NATURAL */
#line 119 "../Amber/graphics/src/objparse.y"
{ yylhsminor.yy50.v = yymsp[-4].minor.yy0.natural; yylhsminor.yy50.t = yymsp[-2].minor.yy0.natural; yylhsminor.yy50.n = yymsp[0].minor.yy0.natural; }
#line 1022 "../Amber/graphics/src/objparse.c"
  yymsp[-4].minor.yy50 = yylhsminor.yy50;
        break;
      case 10: /* floating ::= NUMBER */
#line 123 "../Amber/graphics/src/objparse.y"
{ yylhsminor.yy35 = yymsp[0].minor.yy0.floating; }
#line 1028 "../Amber/graphics/src/objparse.c"
  yymsp[0].minor.yy35 = yylhsminor.yy35;
        break;
      case 11: /* floating ::= NATURAL */
#line 124 "../Amber/graphics/src/objparse.y"
{ yylhsminor.yy35 = (float)yymsp[0].minor.yy0.natural; }
#line 1034 "../Amber/graphics/src/objparse.c"
  yymsp[0].minor.yy35 = yylhsminor.yy35;
        break;
      default:
      /* (12) program ::= obj_line */ yytestcase(yyruleno==12);
      /* (13) program ::= misc */ yytestcase(yyruleno==13);
      /* (14) obj_line ::= vertex (OPTIMIZED OUT) */ assert(yyruleno!=14);
      /* (15) obj_line ::= tex_coord (OPTIMIZED OUT) */ assert(yyruleno!=15);
      /* (16) obj_line ::= normal (OPTIMIZED OUT) */ assert(yyruleno!=16);
      /* (17) obj_line ::= index (OPTIMIZED OUT) */ assert(yyruleno!=17);
      /* (18) obj_line ::= object_name (OPTIMIZED OUT) */ assert(yyruleno!=18);
      /* (19) obj_line ::= obj_line NEWLINE obj_line */ yytestcase(yyruleno==19);
      /* (20) misc ::= PARAM_SPACE_VERTICES LINE_ELEMENT */ yytestcase(yyruleno==20);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  yymsp += yysize+1;
  yypParser->yytos = yymsp;
  yymsp->stateno = (YYACTIONTYPE)yyact;
  yymsp->major = (YYCODETYPE)yygoto;
  yyTraceShift(yypParser, yyact, "... then shift");
  return yyact;
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ObjloaderARG_FETCH
  ObjloaderCTX_FETCH
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
#line 36 "../Amber/graphics/src/objparse.y"

    fprintf(stderr, "Parsing failed!\n");
#line 1091 "../Amber/graphics/src/objparse.c"
/************ End %parse_failure code *****************************************/
  ObjloaderARG_STORE /* Suppress warning about unused %extra_argument variable */
  ObjloaderCTX_STORE
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ObjloaderTOKENTYPE yyminor         /* The minor type of the error token */
){
  ObjloaderARG_FETCH
  ObjloaderCTX_FETCH
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
/************ End %syntax_error code ******************************************/
  ObjloaderARG_STORE /* Suppress warning about unused %extra_argument variable */
  ObjloaderCTX_STORE
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ObjloaderARG_FETCH
  ObjloaderCTX_FETCH
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
#line 33 "../Amber/graphics/src/objparse.y"

    fprintf(stderr, "Parsing succeeded!\n");
#line 1138 "../Amber/graphics/src/objparse.c"
/*********** End %parse_accept code *******************************************/
  ObjloaderARG_STORE /* Suppress warning about unused %extra_argument variable */
  ObjloaderCTX_STORE
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ObjloaderAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Objloader(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ObjloaderTOKENTYPE yyminor       /* The value for the token */
  ObjloaderARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  YYACTIONTYPE yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser = (yyParser*)yyp;  /* The parser */
  ObjloaderCTX_FETCH
  ObjloaderARG_STORE

  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif

  yyact = yypParser->yytos->stateno;
#ifndef NDEBUG
  if( yyTraceFILE ){
    if( yyact < YY_MIN_REDUCE ){
      fprintf(yyTraceFILE,"%sInput '%s' in state %d\n",
              yyTracePrompt,yyTokenName[yymajor],yyact);
    }else{
      fprintf(yyTraceFILE,"%sInput '%s' with pending reduce %d\n",
              yyTracePrompt,yyTokenName[yymajor],yyact-YY_MIN_REDUCE);
    }
  }
#endif

  do{
    assert( yyact==yypParser->yytos->stateno );
    yyact = yy_find_shift_action((YYCODETYPE)yymajor,yyact);
    if( yyact >= YY_MIN_REDUCE ){
      yyact = yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,
                        yyminor ObjloaderCTX_PARAM);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,(YYCODETYPE)yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      break;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_accept(yypParser);
      return;
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) > YY_MAX_SHIFTREDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
      if( yymajor==YYNOCODE ) break;
      yyact = yypParser->yytos->stateno;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      break;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      break;
#endif
    }
  }while( yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}

/*
** Return the fallback token corresponding to canonical token iToken, or
** 0 if iToken has no fallback.
*/
int ObjloaderFallback(int iToken){
#ifdef YYFALLBACK
  if( iToken<(int)(sizeof(yyFallback)/sizeof(yyFallback[0])) ){
    return yyFallback[iToken];
  }
#else
  (void)iToken;
#endif
  return 0;
}
#line 128 "../Amber/graphics/src/objparse.y"


    int AB_load_obj(FILE *infile, struct AB_mesh_info *out)
    {
        /* Setup structure */
        AB_vec_init(&out->vertices);
        AB_vec_init(&out->tex_coords);
        AB_vec_init(&out->normals);

        /* Setup lexer */
        yyscan_t scanner;
        objloader_lex_init(&scanner);
        objloader_set_in(infile, scanner);
        
        /* Setup parser on the stack */
        struct yyParser parser;
        ObjloaderInit(&parser);

        int tok; 
        union AB_objloader_token val;
        struct AB_objloader_temp temp = {0};
        do {
            tok = objloader_lex(&val, scanner);
            Objloader(&parser, tok, val, &temp);
            int loc = objloader_get_lineno(scanner);
            printf("Lineno = %d\n", loc);
        } while (!feof(infile));

        /* Cleanup tmp structure */
        AB_vec_destroy(&temp.vertices_tmp);
        AB_vec_destroy(&temp.tex_coords_tmp);
        AB_vec_destroy(&temp.normals_tmp);
        AB_vec_destroy(&temp.normal_indices);
        AB_vec_destroy(&temp.vertex_indices);
        AB_vec_destroy(&temp.tex_coord_indices);
        /* Cleanup parser */
        ObjloaderFinalize(&parser);
        /* Cleanup lexer */
        objloader_lex_destroy(scanner);
        return 0;
    }

#line 1390 "../Amber/graphics/src/objparse.c"
