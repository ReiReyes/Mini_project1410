#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 
#include <unistd.h> 

int MALLOC_COUNT = 0;
int FREE_COUNT = 0;

#define malloc(x) (MALLOC_COUNT++, malloc(x))
#define free(x) (FREE_COUNT++, free(x))

typedef struct {
    int games_played;
    int wins_player1;
    int wins_player2;
    int draws;
    int * win_patterns; // Dynamic array : tracks HOW games were won 
    int pattern_count;
    int pattern_capacity;
    } GameStats ;


void updateStats ( GameStats * stats , char winner){
    stats->games_played++;
    if (winner == 'X') {
        stats->wins_player1++;
    } else if (winner == 'O') {
        stats->wins_player2++;
    } else if (winner == 'D') {
        stats->draws++;
    }
    if (winner != 'D') {
        if (stats->pattern_count >= stats->pattern_capacity) {
            stats->pattern_capacity *= 2;
            stats->win_patterns = realloc(stats->win_patterns, stats->pattern_capacity * sizeof(char));
        }
        stats->win_patterns[stats->pattern_count] = winner;
        stats->pattern_count++;
    }
}

void freeGameStats ( GameStats * stats ){
    stats->games_played = 0;
    stats->wins_player1 = 0;
    stats->wins_player2 = 0;
    stats->draws = 0;
    stats->pattern_count = 0;
    stats->pattern_capacity = 0;
    free(stats->win_patterns);
}
void clearScreen() {
#ifdef _WIN32         
    system("cls");     
#else      
    system("clear");   
#endif
}
const int MAX_SIZE = 11;
void mySleep(){
    Sleep(1000); 
    sleep(1);
}
void clearBoard(char **board, int size){
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = ' ';
        }
    }
}
char** createBoard ( int size, char **out_data ){
    char *data = malloc(size * size * sizeof(char));
    char **board = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        board[i] = data + (i * size);
    }
    *out_data = data;
    return board;
}

int saveGame(char **board, int size, int last_move, int option, const char *filename, GameStats *stats, int inplay, int difficulty)
{
    FILE *file = fopen(filename, "w");
    if (!file) return -1;

    fprintf(file, "%d\n", size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fputc(board[i][j], file);
        }
        fputc('\n', file);
    }

    fprintf(file, "%d\n", last_move);
    fprintf(file, "%d\n", option);

    fprintf(file, "%d\n", stats->games_played);
    fprintf(file, "%d\n", stats->wins_player1);
    fprintf(file, "%d\n", stats->wins_player2);
    fprintf(file, "%d\n", stats->draws);
    fprintf(file, "%d\n", stats->pattern_count);
    fprintf(file, "%d\n", stats->pattern_capacity);
    for (int i = 0; i < stats->pattern_count; i++) {
        fputc(stats->win_patterns[i], file);
    }
    fprintf(file, "%d\n", inplay); 
    fprintf(file, "%d\n", difficulty);
    fclose(file);
    return 0;
}

int loadGame(char ***board, int *size, int *last_move, int *option,const char *filename, char **data, GameStats *stats, int *inplay, int *difficulty) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    fscanf(file, "%d\n", size);

    *board = createBoard(*size, data);   // alloc board + data

    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            (*board)[i][j] = fgetc(file);
        }
        fgetc(file); // newline
        
        
    }
    fscanf(file, "%d\n", last_move);
    fscanf(file, "%d\n", option); 
    fscanf(file, "%d\n", &stats->games_played);
    fscanf(file, "%d\n", &stats->wins_player1);
    fscanf(file, "%d\n", &stats->wins_player2);
    fscanf(file, "%d\n", &stats->draws);
    fscanf(file, "%d\n", &stats->pattern_count);
    fscanf(file, "%d\n", &stats->pattern_capacity);
    stats->win_patterns = malloc(stats->pattern_capacity * sizeof(char));
    for (int i = 0; i < stats->pattern_count; i++) {
        stats->win_patterns[i] = fgetc(file);
    }
    fscanf(file, "%d\n", inplay);
    fscanf(file, "%d\n", difficulty);
    fclose(file);
    return 0;
}

