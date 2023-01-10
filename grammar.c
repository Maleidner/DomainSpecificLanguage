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
#define YYNOCODE 69
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
#define YYNSTATE             85
#define YYNRULE              56
#define YY_MAX_SHIFT         84
#define YY_MIN_SHIFTREDUCE   111
#define YY_MAX_SHIFTREDUCE   166
#define YY_MIN_REDUCE        167
#define YY_MAX_REDUCE        222
#define YY_ERROR_ACTION      223
#define YY_ACCEPT_ACTION     224
#define YY_NO_ACTION         225
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
#define YY_ACTTAB_COUNT (355)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    27,   12,   29,   20,   18,   16,   14,   13,  224,   35,
 /*    10 */   126,   74,   40,  113,   30,   28,   26,   24,   22,   10,
 /*    20 */   160,    8,   31,   77,  124,  125,   25,   23,   21,   19,
 /*    30 */    17,   15,   43,   38,   74,   13,  158,   30,   28,   26,
 /*    40 */    24,   22,   10,  152,  153,  154,  155,  156,  157,    1,
 /*    50 */   159,   27,   42,   29,  122,   33,  158,   84,  119,   39,
 /*    60 */    81,  126,   74,   40,    3,   30,   28,   26,   24,   22,
 /*    70 */    10,  164,   83,   31,    9,  124,  125,   25,   23,   21,
 /*    80 */    19,   17,   15,   11,   12,  121,   20,   18,   16,   14,
 /*    90 */    13,   78,   36,    2,  152,  153,  154,  155,  156,  157,
 /*   100 */     1,   11,   12,   37,   20,   18,   16,   14,   13,   56,
 /*   110 */    34,  135,  136,  158,   75,  120,   76,   74,   41,   32,
 /*   120 */    30,   28,   26,   24,   22,   10,   11,   12,  117,   20,
 /*   130 */    18,   16,   14,   13,    4,   74,   80,  116,   30,   28,
 /*   140 */    26,   24,   22,   10,   11,   12,    5,   20,   18,   16,
 /*   150 */    14,   13,   42,   44,  118,  115,  158,  158,   45,   82,
 /*   160 */    74,    6,  158,   30,   28,   26,   24,   22,   10,   11,
 /*   170 */    12,  169,   20,   18,   16,   14,   13,  169,   74,  169,
 /*   180 */   169,   30,   28,   26,   24,   22,   10,   11,   12,  169,
 /*   190 */    20,   18,   16,   14,   13,   49,   33,  169,   84,  158,
 /*   200 */    39,   81,  169,   74,   79,    3,   30,   28,   26,   24,
 /*   210 */    22,   10,  123,    7,  169,  169,   20,   18,   16,   14,
 /*   220 */    13,   74,  169,  169,   30,   28,   26,   24,   22,   10,
 /*   230 */    11,   12,  169,   20,   18,   16,   14,   13,  169,   11,
 /*   240 */    12,  114,   20,   18,   16,   14,   13,   74,  169,  169,
 /*   250 */    30,   28,   26,   24,   22,   10,  169,   55,   16,   14,
 /*   260 */    13,  158,   57,  169,   74,  169,  158,   30,   28,   26,
 /*   270 */    24,   22,   10,   74,  169,  169,   30,   28,   26,   24,
 /*   280 */    22,   10,   58,  169,  169,  169,  158,   74,  167,  169,
 /*   290 */    30,   28,   26,   24,   22,   10,  169,   65,  169,   66,
 /*   300 */    33,  158,   84,  158,   39,   81,   71,   72,   59,    3,
 /*   310 */   158,  158,  158,   70,   60,   69,   61,  158,  158,  158,
 /*   320 */   158,   68,   62,   54,   63,  158,  158,  158,  158,   53,
 /*   330 */    64,   52,   67,  158,  158,  158,  158,  169,   51,   73,
 /*   340 */    50,   47,  158,  158,  158,  158,  169,  169,   46,  169,
 /*   350 */    48,  169,  158,  169,  158,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     4,    3,    6,    5,    6,    7,    8,    9,   58,   59,
 /*    10 */    14,   36,   16,   60,   39,   40,   41,   42,   43,   44,
 /*    20 */    54,   55,   26,   60,   28,   29,   30,   31,   32,   33,
 /*    30 */    34,   35,   61,   60,   36,    9,   65,   39,   40,   41,
 /*    40 */    42,   43,   44,   47,   48,   49,   50,   51,   52,   53,
 /*    50 */    54,    4,   61,    6,   63,   12,   65,   14,   64,   16,
 /*    60 */    17,   14,   36,   16,   21,   39,   40,   41,   42,   43,
 /*    70 */    44,   56,   62,   26,   62,   28,   29,   30,   31,   32,
 /*    80 */    33,   34,   35,    2,    3,   13,    5,    6,    7,    8,
 /*    90 */     9,   23,   24,   25,   47,   48,   49,   50,   51,   52,
 /*   100 */    53,    2,    3,   22,    5,    6,    7,    8,    9,   61,
 /*   110 */    59,   37,   38,   65,   66,   13,   23,   36,   19,   45,
 /*   120 */    39,   40,   41,   42,   43,   44,    2,    3,   13,    5,
 /*   130 */     6,    7,    8,    9,   18,   36,   14,   13,   39,   40,
 /*   140 */    41,   42,   43,   44,    2,    3,   15,    5,    6,    7,
 /*   150 */     8,    9,   61,   61,   63,   13,   65,   65,   61,   14,
 /*   160 */    36,   15,   65,   39,   40,   41,   42,   43,   44,    2,
 /*   170 */     3,   68,    5,    6,    7,    8,    9,   68,   36,   68,
 /*   180 */    68,   39,   40,   41,   42,   43,   44,    2,    3,   68,
 /*   190 */     5,    6,    7,    8,    9,   61,   12,   68,   14,   65,
 /*   200 */    16,   17,   68,   36,   20,   21,   39,   40,   41,   42,
 /*   210 */    43,   44,   27,   46,   68,   68,    5,    6,    7,    8,
 /*   220 */     9,   36,   68,   68,   39,   40,   41,   42,   43,   44,
 /*   230 */     2,    3,   68,    5,    6,    7,    8,    9,   68,    2,
 /*   240 */     3,   13,    5,    6,    7,    8,    9,   36,   68,   68,
 /*   250 */    39,   40,   41,   42,   43,   44,   68,   61,    7,    8,
 /*   260 */     9,   65,   61,   68,   36,   68,   65,   39,   40,   41,
 /*   270 */    42,   43,   44,   36,   68,   68,   39,   40,   41,   42,
 /*   280 */    43,   44,   61,   68,   68,   68,   65,   36,    0,   68,
 /*   290 */    39,   40,   41,   42,   43,   44,   68,   61,   68,   61,
 /*   300 */    12,   65,   14,   65,   16,   17,   61,   61,   61,   21,
 /*   310 */    65,   65,   65,   61,   61,   61,   61,   65,   65,   65,
 /*   320 */    65,   61,   61,   61,   61,   65,   65,   65,   65,   61,
 /*   330 */    61,   61,   61,   65,   65,   65,   65,   68,   61,   61,
 /*   340 */    61,   61,   65,   65,   65,   65,   68,   68,   61,   68,
 /*   350 */    61,   68,   65,   68,   65,
};
#define YY_SHIFT_USE_DFLT (-35)
#define YY_SHIFT_COUNT (84)
#define YY_SHIFT_MIN   (-34)
#define YY_SHIFT_MAX   (288)
static const short yy_shift_ofst[] = {
 /*     0 */   -35,   -4,   47,   47,   47,   47,   47,   47,   47,   47,
 /*    10 */    47,   47,   47,   47,   47,   47,   47,   47,   47,   47,
 /*    20 */    47,   47,   47,   47,   47,   47,   47,   47,   47,   47,
 /*    30 */    47,   47,   47,   47,  184,  288,   43,   43,   68,   15,
 /*    40 */    15,  -35,   81,   99,  124,  142,  167,  185,  228,  237,
 /*    50 */   237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
 /*    60 */   237,  237,  237,  237,  237,   -2,  211,  211,  251,  251,
 /*    70 */    26,   26,   26,  -25,   74,  -34,   72,   93,  102,  115,
 /*    80 */   116,  122,  131,  145,  146,
};
#define YY_REDUCE_USE_DFLT (-51)
#define YY_REDUCE_COUNT (41)
#define YY_REDUCE_MIN   (-50)
#define YY_REDUCE_MAX   (289)
static const short yy_reduce_ofst[] = {
 /*     0 */   -50,   48,   -9,   91,  -29,   92,   97,  134,  196,  201,
 /*    10 */   221,  236,  238,  245,  246,  247,  252,  253,  254,  255,
 /*    20 */   260,  261,  262,  263,  268,  269,  270,  271,  277,  278,
 /*    30 */   279,  280,  287,  289,  -47,  -47,  -37,  -27,   -6,   10,
 /*    40 */    12,   51,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   168,  223,  223,  223,  223,  223,  223,  223,  223,  223,
 /*    10 */   223,  223,  223,  223,  223,  223,  223,  223,  223,  223,
 /*    20 */   223,  223,  223,  223,  223,  223,  223,  223,  223,  223,
 /*    30 */   223,  223,  223,  223,  223,  223,  223,  223,  223,  219,
 /*    40 */   219,  168,  223,  223,  223,  223,  223,  223,  223,  207,
 /*    50 */   194,  195,  196,  197,  198,  218,  217,  193,  206,  190,
 /*    60 */   189,  188,  187,  186,  185,  205,  204,  184,  199,  200,
 /*    70 */   201,  203,  202,  183,  223,  223,  223,  223,  223,  223,
 /*    80 */   223,  223,  223,  223,  223,
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
  "$",             "AMPERSAND",     "OR",            "AND",         
  "NOT",           "PLUS",          "MINUS",         "TIMES",       
  "DIVIDE",        "POWER",         "SQRT",          "UMINUS",      
  "WRITE",         "SEMICOLON",     "IDENTIFIER",    "ASSIGN",      
  "TIME",          "FOR",           "IN",            "DO",          
  "ENDDO",         "IF",            "THEN",          "ENDIF",       
  "ELSE",          "ELSEIF",        "LPAR",          "RPAR",        
  "NUMTOKEN",      "STRTOKEN",      "MAXIMUM",       "MINIMUM",     
  "FIRST",         "LAST",          "SUM",           "COUNT",       
  "IS",            "NUMBER",        "LIST",          "EQUAL",       
  "LT",            "LTOE",          "GT",            "GTOE",        
  "SEQTO",         "WITHIN",        "TO",            "TRUE",        
  "FALSE",         "NUL_",          "TIMESTAMP",     "NOW",         
  "CURRENTTIME",   "LSPAR",         "RSPAR",         "COMMA",       
  "OF",            "error",         "code",          "statementblock",
  "statement",     "ex",            "of",            "if_then_else",
  "elseif",        "jsonarray",     "exlist",        "to",          
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
 /*   6 */ "statement ::= FOR IDENTIFIER IN ex DO statementblock ENDDO SEMICOLON",
 /*   7 */ "statement ::= IF if_then_else",
 /*   8 */ "if_then_else ::= ex THEN statement elseif",
 /*   9 */ "elseif ::= ENDIF SEMICOLON",
 /*  10 */ "elseif ::= ELSE statement ENDIF SEMICOLON",
 /*  11 */ "elseif ::= ELSEIF if_then_else",
 /*  12 */ "ex ::= LPAR ex RPAR",
 /*  13 */ "ex ::= NUMTOKEN",
 /*  14 */ "ex ::= STRTOKEN",
 /*  15 */ "ex ::= IDENTIFIER",
 /*  16 */ "ex ::= MINUS ex",
 /*  17 */ "ex ::= NOT ex",
 /*  18 */ "ex ::= MAXIMUM ex",
 /*  19 */ "ex ::= MINIMUM ex",
 /*  20 */ "ex ::= FIRST ex",
 /*  21 */ "ex ::= LAST ex",
 /*  22 */ "ex ::= SUM ex",
 /*  23 */ "ex ::= COUNT ex",
 /*  24 */ "ex ::= ex IS NUMBER",
 /*  25 */ "ex ::= ex IS LIST",
 /*  26 */ "ex ::= TIME of ex",
 /*  27 */ "ex ::= ex EQUAL ex",
 /*  28 */ "ex ::= ex LT ex",
 /*  29 */ "ex ::= ex LTOE ex",
 /*  30 */ "ex ::= ex GT ex",
 /*  31 */ "ex ::= ex GTOE ex",
 /*  32 */ "ex ::= ex PLUS ex",
 /*  33 */ "ex ::= ex MINUS ex",
 /*  34 */ "ex ::= ex TIMES ex",
 /*  35 */ "ex ::= ex DIVIDE ex",
 /*  36 */ "ex ::= ex POWER ex",
 /*  37 */ "ex ::= ex AND ex",
 /*  38 */ "ex ::= ex OR ex",
 /*  39 */ "ex ::= ex SEQTO ex",
 /*  40 */ "ex ::= ex IS WITHIN ex TO ex",
 /*  41 */ "ex ::= TRUE",
 /*  42 */ "ex ::= FALSE",
 /*  43 */ "ex ::= NUL_",
 /*  44 */ "ex ::= TIMESTAMP",
 /*  45 */ "ex ::= NOW",
 /*  46 */ "ex ::= CURRENTTIME",
 /*  47 */ "ex ::= jsonarray",
 /*  48 */ "jsonarray ::= LSPAR RSPAR",
 /*  49 */ "jsonarray ::= LSPAR exlist RSPAR",
 /*  50 */ "exlist ::= ex",
 /*  51 */ "exlist ::= exlist COMMA ex",
 /*  52 */ "of ::=",
 /*  53 */ "of ::= OF",
 /*  54 */ "to ::=",
 /*  55 */ "to ::= TO",
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
  { 58, 1 },
  { 59, 0 },
  { 59, 2 },
  { 60, 3 },
  { 60, 4 },
  { 60, 6 },
  { 60, 8 },
  { 60, 2 },
  { 63, 4 },
  { 64, 2 },
  { 64, 4 },
  { 64, 2 },
  { 61, 3 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 2 },
  { 61, 2 },
  { 61, 2 },
  { 61, 2 },
  { 61, 2 },
  { 61, 2 },
  { 61, 2 },
  { 61, 2 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 6 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 65, 2 },
  { 65, 3 },
  { 66, 1 },
  { 66, 3 },
  { 62, 0 },
  { 62, 1 },
  { 67, 0 },
  { 67, 1 },
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
#line 245 "grammar.y"
{
	printf (cJSON_Print(yymsp[0].minor.yy0)); 
}
#line 973 "grammar.c"
        break;
      case 1: /* statementblock ::= */
#line 257 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg); 
	yymsp[1].minor.yy0 = res;
}
#line 984 "grammar.c"
        break;
      case 2: /* statementblock ::= statementblock statement */
