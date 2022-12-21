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
/************ Begin %include sections from the grammar ************************/
#line 1 "grammar.y"
 

#include "grammar.h"
#include <string.h>
#include <stdlib.h>
#include <string>
#include <cassert>
#include <iostream>
#include "cJSON.h"


using namespace std;
int get_token_id (char*);
const char *getValue (cJSON *token);
const char *getLine (cJSON *token);
cJSON *unary (char *fname, cJSON *a); 
cJSON *binary (char *fname, cJSON *a, cJSON *b); 
cJSON *ternary (char *fname, cJSON *a, cJSON *b, cJSON *c); 
char *linenumber;
char *curtoken;
char *curtype;
#line 50 "grammar.c"
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
**    ParseTOKENTYPE     is the data type used for minor type for terminal
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
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 58
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE cJSON *
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE             68
#define YYNRULE              47
#define YY_MAX_SHIFT         67
#define YY_MIN_SHIFTREDUCE   90
#define YY_MAX_SHIFTREDUCE   136
#define YY_MIN_REDUCE        137
#define YY_MAX_REDUCE        183
#define YY_ERROR_ACTION      184
#define YY_ACCEPT_ACTION     185
#define YY_NO_ACTION         186
/************* End control #defines *******************************************/

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
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
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
#define YY_ACTTAB_COUNT (239)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    99,   92,   31,   25,  124,   97,   98,   23,   21,   19,
 /*    10 */    17,   15,   13,   11,   34,   62,  127,  185,   28,  128,
 /*    20 */   118,  119,  120,  121,  122,   63,   32,    2,   35,  134,
 /*    30 */     1,  129,   99,  128,   31,   25,   65,   97,   98,   23,
 /*    40 */    21,   19,   17,   15,   13,   11,   12,   22,   20,   18,
 /*    50 */    16,   14,  118,  119,  120,  121,  122,    9,   44,  126,
 /*    60 */    36,   43,    1,  128,   60,  128,  128,   29,   45,   26,
 /*    70 */    24,   10,   67,  128,   12,   22,   20,   18,   16,   14,
 /*    80 */   125,   33,   95,   61,   12,   22,   20,   18,   16,   14,
 /*    90 */    37,   34,   94,  123,   64,  128,  128,   26,   24,   10,
 /*   100 */    67,   12,   22,   20,   18,   16,   14,   26,   24,   10,
 /*   110 */    67,  130,    6,   12,   22,   20,   18,   16,   14,    4,
 /*   120 */     5,   93,    8,   46,   26,   24,   10,   67,  128,    7,
 /*   130 */    12,   22,   20,   18,   16,   14,   26,   24,   10,   67,
 /*   140 */    42,  139,  139,   96,  139,  128,   12,   22,   20,   18,
 /*   150 */    16,   14,  139,   26,   24,   10,   67,   22,   20,   18,
 /*   160 */    16,   14,  139,   18,   16,   14,  139,  139,  139,   26,
 /*   170 */    24,   10,   67,   14,  139,  139,  139,  139,  139,   26,
 /*   180 */    24,   10,   67,   26,   24,   10,   67,  137,  139,  139,
 /*   190 */    47,   26,   24,   10,   67,  128,   27,   54,   66,   48,
 /*   200 */    30,   59,  128,  139,  128,   49,  128,  139,   58,   50,
 /*   210 */   128,   57,  139,  128,  128,  139,  128,  139,  139,   51,
 /*   220 */    56,   52,   55,    3,  128,  128,  128,  128,  139,   53,
 /*   230 */    41,   38,   40,   39,  128,  128,  128,  128,  128,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    11,   48,   13,   14,   53,   16,   17,   18,   19,   20,
 /*    10 */    21,   22,   23,   24,   49,   52,   51,   46,   47,   54,
 /*    20 */    31,   32,   33,   34,   35,   38,   39,   40,   49,   44,
 /*    30 */    41,   42,   11,   54,   13,   14,   50,   16,   17,   18,
 /*    40 */    19,   20,   21,   22,   23,   24,    2,    3,    4,    5,
 /*    50 */     6,    7,   31,   32,   33,   34,   35,   50,   49,   10,
 /*    60 */    49,   49,   41,   54,   55,   54,   54,   52,   49,   25,
 /*    70 */    26,   27,   28,   54,    2,    3,    4,    5,    6,    7,
 /*    80 */    10,   37,   10,   38,    2,    3,    4,    5,    6,    7,
 /*    90 */    49,   49,   10,   51,   11,   54,   54,   25,   26,   27,
 /*   100 */    28,    2,    3,    4,    5,    6,    7,   25,   26,   27,
 /*   110 */    28,   42,   43,    2,    3,    4,    5,    6,    7,   12,
 /*   120 */    12,   10,   29,   49,   25,   26,   27,   28,   54,   30,
 /*   130 */     2,    3,    4,    5,    6,    7,   25,   26,   27,   28,
 /*   140 */    49,   57,   57,   15,   57,   54,    2,    3,    4,    5,
 /*   150 */     6,    7,   57,   25,   26,   27,   28,    3,    4,    5,
 /*   160 */     6,    7,   57,    5,    6,    7,   57,   57,   57,   25,
 /*   170 */    26,   27,   28,    7,   57,   57,   57,   57,   57,   25,
 /*   180 */    26,   27,   28,   25,   26,   27,   28,    0,   57,   57,
 /*   190 */    49,   25,   26,   27,   28,   54,    9,   49,   11,   49,
 /*   200 */    13,   49,   54,   57,   54,   49,   54,   57,   49,   49,
 /*   210 */    54,   49,   57,   54,   54,   57,   54,   57,   57,   49,
 /*   220 */    49,   49,   49,   36,   54,   54,   54,   54,   57,   49,
 /*   230 */    49,   49,   49,   49,   54,   54,   54,   54,   54,
};
#define YY_SHIFT_USE_DFLT (-16)
#define YY_SHIFT_COUNT (67)
#define YY_SHIFT_MIN   (-15)
#define YY_SHIFT_MAX   (187)
static const short yy_shift_ofst[] = {
 /*     0 */   -16,  -11,   21,   21,   21,   21,   21,   21,   21,   21,
 /*    10 */    21,   21,   21,   21,   21,   21,   21,   21,   21,   21,
 /*    20 */    21,   21,   21,   21,   21,   21,   21,   21,  187,  -13,
 /*    30 */   -15,  -15,  -16,  -16,   44,   72,   82,   99,  128,  111,
 /*    40 */   144,  144,  144,  144,  144,  144,  144,  144,  144,  144,
 /*    50 */   144,  144,  144,  144,  154,  158,  158,  166,  166,  166,
 /*    60 */    69,   49,   45,   70,  107,   83,  108,   93,
};
#define YY_REDUCE_USE_DFLT (-50)
#define YY_REDUCE_COUNT (33)
#define YY_REDUCE_MIN   (-49)
#define YY_REDUCE_MAX   (184)
static const short yy_reduce_ofst[] = {
 /*     0 */   -29,    9,  -35,   42,  -21,   11,   12,   19,   41,   74,
 /*    10 */    91,  141,  148,  150,  152,  156,  159,  160,  162,  170,
 /*    20 */   171,  172,  173,  180,  181,  182,  183,  184,  -47,  -49,
 /*    30 */   -14,    7,  -37,   15,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   138,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    10 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    20 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    30 */   180,  180,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    40 */   155,  156,  163,  179,  178,  164,  154,  153,  152,  151,
 /*    50 */   150,  149,  148,  147,  162,  157,  158,  159,  160,  161,
 /*    60 */   184,  184,  184,  184,  184,  184,  184,  184,
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
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
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
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "AMPERSAND",     "AND",           "PLUS",        
  "MINUS",         "TIMES",         "DIVIDE",        "POWER",       
  "SQRT",          "WRITE",         "SEMICOLON",     "IDENTIFIER",  
  "ASSIGN",        "TIME",          "LPAR",          "RPAR",        
  "NUMTOKEN",      "STRTOKEN",      "NOT",           "MAXIMUM",     
  "MINIMUM",       "FIRST",         "LAST",          "SUM",         
  "COUNT",         "LT",            "GT",            "OR",          
  "IS",            "WITHIN",        "TO",            "TRUE",        
  "FALSE",         "NUL_",          "TIMESTAMP",     "NOW",         
  "IF",            "THEN",          "ENDIF",         "ELSE",        
  "ELSEIF",        "LSPAR",         "RSPAR",         "COMMA",       
  "OF",            "error",         "code",          "statementblock",
  "statement",     "ex",            "of",            "if_then_else",
  "statements",    "elseif",        "jsonarray",     "exlist",      
  "to",          
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "code ::= statementblock",
 /*   1 */ "statementblock ::=",
 /*   2 */ "statementblock ::= statementblock statement",
 /*   3 */ "statement ::= WRITE ex SEMICOLON",
 /*   4 */ "statement ::= IDENTIFIER ASSIGN ex SEMICOLON",
 /*   5 */ "statement ::= TIME of IDENTIFIER ASSIGN ex SEMICOLON",
 /*   6 */ "ex ::= LPAR ex RPAR",
 /*   7 */ "ex ::= NUMTOKEN",
 /*   8 */ "ex ::= STRTOKEN",
 /*   9 */ "ex ::= IDENTIFIER",
 /*  10 */ "ex ::= NOT ex",
 /*  11 */ "ex ::= MAXIMUM ex",
 /*  12 */ "ex ::= MINIMUM ex",
 /*  13 */ "ex ::= FIRST ex",
 /*  14 */ "ex ::= LAST ex",
 /*  15 */ "ex ::= SUM ex",
 /*  16 */ "ex ::= COUNT ex",
 /*  17 */ "ex ::= TIME of ex",
 /*  18 */ "ex ::= ex LT ex",
 /*  19 */ "ex ::= ex GT ex",
 /*  20 */ "ex ::= ex PLUS ex",
 /*  21 */ "ex ::= ex MINUS ex",
 /*  22 */ "ex ::= ex TIMES ex",
 /*  23 */ "ex ::= ex DIVIDE ex",
 /*  24 */ "ex ::= ex POWER ex",
 /*  25 */ "ex ::= ex AND ex",
 /*  26 */ "ex ::= ex OR ex",
 /*  27 */ "ex ::= ex IS WITHIN ex TO ex",
 /*  28 */ "ex ::= TRUE",
 /*  29 */ "ex ::= FALSE",
 /*  30 */ "ex ::= NUL_",
 /*  31 */ "ex ::= TIMESTAMP",
 /*  32 */ "ex ::= NOW",
 /*  33 */ "statement ::= IF if_then_else",
 /*  34 */ "if_then_else ::= ex THEN statements elseif",
 /*  35 */ "elseif ::= ENDIF SEMICOLON",
 /*  36 */ "elseif ::= ELSE statements ENDIF SEMICOLON",
 /*  37 */ "elseif ::= ELSEIF if_then_else",
 /*  38 */ "ex ::= jsonarray",
 /*  39 */ "jsonarray ::= LSPAR RSPAR",
 /*  40 */ "jsonarray ::= LSPAR exlist RSPAR",
 /*  41 */ "exlist ::= ex",
 /*  42 */ "exlist ::= exlist COMMA ex",
 /*  43 */ "of ::=",
 /*  44 */ "of ::= OF",
 /*  45 */ "to ::=",
 /*  46 */ "to ::= TO",
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
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

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
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    if( yyGrowStack(pParser) ){
      pParser->yystack = &pParser->yystk0;
      pParser->yystksz = 1;
    }
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
  return pParser;
}

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
  ParseARG_FETCH;
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
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
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
            yy_lookahead[j]==YYWILDCARD
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
      }
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
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
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
  assert( i!=YY_REDUCE_USE_DFLT );
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
   ParseARG_FETCH;
   yypParser->yytos--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
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
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 46, 1 },
  { 47, 0 },
  { 47, 2 },
  { 48, 3 },
  { 48, 4 },
  { 48, 6 },
  { 49, 3 },
  { 49, 1 },
  { 49, 1 },
  { 49, 1 },
  { 49, 2 },
  { 49, 2 },
  { 49, 2 },
  { 49, 2 },
  { 49, 2 },
  { 49, 2 },
  { 49, 2 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 3 },
  { 49, 6 },
  { 49, 1 },
  { 49, 1 },
  { 49, 1 },
  { 49, 1 },
  { 49, 1 },
  { 48, 2 },
  { 51, 4 },
  { 53, 2 },
  { 53, 4 },
  { 53, 2 },
  { 49, 1 },
  { 54, 2 },
  { 54, 3 },
  { 55, 1 },
  { 55, 3 },
  { 50, 0 },
  { 50, 1 },
  { 56, 0 },
  { 56, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
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
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
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
      case 0: /* code ::= statementblock */
#line 224 "grammar.y"
{
	printf (cJSON_Print(yymsp[0].minor.yy0)); 
}
#line 924 "grammar.c"
        break;
      case 1: /* statementblock ::= */
#line 236 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg); 
	yymsp[1].minor.yy0 = res;
}
#line 935 "grammar.c"
        break;
      case 2: /* statementblock ::= statementblock statement */