void printBoard(int size, char **board) {
    printf("\n  ");
    for(int i =1; i <= size; i++){
        printf(" %d  ",i);
    }
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%c ", i+65);
        for(int j = 0; j < size; j++){
            if(j < size-1){
                printf(" %c |", board[i][j] );
            }else{
                printf(" %c", board[i][j]);
            }
        }
        printf("\n");
        printf("   ---");
        for(int i = 1; i < size-1; i++){
            printf("----");
        }
        
        printf("--\n");
    }
}


char isGameOver(int size, char **board) {
    for (int i = 0; i < size; i++) { // check rows
        char first = board[i][0];
        if (first == ' ') continue;
        int win = 1;
        for (int j = 1; j < size; j++) {
            if (board[i][j] != first) {
                win = 0;
                break;
            }
        }
    if (win) return first;
    }

    for (int i = 0; i < size; i++) {//check for collums
        char first = board[0][i];
        if (first == ' ') continue;
        int win = 1;
        for (int j = 1; j < size; j++) {
            if (board[j][i] != first) {
                win = 0;
                break;
            }
        }
    if (win) return first;
    }
    

    char first = board[0][0];   // check for diagonals 
        if (first != ' '){
            int win = 1;
            for (int i = 1; i < size; i++) {
                if (board[i][i] != first) {
                    win = 0;
                    break;
                }
            }
        if (win) return first;
    }
    

    first = board[0][size - 1]; // check for diagonals from other way
    if (first != ' ') {
        int win = 1;
        for (int i = 1; i < size; i++) {
            if (board[i][size - 1 - i] != first) {
                win = 0;
                break;
            }
        }
        if (win) return first;
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(board[i][j] == ' '){
                return ' '; // no winner yet
            }
        }
    
    }
return 'D'; // Draw
}
void printStatistics ( const GameStats * stats, int option ){ 
    printf("Games Played: %d\n", stats->games_played);
    printf("Player 1 Wins: %d\n", stats->wins_player1);
    if(option == 2){
        printf("AI Wins: %d\n", stats->wins_player2);
    }else{
        printf("Player 2 Wins: %d\n", stats->wins_player2);
    }
    printf("Draws: %d\n", stats->draws);
    printf("Win Patterns: ");
    for (int i = 0; i < stats->pattern_count; i++) {
        printf("%c ", stats->win_patterns[i]);
    }
    printf("\n");
}
int wantToPlay(int keepPlaying, GameStats *stats, int grid, char **board, int player, int option, int inplay, int difficulty){
    while (1){
        printf("\n(1) Play Rematch\n(2) Check Stats \n(3) Save Game\n(4) Go Back To Menu\nWhat do you want to do: "); // have to implement save game option
        scanf("%d", &keepPlaying);
            if(keepPlaying == 4){
                keepPlaying = 0;
                clearScreen();
                freeGameStats(stats);
                return keepPlaying;
                
            }else if(keepPlaying == 3){
                int inplay = 0;
                saveGame(board, grid, player, option, "saved_game.txt", stats, inplay, difficulty);
                clearScreen();
                printf("Game saved successfully.\n");
            }else if(keepPlaying == 2){
                clearScreen();
                printStatistics(stats, option);
            }
            else if(keepPlaying == 1){
                return keepPlaying;
            }else{
                printf("\nInvalid Input, \n%d is not a valid option", keepPlaying);
            }
        
    }
    return 0;
}

