#include <stdio.h>

int main(){
    int option;
    printf("Welcome to the Tic-Tac-Toe Game\n");
    while(1){
        printf("(1) Player vs Player");
        printf("\n(2) Player vs AI");
        printf("\n(3) Exit the game");
        printf("\nSelect the option you would like: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            
            break;
        
        case 2:
            
            break;
        case 3:
            printf("\nThamk You for playing");
            break;
        default:
            break;
        }
    }

    return 0;
}