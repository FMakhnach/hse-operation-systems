#include <stdio.h>

int main(int argc, char *argv[], char *envp[]){
  printf("There are %i command line arguments:\n", argc);
  for (int i = 0; i < argc; ++i) {
    printf("Argument #%i: %s\n", i + 1, argv[i]);
  }

  printf("\nEnvironment parameters:\n");
  while(*envp != NULL) {
    printf("%s\n", *envp);
    ++envp;
  }

  return 0;
}