int* aiMoveDecision(int size, char **board, int difficulty){
    int playerWin = 0, aiMove[2];
    int *ptr = aiMove;
    if (difficulty == 2){ //Medium Difficulty
        for (int i = 0; i < size; i++) { //row winning
            playerWin=0;
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 'O') {
                    playerWin++;
                }
                if (board[i][j] == 'X') {
                    playerWin--;
                }
            }
            if(playerWin ==  size-1){
                for (int j = 0; j < size; j++) {
                    if(board[i][j] == ' '){
                        aiMove[0] = i;
                        aiMove[1] = j;
                        return ptr;
                    }
                }
            }
        }

        for (int i = 0; i < size; i++) { // column winning
            playerWin=0;
            for (int j = 0; j < size; j++) {
                if (board[j][i] == 'O') {
                    playerWin++;
                }
                if (board[j][i] == 'X') {
                    playerWin--;
                }
            }
            if(playerWin ==  size-1){
                for (int j = 0; j < size; j++) {
                    if(board[j][i] == ' '){
                        aiMove[0] = j;
                        aiMove[1] = i;
                        return ptr;
                    }
                }
            }
        }

        playerWin = 0;
        for (int i = 0; i < size; i++) { //Diagonal winning
            if (board[i][i] == 'O') {
                playerWin++;
            }
            if (board[i][i] == 'X') {
                playerWin--;
            }
        }
        if(playerWin == size-1){
            for (int i = 0; i < size; i++) {
                if (board[i][i] == ' ') {
                    aiMove[0] = i;
                    aiMove[1] = i;
                    return ptr;
                }
        }
        }

        playerWin = 0;
        for (int i = 0; i < size; i++) { //AntiDiagonal winning
            if (board[i][size - 1 - i] == 'O') {
                playerWin++;
            }
            if (board[i][size - 1 - i] == 'X') {
                playerWin--;
            }
        }
        if(playerWin == size-1){
            for (int i = 1; i < size; i++) {
                if (board[i][size - 1 - i] == ' ') {
                    aiMove[0] = i;
                    aiMove[1] = size - 1 - i;
                    return ptr;
                }
        }
        }

        for (int i = 0; i < size; i++) { //row blocking
            playerWin=0;
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 'X') {
                    playerWin++;
                }
                if (board[i][j] == 'O') {
                    playerWin--;
                }
            }
            if(playerWin ==  size-1){
                for (int j = 0; j < size; j++) {
                    if(board[i][j] == ' '){
                        aiMove[0] = i;
                        aiMove[1] = j;
                        return ptr;
                    }
                }
            }
        }

        for (int i = 0; i < size; i++) { // column blocking
            playerWin=0;
            for (int j = 0; j < size; j++) {
                if (board[j][i] == 'X') {
                    playerWin++;
                }
                if (board[j][i] == 'O') {
                    playerWin--;
                }
            }
            if(playerWin ==  size-1){
                for (int j = 0; j < size; j++) {
                    if(board[j][i] == ' '){
                        aiMove[0] = j;
                        aiMove[1] = i;
                        return ptr;
                    }
                }
            }
        }

        playerWin = 0;
        for (int i = 0; i < size; i++) { //Diagonal Blocking
            if (board[i][i] == 'X') {
                playerWin++;
            }
            if (board[i][i] == 'O') {
                playerWin--;
            }
        }
        if(playerWin == size-1){
            for (int i = 0; i < size; i++) {
                if (board[i][i] == ' ') {
                    aiMove[0] = i;
                    aiMove[1] = i;
                    return ptr;
                }
        }
        }

        playerWin = 0;
        for (int i = 0; i < size; i++) { //AntiDiagonal Blocking
            if (board[i][size - 1 - i] == 'X') {
                playerWin++;
            }
            if (board[i][size - 1 - i] == 'O') {
                playerWin--;
            }
        }

        if(playerWin == size-1){
            for (int i = 0; i < size; i++) {
                if (board[i][size - 1 - i] == ' ') {
                    aiMove[0] = i;
                    aiMove[1] = size - 1 - i;
                    return ptr;
                }
        }
        }
    }
    if (difficulty == 3){ //Hard Difficulty
        // Future implementation of a more advanced AI algorithm can be placed here.
    }
    while(1){
        int move1 = rand()%size;
        int move2 = rand()%size;
        if(board[move1][move2] == ' '){
            aiMove[0] = move1;
            aiMove[1] = move2;
            return ptr;
        }
    }
    return ptr;
}
void Gamepva(int grid,  char **board, int playerload, int *save, int option, GameStats *stats, int difficulty){
    int player = 1;
    if (playerload != 0) {
        player = playerload;
    }else{
        clearBoard(board, grid);
    }
    while(1){
        clearScreen();
        printBoard(grid, board);
        int row, col;
        char rowchar;
        printf("Player %d, please enter a row and column (e.g., 1 A) or enter (0 0) to save: ", player);
        while (scanf("%d %c", &col, &rowchar) != 2) {
                printf("Invalid input, please try again: ");
                while (getchar() != '\n');
        }
        
        row = rowchar - 64;
        if (col == 0 || row == -64) {
                clearScreen();
                int inplay = 1;
                saveGame(board, grid, player, option, "saved_game.txt", stats, inplay, difficulty);
                freeGameStats(stats);
                printf("Game saved successfully.\n");
                *save = 1;
                break;
        }
        if (row < 1 || row > grid || col < 1 || col > grid || board[row - 1][col - 1] != ' ') {
                printf("Invalid move, please try again.\n");
                mySleep();
                
        } else {
                if (player == 1) {
                    board[row - 1][col - 1] = 'X';
                    player = 2;
                }
        }
        if(player == 2 && isGameOver(grid, board) == ' '){
            int *aimove= aiMoveDecision(grid, board, difficulty);
            board[*aimove][*(aimove+1)] = 'O';
            player = 1;
        }

        if (isGameOver(grid, board) != ' '){
            clearScreen();
            printf("Game over!\n");
            printBoard(grid, board);
            char winner = isGameOver(grid, board);
            updateStats(stats, winner);
            if (winner != 'D') {
                printf("Player %c wins!\n", winner);
                
            } else {
                printf("It's a draw!\n");
            }
            break;
    }
}
}
void Gamepvp(int grid,  char **board, int playerload, int *save, int option, GameStats *stats){
    int player = 1;
    if (playerload != 0) {
        player = playerload;
    }else{
        clearBoard(board, grid);
        }
    while(1){
        clearScreen();
        printBoard(grid, board);
        int row, col;
        char rowchar;
        
        printf("Player %d, please enter a row and column (e.g., 1 A) or enter (0 0) to save: ", player);
        while (scanf("%d %c", &col, &rowchar) != 2) {
                printf("Invalid input, please try again: ");
                while (getchar() != '\n');
        }
        
        row = rowchar - 64;
        if (col == 0 || row == -64) {
                clearScreen();
                int inplay = 1;
                saveGame(board, grid, player, option, "saved_game.txt", stats, inplay, 0);
                freeGameStats(stats);
                printf("Game saved successfully.\n");
                *save = 1;
                break;
        }
        if (row < 1 || row > grid || col < 1 || col > grid || board[row - 1][col - 1] != ' ') {
                printf("Invalid move, please try again.\n");
                mySleep();
                
        } else {
                if (player == 1) {
                    board[row - 1][col - 1] = 'X';
                    player = 2;
                } else {
                    board[row - 1][col - 1] = 'O';
                    player = 1;
                }
        }

        if (isGameOver(grid, board) != ' '){
            clearScreen();
            printf("Game over!\n");
            printBoard(grid, board);
            char winner = isGameOver(grid, board);
            updateStats(stats, winner);
            if (winner != 'D') {
                printf("Player %c wins!\n", winner);
            } else {
                printf("It's a draw!\n");
            }
            break;
        }
    }
        
}

