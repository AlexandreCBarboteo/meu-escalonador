#include <stdio.h>

int main(){
    int i = 0;
    while(1) {
        if (i == 5)
            break;

        printf("i = %d\n", i);
        i++;
    }
    return 0;
}