#line 267 "grammar.y"
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( yymsp[-1].minor.yy0, "statements"), yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-1].minor.yy0;
}
#line 992 "grammar.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* statement ::= WRITE ex SEMICOLON */
#line 280 "grammar.y"
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "WRITE"); 
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
	yymsp[-2].minor.yy0 = res; 
}
#line 1003 "grammar.c"
        break;
      case 4: /* statement ::= IDENTIFIER ASSIGN ex SEMICOLON */
#line 292 "grammar.y"
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "ASSIGN"); 
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
	yylhsminor.yy0 = res;
}
#line 1014 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* statement ::= TIME of IDENTIFIER ASSIGN ex SEMICOLON */
#line 302 "grammar.y"
{
    cJSON *res = cJSON_CreateObject(); 
    cJSON_AddStringToObject(res, "type", "TIMEASSIGNMENT"); 
    cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
    cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
    yymsp[-5].minor.yy0 = res;
}
#line 1026 "grammar.c"
        break;
      case 6: /* statement ::= FOR IDENTIFIER IN ex DO statementblock ENDDO SEMICOLON */
#line 316 "grammar.y"
{
cJSON *res = cJSON_CreateObject(); 
cJSON_AddStringToObject(res, "type", "FOR");
cJSON_AddStringToObject(res, "varname", getValue(yymsp[-6].minor.yy0));
cJSON_AddItemToObject(res, "expression", yymsp[-4].minor.yy0);
cJSON_AddItemToObject(res, "statements", yymsp[-2].minor.yy0);
yymsp[-7].minor.yy0 = res;
}
#line 1038 "grammar.c"
        break;
      case 7: /* statement ::= IF if_then_else */
      case 11: /* elseif ::= ELSEIF if_then_else */ yytestcase(yyruleno==11);
