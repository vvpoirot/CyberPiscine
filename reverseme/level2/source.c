#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void no() {
    printf("Nope.\n");
    exit(1);
}

void ok() {
    printf("Good job.\n");
}

// lvl 2 : 
// input must be number >> split 3 by 3 >> atoi once part for get ascii code >> put 1 by 1 in final string

int main() {
    char input[255];
    
    printf("Input : ");
    scanf("%s", input);

    if (input[1] != '0')
        no();
    if (input[0] != '0')
        no();

    char final_string[255];
    final_string[0] = 'd';

    int it = 1;
    int i = 2;
    while (input[i]) {
        char part[4];
        strncpy(part, &input[i], 3);
        part[3] = '\0';
        final_string[it] = atoi(part);
        i += 3;
        it++;
    }
    final_string[it] = '\0';

    if (!strcmp(final_string, "delabere"))
        ok();
    else
        no();

    return 0;
}