%include { 

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
}  
   
   
%code {

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
	if (strcmp(token, "LT") == 0) return LT;
	if (strcmp(token, "LTOE") == 0) return LTOE;
	if (strcmp(token, "GT") == 0) return GT;
	if (strcmp(token, "GTOE") == 0) return GTOE;
	if (strcmp(token, "EQUAL") == 0) return EQUAL;
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



}
   
%syntax_error {
  printf ("{\"error\" : true, \"message\": \"Syntax Error: Compiler reports unexpected token \\\"%s\\\" of type \\\"%s\\\" in line %s\"}\n", curtoken, curtype, linenumber);
  exit(0);
} 
  
%token_type {cJSON *}
%default_type {cJSON *} 

/////////////////////// 
/////////////////////// 
// PRECEDENCE
///////////////////////
/////////////////////// 

%left	   AMPERSAND .
%left	   AND .
%left 	   PLUS MINUS .
%left 	   TIMES DIVIDE .
%right     POWER .
%left      SQRT .




/////////////////////// 
// CODE
///////////////////////


 
code ::= statementblock(sb) . 
{
	printf (cJSON_Print(sb)); 
}  



/////////////////////// 
// STATEMENTBLOCK
///////////////////////


statementblock(sb) ::= .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg); 
	sb = res;
}


statementblock(sb) ::= statementblock(a) statement(b) .
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( a, "statements"), b);
	sb = a;
}




///////////////////////////
// WRITE
///////////////////////////

statement(r) ::= WRITE ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "WRITE"); 
	cJSON_AddItemToObject(res, "arg", e); 
	r = res; 
}

///////////////////////////
// ASSIGN
///////////////////////////

statement(r) ::= IDENTIFIER(a) ASSIGN ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "ASSIGN"); 
	cJSON_AddStringToObject(res, "varname", getValue(a));
	cJSON_AddItemToObject(res, "arg", e); 
	r = res;
}

ex(r) ::= LPAR ex(a) RPAR .    
{ 
	r = a; 
}


ex(r) ::= NUMTOKEN (a).        
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(a)); 
	r = res; 
} 


ex(r) ::= STRTOKEN (a).        
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(a)); 
	r = res; 
}


ex(r) ::= IDENTIFIER(a) .      
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(a)); 
	cJSON_AddStringToObject(res, "line", getLine(a)); 
	r = res; 
}


ex(r) ::= NOT ex(a) .                                
{r = unary ("NOT", a); }

ex(r) ::= MAXIMUM ex(a) .                                
{r = unary ("MAXIMUM", a); }

ex(r) ::= MINIMUM ex(a) .                                
{r = unary ("MINIMUM", a); }

ex(r) ::= FIRST ex(a) .                                
{r = unary ("FIRST", a); }

ex(r) ::= LAST ex(a) .                                
{r = unary ("LAST", a); }

ex(r) ::= SUM ex(a) .                                
{r = unary ("SUM", a); }

ex(r) ::= COUNT ex(a) .                                
{r = unary ("COUNT", a); }

ex(r) ::= ex(a) LT ex(b) .                                
{r = binary ("LT", a, b); }

ex(r) ::= ex(a) GT ex(b) .                                
{r = binary ("GT", a, b); }

ex(r) ::= ex(a) PLUS ex(b) .                                
{r = binary ("PLUS", a, b); }

ex(r) ::= ex(a) MINUS ex(b) .                               
{r = binary ("MINUS", a, b); }

ex(r) ::= ex(a) TIMES ex(b) .                               
{r = binary ("TIMES", a, b); }

ex(r) ::= ex(a) DIVIDE ex(b) .                              
{r = binary ("DIVIDE", a, b); }

ex(r) ::= ex(a) POWER ex(b) .                               
{r = binary ("POWER", a, b); }

ex(r) ::= ex(a) AND ex(b) .                               
{r = binary ("AND", a, b); }

ex(r) ::= ex(a) OR ex(b) .                               
{r = binary ("OR", a, b); }




ex(r) ::= TRUE .                               
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	cJSON_AddStringToObject(res, "value", "True");
	r = res;
}

ex(r) ::= FALSE .                               
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	cJSON_AddStringToObject(res, "value", "False");
	r = res;
}

ex(r) ::= NUL_ .                               
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	cJSON_AddStringToObject(res, "value", "null");
	r = res;
}



///////////////////////////
// IF
///////////////////////////

statement(r) ::= IF if_then_else(a) .
{r = a;}


if_then_else(r) ::= ex(a) THEN statements(b) elseif(c) . 
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "typ", "IF");
    cJSON_AddItemToObject(res, "condition", a);
    cJSON_AddItemToObject(res, "thenbranch", (b));
    cJSON_AddItemToObject(res, "elsebranch", (c));
    r = res;
}


elseif(r) ::= ENDIF SEMICOLON .
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "typ", "NOOP");
    r = res;
}


elseif(r) ::= ELSE statements(a) ENDIF SEMICOLON.
{r = a;}

elseif(r) ::= ELSEIF if_then_else(a) .
{r = a;}


ex(r) ::= jsonarray (a) .
{r = a;}
// Leere Liste
jsonarray(r) ::= LSPAR RSPAR .
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "type", "EMPTYLIST");
    r = res;
}

// Nicht-leere Liste: Liste von Expressions, kommasepariert
jsonarray(r) ::= LSPAR exlist(a) RSPAR . 
{
        cJSON *res = cJSON_CreateObject();
        cJSON_AddStringToObject(res, "type", "LIST");
        cJSON_AddItemToObject(res, "args", a);
        r = res;
}

// Eine einzelne Expression ist eine gültige Liste von Expressions
exlist(r) ::= ex(a) .
{
    cJSON *arg = cJSON_CreateArray();
    cJSON_AddItemToArray(arg, a);
    r = arg;
}

// Kommaseparierte Expressions
exlist(r) ::= exlist(a) COMMA ex(b) .
{
    cJSON_AddItemToArray(a,b);
    r = a;
}