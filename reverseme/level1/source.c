#include <stdio.h>
#include <string.h>

// lvl 1 : 
// strcmp input with password

int main() {
    char input[50];
    printf("Please enter key: ");
    scanf("%s", input);
    if (!strcmp(input, "__stack_check"))
        printf("Good job.\n");
    else
        printf("Nope.\n");
    return (0);
}