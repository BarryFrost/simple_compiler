#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
  if(argc != 2) {
    fprintf(stderr, "%s: no argument is specified, exit.\n", argv[0]);
    return 1;
  }
  
  char *p = argv[1];

  printf("  .global main\n");
  printf("main:\n");
  printf("  mov $%ld, %%eax\n", strtol(p, &p, 10));

  while(*p) {
    if(*p == '+') {
    	p++;
	printf("  add $%ld, %%eax\n", strtol(p, &p, 10));
        continue;
    }

    if(*p == '-') {
        p++;
        printf("  sub $%ld, %%eax\n", strtol(p, &p, 10));
	continue;
    }
    
    fprintf(stderr, "unexpected character: '%c'\n", *p);
  }

  printf("  ret\n");
  return 0;
}
