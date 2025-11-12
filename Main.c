#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int option, always =1, grid;
    printf("Welcome to the Tic-Tac-Toe Game\n");
    while(always){
        printf("(1) Player vs Player");
        printf("\n(2) Player vs AI");
        printf("\n(3) Exit the game");
        printf("\nSelect the option you would like: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Which grid size do you want (put n for nxn size grid): ");
            scanf("%d", grid);
            
            break;
        
        case 2:
            
            break;
        case 3:
            printf("\nThank You for playing");
            always = 0;
            break;

        default:
            break;
        }
    }

    return 0;
}