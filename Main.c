#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 10 // Maximum grid size

int main()
{
    int option, always = 1, grid;
    printf("Welcome to the Tic-Tac-Toe Game\n");
    while (always)
    {
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
int initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = ' ';
        }
    }
    return 0;
}
int displayBoard(char board[MAX_SIZE][MAX_SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
        for (int k = 0; k < size; k++)
        {
            printf("----");
        }
        printf("-\n");
    }
    return 0;
}
int playerMove()
{
    int row, col;
    printf("Enter your move (row and column): ");
    scanf("%d %d", &row, &col);

    return 0;
}