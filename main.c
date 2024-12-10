#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* token_type[4] = {
  "punctuator",
  "number",
  "EOF",
  ""
};

typedef enum {
  TK_PUNCT,
  TK_NUM,
  TK_EOF,
} TokenKind;

// Token type
typedef struct Token Token;
struct Token {
  TokenKind kind; // Token kind
  Token *next;    // Next token
  int val;        // If kind is TK_NUM, its value
  char *loc;       // Token location
  int len;        // Token length
};

// error report function reports an error and exit
static void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Consumes the current token if it matches `s`
static bool equal(Token *tok, char *op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

// Ensure that the current token is `s`
static Token *skip(Token *tok, char *s) {
  if(!equal(tok, s))
    error("expected '%s'", s);
  return tok->next;
}

// Ensure that the current token is TK_NUM
static int get_number(Token *tok) {
  if(tok->kind != TK_NUM)
    error("expected a number");
  return tok->val;
}

// Create a new token
static Token *new_token(TokenKind kind, char *start, char *end) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

// Tokenize `p` and returns new tokens.
static Token *tokenize(char *p) {
  Token head = {};
  Token *cur = &head;

  while (*p) {
    // Skip whitespace characters.
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Numeruc literal
    if (isdigit(*p)) {
      cur = cur->next = new_token(TK_NUM, p, p); // form anew toekn and move to next pointer simultaneously
      char *q = p;
      cur->val = strtoul(p, &p, 10); // convert characters which are consecutive digit characters to a number and return an end pointer
      cur->len = p - q;
      continue;
    }

    // Punctuator
    if (*p == '+' || *p == '-') {
      cur = cur->next = new_token(TK_PUNCT, p, p + 1);
      p++;
      continue;
    }
    
    error("invalid token");
  }
  
  cur = cur->next = new_token(TK_EOF, p, p);
  return head.next;
}

int main(int argc, char **argv){
  if(argc != 2)
    error("%s: invalid number of arguments", argv[0]);

  Token *tok = tokenize(argv[1]);

  printf("  .global main\n");
  printf("main:\n");
  
  // The first token must be a number
  printf("/* token type: %s, value: %d, loc: %s, len: %d */\n",
    token_type[tok->kind], tok->val, tok->loc, tok->len);
  printf("  mov $%d, %%eax\n", get_number(tok));
  tok = tok->next;

  // ... followed by either `+ <number>` or `- <number>`.
  while (tok->kind != TK_EOF) {
    printf("/* token type: %s, value: %d, loc: %s, len: %d */\n",
    token_type[tok->kind], tok->val, tok->loc, tok->len);
    
    if (equal(tok, "+")) {
      printf("  add $%d, %%eax\n", get_number(tok->next));
      tok = tok->next->next;
      continue;
    }
    
    tok = skip(tok, "-"); 
    printf("  sub $%d, %%eax\n", get_number(tok));
    tok = tok->next;
  }

  printf("  ret\n");
  return 0;
}
