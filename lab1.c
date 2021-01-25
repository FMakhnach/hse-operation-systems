#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    printf("User id: %d\n", getuid());
    printf("Users group id: %d\n", getgid());

    return 0;
}

