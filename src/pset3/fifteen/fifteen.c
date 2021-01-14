/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// dimensions
int d;

// board
int **board;



// prototypes
void clear(void);
void greet(void);
void init(int d, int **board);
void draw(int d, int **board);
bool move(int tile, int d);
bool won(int d, int **board);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }
    int d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
    printf("Board must be between %i x %i and %i x %i, inclusive.\n",
        DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
    return 2;
    }
    // ensure valid dimensions
    int xyz;
    xyz = d * sizeof(int*);
    //printf("number of bytes to be allocated is: %i\n", xyz);
    board = malloc((size_t)xyz);
    for(int i = 0; i < d; i++) {
        board[i] = malloc(d * sizeof(int));
    }
    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init(d, board);
    //printf("test");

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        //clear();

        // draw the current state of the board
        draw(d, board);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(d, board))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile, d))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }
        /**else{
            move(tile);
        }*/

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    free(board);
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    //printf("\nsize of int is %lu\n", sizeof(int));
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(int d, int **board)
{
    int c = d * d - 1;
    for(int i = 0; i < d; i++){
        //printf("i = %i\n", i);
        for(int j = 0; j < d; j++){
            //printf("j = %i\n", j);
            if(i == d - 1 && j == d - 1){
                board[i][j] = 0;
            }
            if(d % 2 == 1){
                if(c == 0){
                    break;
                }
                //printf("c = %i, i = %i, j = %i\n", c, i, j);
                board[i][j] = c;
                //printf("c = %i\n", c);
                c--;
            }
            else{
                if(c == 0){
                    break;
                }
                if(c == 2){
                    //printf("test3\n");
                    board[i][j] = 1;
                    //printf("test4\n");
                    board[i][j + 1] = 2;
                    //printf("test5\n");
                    board[i][j + 2] = 0;
                    //printf("test6\n");
                    break;
                }
                board[i][j] = c;
                c--;
            }
        }
    }
}
    // TODO


/**
 * Prints the board in its current state.
 */
void draw(int d, int **board)
{
    //usleep(50000);
    clear();
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            if(board[i][j] != 0){
                printf("%i ", board[i][j]);
            }
            else{
                printf("%c", board[i][j]);
            }
        }
        printf("\n");
    }
}
    // TODO


/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile, int d)
{
    //printf("test\n");

    // TODO
    int empi, empj;
    //printf("test2\n");
    char copy;
    //printf("test2.a\n");
    for(int i = 0; i < d; i++){
        //printf("test2.b\n");
        //printf("test2.1\n");
        for(int j = 0; j < d; j++){
        //    printf("test2.2\n");
            copy = board[i][j];
            if(copy == 0){
                empi = i;
                empj = j;
            }
        }
    }
    //printf("Underscore i is:%i, underscore j is:%i\n", empi, empj);
    //printf("test\n");
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            //printf("test");
            if(tile == board[i][j]){
                //printf("Tile i is:%i, tile j is:%i\n", i, j);
               // printf("test\n");
                if(i + 1 == empi && j == empj){
                    board[i][j] = 0;
                    board[empi][j] = tile;
                //    printf("test");
                    return true;
                }
                if(j + 1 == empj && i == empi){
                    board[i][j] = 0;
                    board[i][empj] = tile;
                //    printf("test");
                    return true;
                }
                if(i - 1 == empi && j == empj){
                    board[i][j] = 0;
                    board[empi][j] = tile;
                  //  printf("test");
                    return true;
                }
                if(j - 1 == empj && i == empi){
                    board[i][j] = 0;
                    board[i][empj] = tile;
                    //printf("test");
                    return true;
                }
                else{
                   return false;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(int d, int **board)
{
    // TODO
    int check = 1;
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            if(check == d * d - 1){
                return true;
            }
            if(board[i][j] == check){
                check++;
            }
            else{
                return false;
            }
        }
        printf("\n");
    }
    return false;
}