#line 330 "grammar.y"
{yymsp[-1].minor.yy0 = yymsp[0].minor.yy0;}
#line 1044 "grammar.c"
        break;
      case 8: /* if_then_else ::= ex THEN statement elseif */
#line 333 "grammar.y"
{
cJSON *res = cJSON_CreateObject();
cJSON_AddStringToObject(res, "type", "IF");
cJSON_AddItemToObject(res, "condition", yymsp[-3].minor.yy0);
cJSON_AddItemToObject(res, "thenbranch", (yymsp[-1].minor.yy0));
cJSON_AddItemToObject(res, "elsebranch", (yymsp[0].minor.yy0));
yylhsminor.yy0 = res;
}
#line 1056 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* elseif ::= ENDIF SEMICOLON */
#line 343 "grammar.y"
{
cJSON *res = cJSON_CreateObject();
cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
cJSON *arg = cJSON_CreateArray();
cJSON_AddItemToObject(res, "statements", arg);
yymsp[-1].minor.yy0 = res;
}
#line 1068 "grammar.c"
        break;
      case 10: /* elseif ::= ELSE statement ENDIF SEMICOLON */
#line 352 "grammar.y"
{yymsp[-3].minor.yy0 = yymsp[-2].minor.yy0;}
#line 1073 "grammar.c"
        break;
      case 12: /* ex ::= LPAR ex RPAR */