#line 246 "grammar.y"
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( yymsp[-1].minor.yy0, "statements"), yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-1].minor.yy0;
}
#line 943 "grammar.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* statement ::= WRITE ex SEMICOLON */
#line 259 "grammar.y"
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "WRITE"); 
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
	yymsp[-2].minor.yy0 = res; 
}
#line 954 "grammar.c"
        break;
      case 4: /* statement ::= IDENTIFIER ASSIGN ex SEMICOLON */
#line 271 "grammar.y"
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "ASSIGN"); 
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
	yylhsminor.yy0 = res;
}
#line 965 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* statement ::= TIME of IDENTIFIER ASSIGN ex SEMICOLON */
#line 281 "grammar.y"
{
    cJSON *res = cJSON_CreateObject(); 
    cJSON_AddStringToObject(res, "type", "TIMEASSIGNMENT"); 
    cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
    cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
    yymsp[-5].minor.yy0 = res;
}
#line 977 "grammar.c"
        break;
      case 6: /* ex ::= LPAR ex RPAR */
#line 292 "grammar.y"
{ 
	yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0; 
}
#line 984 "grammar.c"
        break;
      case 7: /* ex ::= NUMTOKEN */
#line 298 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 994 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 8: /* ex ::= STRTOKEN */
#line 307 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1005 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* ex ::= IDENTIFIER */
#line 316 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(yymsp[0].minor.yy0)); 
	cJSON_AddStringToObject(res, "line", getLine(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1017 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 10: /* ex ::= NOT ex */
#line 326 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("NOT", yymsp[0].minor.yy0); }
#line 1023 "grammar.c"
        break;
      case 11: /* ex ::= MAXIMUM ex */
#line 329 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("MAXIMUM", yymsp[0].minor.yy0); }
#line 1028 "grammar.c"
        break;
      case 12: /* ex ::= MINIMUM ex */
#line 332 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("MINIMUM", yymsp[0].minor.yy0); }
#line 1033 "grammar.c"
        break;
      case 13: /* ex ::= FIRST ex */
#line 335 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("FIRST", yymsp[0].minor.yy0); }
#line 1038 "grammar.c"
        break;
      case 14: /* ex ::= LAST ex */
#line 338 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("LAST", yymsp[0].minor.yy0); }
#line 1043 "grammar.c"
        break;
      case 15: /* ex ::= SUM ex */
#line 341 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("SUM", yymsp[0].minor.yy0); }
#line 1048 "grammar.c"
        break;
      case 16: /* ex ::= COUNT ex */
#line 344 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("COUNT", yymsp[0].minor.yy0); }
#line 1053 "grammar.c"
        break;
      case 17: /* ex ::= TIME of ex */
#line 348 "grammar.y"
{ 
    cJSON *res = cJSON_CreateObject(); 
    cJSON_AddStringToObject(res, "type", "TIMEOF"); 
    cJSON_AddItemToObject(res, "arg", yymsp[0].minor.yy0);
    yymsp[-2].minor.yy0 = res;
}
#line 1063 "grammar.c"
        break;
      case 18: /* ex ::= ex LT ex */
#line 358 "grammar.y"
{yylhsminor.yy0 = binary ("LT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1068 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* ex ::= ex GT ex */
#line 361 "grammar.y"
{yylhsminor.yy0 = binary ("GT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1074 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* ex ::= ex PLUS ex */
#line 364 "grammar.y"
{yylhsminor.yy0 = binary ("PLUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1080 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 21: /* ex ::= ex MINUS ex */
#line 367 "grammar.y"
{yylhsminor.yy0 = binary ("MINUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1086 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* ex ::= ex TIMES ex */
#line 370 "grammar.y"
{yylhsminor.yy0 = binary ("TIMES", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1092 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* ex ::= ex DIVIDE ex */
#line 373 "grammar.y"
{yylhsminor.yy0 = binary ("DIVIDE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1098 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 24: /* ex ::= ex POWER ex */
#line 376 "grammar.y"
{yylhsminor.yy0 = binary ("POWER", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1104 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* ex ::= ex AND ex */
#line 379 "grammar.y"
{yylhsminor.yy0 = binary ("AND", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1110 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* ex ::= ex OR ex */
#line 382 "grammar.y"
{yylhsminor.yy0 = binary ("OR", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1116 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 27: /* ex ::= ex IS WITHIN ex TO ex */
#line 386 "grammar.y"
{yylhsminor.yy0 = ternary("ISWITHIN", yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0);}
#line 1122 "grammar.c"
  yymsp[-5].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* ex ::= TRUE */
#line 389 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	cJSON_AddStringToObject(res, "value", "True");
	yymsp[0].minor.yy0 = res;
}
#line 1133 "grammar.c"
        break;
      case 29: /* ex ::= FALSE */
#line 397 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	cJSON_AddStringToObject(res, "value", "False");
	yymsp[0].minor.yy0 = res;
}
#line 1143 "grammar.c"
        break;
      case 30: /* ex ::= NUL_ */
#line 405 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	cJSON_AddStringToObject(res, "value", "null");
	yymsp[0].minor.yy0 = res;
}
#line 1153 "grammar.c"
        break;
      case 31: /* ex ::= TIMESTAMP */
#line 413 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIMESTAMP");
	cJSON_AddStringToObject(res, "value", "getValue(yymsp[0].minor.yy0)");
	yylhsminor.yy0 = res;
}
#line 1163 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* ex ::= NOW */
#line 421 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NOW");
	yymsp[0].minor.yy0 = res;
}
#line 1173 "grammar.c"
        break;
      case 33: /* statement ::= IF if_then_else */
      case 37: /* elseif ::= ELSEIF if_then_else */ yytestcase(yyruleno==37);
#line 432 "grammar.y"
{yymsp[-1].minor.yy0 = yymsp[0].minor.yy0;}
#line 1179 "grammar.c"
        break;
      case 34: /* if_then_else ::= ex THEN statements elseif */
#line 436 "grammar.y"
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "typ", "IF");
    cJSON_AddItemToObject(res, "condition", yymsp[-3].minor.yy0);
    cJSON_AddItemToObject(res, "thenbranch", (yymsp[-1].minor.yy0));
    cJSON_AddItemToObject(res, "elsebranch", (yymsp[0].minor.yy0));
    yylhsminor.yy0 = res;
}
#line 1191 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 35: /* elseif ::= ENDIF SEMICOLON */
#line 447 "grammar.y"
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "typ", "NOOP");
    yymsp[-1].minor.yy0 = res;
}
#line 1201 "grammar.c"
        break;
      case 36: /* elseif ::= ELSE statements ENDIF SEMICOLON */
#line 455 "grammar.y"
{yymsp[-3].minor.yy0 = yymsp[-2].minor.yy0;}
#line 1206 "grammar.c"
        break;
      case 38: /* ex ::= jsonarray */
#line 462 "grammar.y"
{yylhsminor.yy0 = yymsp[0].minor.yy0;}
#line 1211 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* jsonarray ::= LSPAR RSPAR */
#line 465 "grammar.y"
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "type", "EMPTYLIST");
    yymsp[-1].minor.yy0 = res;
}
#line 1221 "grammar.c"
        break;
      case 40: /* jsonarray ::= LSPAR exlist RSPAR */
#line 473 "grammar.y"
{
        cJSON *res = cJSON_CreateObject();
        cJSON_AddStringToObject(res, "type", "LIST");
        cJSON_AddItemToObject(res, "args", yymsp[-1].minor.yy0);
        yymsp[-2].minor.yy0 = res;
}
#line 1231 "grammar.c"
        break;
      case 41: /* exlist ::= ex */
#line 482 "grammar.y"
{
    cJSON *arg = cJSON_CreateArray();
    cJSON_AddItemToArray(arg, yymsp[0].minor.yy0);
    yylhsminor.yy0 = arg;
}
#line 1240 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 42: /* exlist ::= exlist COMMA ex */
#line 490 "grammar.y"
{
    cJSON_AddItemToArray(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
    yylhsminor.yy0 = yymsp[-2].minor.yy0;
}
#line 1249 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (43) of ::= */ yytestcase(yyruleno==43);
      /* (44) of ::= OF */ yytestcase(yyruleno==44);
      /* (45) to ::= (OPTIMIZED OUT) */ assert(yyruleno!=45);
      /* (46) to ::= TO (OPTIMIZED OUT) */ assert(yyruleno!=46);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 193 "grammar.y"

  printf ("{\"error\" : true, \"message\": \"Syntax Error: Compiler reports unexpected token \\\"%s\\\" of type \\\"%s\\\" in line %s\"}\n", curtoken, curtype, linenumber);
  exit(0);
#line 1317 "grammar.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
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
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
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
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
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
        while( yypParser->yytos >= &yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
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
      yymajor = YYNOCODE;
      
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
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
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
#line 25 "grammar.y"


using namespace std;
typedef struct {char *value; int line;} token;

token* create_token (char *value, int line) {
	token *t = (token*) malloc (sizeof (token));
	t->value = strdup (value);
	t->line = line;
	return t;
}

const char * getValue (cJSON* token) {
	return cJSON_GetObjectItem (token, "value")->valuestring;
}


const char * getLine (cJSON* token) {
	return cJSON_GetObjectItem (token, "line")->valuestring;
}


int main(int argc, char* argv[]) {
	char *result;
	std::string line;
	std::string input = "";
	while (std::getline(std::cin, line)) {
      input += line + "\n";
    }
	if (input == "") {
		cout << "Empty input";
		exit(0);
	}
	
	cJSON *root = cJSON_Parse(input.c_str());
	
	if (!root) {
		cout << "JSON invalid\n";
		exit(0);
	}
	
	void* pParser = ParseAlloc (malloc);
	int num = cJSON_GetArraySize (root);
	
	for (int i = 0; i < num; i++ ) {
	
		// Knoten im Token-Stream auslesen
		cJSON *node = cJSON_GetArrayItem(root,i);
		
		char *line = cJSON_GetArrayItem(node,0)->valuestring;
		char *type = cJSON_GetArrayItem(node,1)->valuestring;
		char *value = cJSON_GetArrayItem(node,2)->valuestring;
		
		cJSON *tok = cJSON_CreateObject();
		cJSON_AddStringToObject(tok, "value", value);
		cJSON_AddStringToObject(tok, "line", line);

		linenumber = line;
		curtoken = value;
		curtype = type;
		// THE und Kommentare werden ueberlesen
		if (strcmp(type, "THE") == 0) continue;
		if (strcmp(type, "COMMENT") == 0) continue;
		if (strcmp(type, "MCOMMENT") == 0) continue;
		
		int tokenid = get_token_id (type);
		Parse (pParser, tokenid, tok);
		
	}
	Parse (pParser, 0, 0);
    ParseFree(pParser, free );
}




/////////////////////// 
/////////////////////// 
// TOKENS
///////////////////////
/////////////////////// 

int get_token_id (char *token) {
	if (strcmp(token, "DIVIDE") == 0) return DIVIDE;
	if (strcmp(token, "TIME") == 0) return TIME;
	if (strcmp(token, "OF") == 0) return OF;
	if (strcmp(token, "IS") == 0) return IS;
	if (strcmp(token, "WITHIN") == 0) return WITHIN;
	if (strcmp(token, "IDENTIFIER") == 0) return IDENTIFIER;
	if (strcmp(token, "COMMA") == 0) return COMMA;
	if (strcmp(token, "LPAR") == 0) return LPAR; 
	if (strcmp(token, "RPAR") == 0) return RPAR;
	if (strcmp(token, "LSPAR") == 0) return LSPAR;
	if (strcmp(token, "RSPAR") == 0) return RSPAR;
	if (strcmp(token, "MINUS") == 0) return MINUS;
	if (strcmp(token, "NUMTOKEN") == 0) return NUMTOKEN;
	if (strcmp(token, "PLUS") == 0) return PLUS;
	if (strcmp(token, "POWER") == 0) return POWER;
	if (strcmp(token, "SEMICOLON") == 0) return SEMICOLON;
	if (strcmp(token, "STRTOKEN") == 0) return STRTOKEN;
	if (strcmp(token, "WRITE") == 0) return WRITE;
	if (strcmp(token, "TIMES") == 0) return TIMES;
	if (strcmp(token, "ASSIGN") == 0) return ASSIGN;
	if (strcmp(token, "NULL") == 0) return NUL_;
	if (strcmp(token, "TRUE") == 0) return TRUE;
	if (strcmp(token, "FALSE") == 0) return FALSE;
	if (strcmp(token, "AND") == 0) return AND;
	if (strcmp(token, "OR") == 0) return OR;
	if (strcmp(token, "NOT") == 0) return NOT;
	if (strcmp(token, "MAXIMUM") == 0) return MAXIMUM;
	if (strcmp(token, "MINIMUM") == 0) return MINIMUM;
	if (strcmp(token, "FIRST") == 0) return FIRST;
	if (strcmp(token, "LAST") == 0) return LAST;
	if (strcmp(token, "SUM") == 0) return SUM;
	if (strcmp(token, "COUNT") == 0) return COUNT;
	if (strcmp(token, "LT") == 0) return LT;
	if (strcmp(token, "GT") == 0) return GT;
	if (strcmp(token, "NOW") == 0) return NOW;
	if (strcmp(token, "TO") == 0) return TO;
	

	printf ("{\"error\" : true, \"message\": \"UNKNOWN TOKEN TYPE %s\"}\n", token);
	exit(0);
} 
  


cJSON* unary (char* fname, cJSON* a) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a);
	cJSON_AddStringToObject(res, "type", fname);
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
} 



cJSON* binary (char *fname, cJSON *a, cJSON *b) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a); 
	cJSON_AddItemToArray(arg, b);
	cJSON_AddStringToObject(res, "type", fname); 
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
}



cJSON* ternary (char *fname, cJSON *a, cJSON *b, cJSON *c) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a); 
	cJSON_AddItemToArray(arg, b); 
	cJSON_AddItemToArray(arg, c);
	cJSON_AddStringToObject(res, "type", fname); 
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
}



#line 1687 "grammar.c"