void printScore(int option, GameStats *stats){
    if(option != 2){
        printf("\nScores: \nPlayer 1: %d \nPlayer 2: %d\n", stats->wins_player1, stats->wins_player2);
    }else{
        printf("\nScores: \nPlayer 1: %d \nAI: %d\n", stats->wins_player1, stats->wins_player2);
    }
    
}



GameStats  createGameStats (){
    GameStats stats;
    stats.games_played = 0;
    stats.wins_player1 = 0;
    stats.wins_player2 = 0;
    stats.draws = 0;
    stats.pattern_count = 0;
    stats.pattern_capacity = 5; 
    stats.win_patterns = malloc(stats.pattern_capacity *sizeof(char));
    return stats;
}



int main(){
    srand(time(NULL));
    int option, always = 1, grid, keepPlaying, playerload = 0, inplay = 0, difficulty = 0;
    char *data = NULL;
    char **boardptr = NULL;
    GameStats stats;
    printf("Welcome to the Tic-Tac-Toe Game\n");
    while(always){
        printf("(1) Player vs Player");
        printf("\n(2) Player vs AI");
        printf("\n(3) Load saved game");
        printf("\n(4) Exit the game");
        printf("\nSelect the option you would like: ");
        scanf("%d", &option);
        if (option == 2 || option == 1){
            while (1) {
                printf("\nWhich grid size do you want (put n for nxn size grid): ");

                if (scanf("%d", &grid) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    while (getchar() != '\n');
                    continue; 
                }
                while (getchar() != '\n'); 
                if (grid >= 3 && grid <= 15) {
                    boardptr = createBoard(grid, &data);
                    stats = createGameStats();
                    break; 
                } else {
                    printf("Invalid grid size. Must be between 3 and 15.\n");
                }
            }
        }
        int save = 0;
       
        
        switch (option)
        {
        case 1:
            keepPlaying = 1;
            stats.wins_player1 = 0;
            stats.wins_player2 = 0;
            while(keepPlaying){
                clearBoard(boardptr, grid);
                Gamepvp(grid, boardptr, playerload, &save, option, &stats);
                if(save == 1){
                    break;
                }
                printScore(option, &stats);
                keepPlaying = wantToPlay(keepPlaying, &stats, grid, boardptr, playerload, option, inplay, difficulty);
                
            }
            free(boardptr);
            free(data);
            data = NULL;
            boardptr = NULL;
            
            break;
        
        case 2:
            while(1){
            printf("\nSelect Difficulty Level:\n(1) Easy\n(2) Medium\n(3) Hard\nYour choice: ");
            scanf("%d", &difficulty);
            if(difficulty >=1 && difficulty <=3){
                break;
            }else{
                printf("\nInvalid Input, \n%d is not a valid option", difficulty);
            }
            }
            keepPlaying =1;
            stats.wins_player1 = 0;
            stats.wins_player2 = 0;
            while(keepPlaying){
                clearBoard(boardptr, grid);
                Gamepva(grid, boardptr, playerload, &save, option, &stats, difficulty);
                if(save == 1){
                    break;
                }
                printScore(option, &stats);
                keepPlaying = wantToPlay(keepPlaying, &stats, grid, boardptr, playerload, option, inplay, difficulty);
            }
            free(boardptr);
            free(data);
            data = NULL;
            boardptr = NULL;
            break;
        case 3:
            
            if (boardptr != NULL) {
                free(data);
                free(boardptr);
            }
            if (loadGame(&boardptr, &grid, &playerload, &option, "saved_game.txt", &data, &stats, &inplay, &difficulty) == -1) {
                printf("No saved game found.\n");
                break;
            }
            if(inplay == 0){
                playerload = 0;
            }
            keepPlaying =1;
            while (keepPlaying) {
                if(option == 2){
                    Gamepva(grid, boardptr, playerload, &save, option, &stats, difficulty);
                }else{
                    Gamepvp(grid, boardptr, playerload, &save, option, &stats);
                }
                if(save == 1){
                    break;
                }
                printScore(option, &stats);
                keepPlaying = wantToPlay(keepPlaying, &stats, grid, boardptr, playerload, option, inplay, difficulty);
                playerload = 0;
            }

            free(data);
            free(boardptr);
            data = NULL;
            boardptr = NULL;
            break;
            
        case 4:
            printf("\nThank You for playing");
            always = 0;
            break;

        default:
            break;
        }
        
    } 
    printf("\nMallocs: %d\nFrees: %d\n", MALLOC_COUNT, FREE_COUNT); // Memory allocation report erase when not needed

    return 0;
}