#line 362 "grammar.y"
{ 
	yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0; 
}
#line 1080 "grammar.c"
        break;
      case 13: /* ex ::= NUMTOKEN */
#line 368 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1090 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* ex ::= STRTOKEN */
#line 377 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1101 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* ex ::= IDENTIFIER */
#line 386 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(yymsp[0].minor.yy0)); 
	cJSON_AddStringToObject(res, "line", getLine(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1113 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 16: /* ex ::= MINUS ex */
#line 396 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("UMINUS", yymsp[0].minor.yy0); }
#line 1119 "grammar.c"
        break;
      case 17: /* ex ::= NOT ex */
#line 399 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("NOT", yymsp[0].minor.yy0); }
#line 1124 "grammar.c"
        break;
      case 18: /* ex ::= MAXIMUM ex */
#line 402 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("MAXIMUM", yymsp[0].minor.yy0); }
#line 1129 "grammar.c"
        break;
      case 19: /* ex ::= MINIMUM ex */
#line 405 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("MINIMUM", yymsp[0].minor.yy0); }
#line 1134 "grammar.c"
        break;
      case 20: /* ex ::= FIRST ex */
#line 408 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("FIRST", yymsp[0].minor.yy0); }
#line 1139 "grammar.c"
        break;
      case 21: /* ex ::= LAST ex */
#line 411 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("LAST", yymsp[0].minor.yy0); }
#line 1144 "grammar.c"
        break;
      case 22: /* ex ::= SUM ex */
#line 414 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("SUM", yymsp[0].minor.yy0); }
#line 1149 "grammar.c"
        break;
      case 23: /* ex ::= COUNT ex */
#line 417 "grammar.y"
{yymsp[-1].minor.yy0 = unary ("COUNT", yymsp[0].minor.yy0); }
#line 1154 "grammar.c"
        break;
      case 24: /* ex ::= ex IS NUMBER */
#line 420 "grammar.y"
{yylhsminor.yy0 = unary ("ISNUMBER", yymsp[-2].minor.yy0); }
#line 1159 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* ex ::= ex IS LIST */
#line 423 "grammar.y"
{yylhsminor.yy0 = unary ("ISLIST", yymsp[-2].minor.yy0); }
#line 1165 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* ex ::= TIME of ex */
#line 428 "grammar.y"
{ 
    cJSON *res = cJSON_CreateObject(); 
    cJSON_AddStringToObject(res, "type", "TIMEOF"); 
    cJSON_AddItemToObject(res, "arg", yymsp[0].minor.yy0);
    yymsp[-2].minor.yy0 = res;
}
#line 1176 "grammar.c"
        break;
      case 27: /* ex ::= ex EQUAL ex */
#line 441 "grammar.y"
{yylhsminor.yy0 = binary ("EQUAL", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1181 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* ex ::= ex LT ex */
#line 444 "grammar.y"
{yylhsminor.yy0 = binary ("LT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1187 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* ex ::= ex LTOE ex */
#line 447 "grammar.y"
{yylhsminor.yy0 = binary ("LTOE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1193 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* ex ::= ex GT ex */
#line 450 "grammar.y"
{yylhsminor.yy0 = binary ("GT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1199 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 31: /* ex ::= ex GTOE ex */
#line 453 "grammar.y"
{yylhsminor.yy0 = binary ("GTOE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1205 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* ex ::= ex PLUS ex */
#line 456 "grammar.y"
{yylhsminor.yy0 = binary ("PLUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1211 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* ex ::= ex MINUS ex */
#line 459 "grammar.y"
{yylhsminor.yy0 = binary ("MINUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1217 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 34: /* ex ::= ex TIMES ex */
#line 462 "grammar.y"
{yylhsminor.yy0 = binary ("TIMES", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1223 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 35: /* ex ::= ex DIVIDE ex */
#line 465 "grammar.y"
{yylhsminor.yy0 = binary ("DIVIDE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1229 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 36: /* ex ::= ex POWER ex */
#line 468 "grammar.y"
{yylhsminor.yy0 = binary ("POWER", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1235 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* ex ::= ex AND ex */
#line 471 "grammar.y"
{yylhsminor.yy0 = binary ("AND", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1241 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* ex ::= ex OR ex */
#line 474 "grammar.y"
{yylhsminor.yy0 = binary ("OR", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1247 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* ex ::= ex SEQTO ex */
#line 477 "grammar.y"
{yylhsminor.yy0 = binary ("SEQTO", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1253 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* ex ::= ex IS WITHIN ex TO ex */
#line 481 "grammar.y"
{yylhsminor.yy0 = ternary("ISWITHIN", yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0);}
#line 1259 "grammar.c"
  yymsp[-5].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* ex ::= TRUE */
#line 484 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	cJSON_AddStringToObject(res, "value", "True");
	yymsp[0].minor.yy0 = res;
}
#line 1270 "grammar.c"
        break;
      case 42: /* ex ::= FALSE */
#line 492 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	cJSON_AddStringToObject(res, "value", "False");
	yymsp[0].minor.yy0 = res;
}
#line 1280 "grammar.c"
        break;
      case 43: /* ex ::= NUL_ */
#line 500 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	cJSON_AddStringToObject(res, "value", "null");
	yymsp[0].minor.yy0 = res;
}
#line 1290 "grammar.c"
        break;
      case 44: /* ex ::= TIMESTAMP */
#line 508 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIMESTAMP");
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0));
	yylhsminor.yy0 = res;
}
#line 1300 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* ex ::= NOW */
#line 516 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NOW");
	yymsp[0].minor.yy0 = res;
}
#line 1310 "grammar.c"
        break;
      case 46: /* ex ::= CURRENTTIME */
#line 523 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "CURRENTTIME");
	yymsp[0].minor.yy0 = res;
}
#line 1319 "grammar.c"
        break;
      case 47: /* ex ::= jsonarray */
#line 530 "grammar.y"
{yylhsminor.yy0 = yymsp[0].minor.yy0;}
#line 1324 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 48: /* jsonarray ::= LSPAR RSPAR */
#line 534 "grammar.y"
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "type", "EMPTYLIST");
    yymsp[-1].minor.yy0 = res;
}
#line 1334 "grammar.c"
        break;
      case 49: /* jsonarray ::= LSPAR exlist RSPAR */
#line 542 "grammar.y"
{
        cJSON *res = cJSON_CreateObject();
        cJSON_AddStringToObject(res, "type", "LIST");
        cJSON_AddItemToObject(res, "args", yymsp[-1].minor.yy0);
        yymsp[-2].minor.yy0 = res;
}
#line 1344 "grammar.c"
        break;
      case 50: /* exlist ::= ex */
#line 551 "grammar.y"
{
    cJSON *arg = cJSON_CreateArray();
    cJSON_AddItemToArray(arg, yymsp[0].minor.yy0);
    yylhsminor.yy0 = arg;
}
#line 1353 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* exlist ::= exlist COMMA ex */
#line 559 "grammar.y"
{
    cJSON_AddItemToArray(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
    yylhsminor.yy0 = yymsp[-2].minor.yy0;
}
#line 1362 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (52) of ::= */ yytestcase(yyruleno==52);
      /* (53) of ::= OF */ yytestcase(yyruleno==53);
      /* (54) to ::= (OPTIMIZED OUT) */ assert(yyruleno!=54);
      /* (55) to ::= TO (OPTIMIZED OUT) */ assert(yyruleno!=55);
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
#line 212 "grammar.y"

  printf ("{\"error\" : true, \"message\": \"Syntax Error: Compiler reports unexpected token \\\"%s\\\" of type \\\"%s\\\" in line %s\"}\n", curtoken, curtype, linenumber);
  exit(0);
#line 1430 "grammar.c"
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
	if (strcmp(token, "UMINUS") == 0) return UMINUS;
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
	if (strcmp(token, "EQUAL") == 0) return EQUAL;
	if (strcmp(token, "LT") == 0) return LT;
	if (strcmp(token, "LTOE") == 0) return LTOE;
	if (strcmp(token, "GT") == 0) return GT;
	if (strcmp(token, "GTOE") == 0) return GTOE;
	if (strcmp(token, "NOW") == 0) return NOW;
	if (strcmp(token, "CURRENTTIME") == 0) return CURRENTTIME;
	if (strcmp(token, "TO") == 0) return TO;
	if (strcmp(token, "FOR") == 0) return FOR;
	if (strcmp(token, "IN") == 0) return IN;
	if (strcmp(token, "DO") == 0) return DO;
	if (strcmp(token, "ENDDO") == 0) return ENDDO;
	if (strcmp(token, "IS") == 0) return IS;
	if (strcmp(token, "NUMBER") == 0) return NUMBER;
	if (strcmp(token, "LIST") == 0) return LIST;
	if (strcmp(token, "IF") == 0) return IF;
	if (strcmp(token, "THEN") == 0) return THEN;
	if (strcmp(token, "ELSE") == 0) return ELSE;
	if (strcmp(token, "ELSEIF") == 0) return ELSEIF;
	if (strcmp(token, "ENDIF") == 0) return ENDIF;
	if (strcmp(token, "SEQTO") == 0) return SEQTO;
	if (strcmp(token, "TIMESTAMP") == 0) return TIMESTAMP;
	

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



#line 1819 "grammar.c"
