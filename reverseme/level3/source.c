#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void no() {
    printf("Nope.\n");
    exit(1);
    printf("Good job.\n");
}

void _no() {
    printf("Good job.\n");
}

// lvl 3 : 
// Add 5 '*' to start input string
// *****42123 >> skip 42 >> replace char[1, 2, 3] by 123 >> *123*42123 >> atoi char[1, 2, 3] >> set in final string

int main() {
    char post_input[255];
    
    printf("Input : ");
    scanf("%s", post_input);

    char input[255];
    strcat(input, "*****");
    strcat(input, post_input);

    if (input[7] != '2')
        no();
    if (input[6] != '4')
        no();

    char final_string[255];
    final_string[0] = '*';

    int it = 1;
    int i = 2;
    while (input[i + 5]) {
        char part[4];
        input[1] = input[i + 6];
        input[2] = input[i + 7];
        input[3] = input[i + 8];
        strncpy(part, &input[1], 3);
        part[3] = '\0';
        final_string[it] = atoi(part);
        i += 3;
        it++;
    }
    final_string[it] = '\0';

    if (!strcmp(final_string, "********"))
        _no();
    else
        no();

    return 0;
}