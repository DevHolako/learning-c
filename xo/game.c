/*use rust or go; more effecient and new*/
#include <stdio.h>
#include <stdlib.h> // For system()

// Define a macro for clearing the screen based on the OS
#if defined(_WIN32) || defined(_WIN64)
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif


struct player
{
    char name[100];
    int wins;
    int loses;
    int draw;
    int turn;
    int is_gay;
    char symbol;
};


struct game
{
    int phase;
    struct player players[2];
    char state[3][3];
};



void print_state(char state[3][3]);
void update_state(struct game *game, int pos);
int check_winner(struct game *game);
int check_draw(struct game *game);



int main()
{
   

    int running = 1;
    struct game game = {0}; // Initialize the game struct with zero values
    struct player p1 = {0}; // Initialize player 1 with zero values
    struct player p2 = {0}; // Initialize player 2 with zero values

    // Initialize game state
    int counter_i = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game.state[i][j] = '0' + counter_i; // Initialize cells with characters '1', '2', etc.
            counter_i++;
        }
    }
    
    game.players[0] = p1;
    game.players[1] = p2;
    // Set player symbols
    game.players[0].symbol = 'X';
    game.players[1].symbol = 'O';
    game.phase = 1;

   
    while (running == 1)
    {
       if (game.phase == 2)
       {
           
            system(CLEAR_SCREEN); // Clear the screen
            print_state(game.state);

            struct player *current_player = NULL;
            for (int i = 0; i < 2; i++) {
                if (game.players[i].turn == 1) {
                    current_player = &game.players[i];
                    break;
                }
            }

            if (current_player == NULL) {
                printf("Error: No current player found.\n");
                return 1;
            }

            int pos; // Declare the variable to store the position
            printf("Please choose your position on the board: ");
            scanf("%d", &pos); // Pass the address of 'pos' to scanf
            
            // Validate position
            if (pos < 1 || pos > 9) {
                printf("Invalid position. Please choose a number between 1 and 9.\n");
                continue;
            }
            printf("current player %s chose pos : %d\n",current_player->name,pos);
            // update the stat of the game
            update_state(&game,pos);

            // Check for win or draw
            int winner = check_winner(&game);
            if (winner != 0) {
                printf("Player %s wins!\n", game.players[winner - 1].name);
                game.players[winner - 1].wins++;
                game.players[(winner - 1) ^ 1].loses++; // Loser is the other player
                running = 0;
                continue;
            }

            if (check_draw(&game)) {
                printf("It's a draw!\n");
                for (int i = 0; i < 2; i++) {
                    game.players[i].draw++;
                }
                running = 0;
                continue;
            }

            // Switch turns
            int toggler = game.players[0].turn;
            game.players[0].turn = game.players[1].turn;
            game.players[1].turn = toggler;
            continue;  
       }

       if(game.phase == 1){
          printf("Enter the name of player 1: ");
          scanf("%s",game.players[0].name);
      
          printf("Enter the name of player 2: ");
          scanf("%s",game.players[1].name);
          printf("p1 : %s vs p2 : %s\n", game.players[0].name,game.players[1].name);

          game.phase = 2;
          game.players[0].turn = 1;
          game.players[1].turn = 0;
          continue;
       }

        return 0;
    }
    return 0;
}


void print_state(char state[3][3]){
    for (int i = 0; i < 3; i++)
    {
       printf("%c | %c | %c\n",state[i][0],state[i][1],state[i][2]);
    
    }
}

 

void update_state(struct game *game,int pos){
    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;

    struct player *current_player = NULL;
    for (int i = 0; i < 2; i++) {
        if (game->players[i].turn == 1) {
            current_player = &game->players[i];
            break;
        }
    }

    if (current_player == NULL) {
        printf("Error: No current player found.\n");
        return;
    }

    if (game->state[row][col] == 'X' || game->state[row][col] == 'O') {
        printf("Position already taken. Please choose another position.\n");
        return;
    }

    game->state[row][col] = current_player->symbol;
}


int check_winner(struct game *game) {
    // Check rows, columns, and diagonals
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (game->state[i][0] == game->state[i][1] && game->state[i][1] == game->state[i][2]) {
            if (game->state[i][0] == 'X') return 1;
            if (game->state[i][0] == 'O') return 2;
        }
        // Check columns
        if (game->state[0][i] == game->state[1][i] && game->state[1][i] == game->state[2][i]) {
            if (game->state[0][i] == 'X') return 1;
            if (game->state[0][i] == 'O') return 2;
        }
    }
    // Check diagonals
    if (game->state[0][0] == game->state[1][1] && game->state[1][1] == game->state[2][2]) {
        if (game->state[0][0] == 'X') return 1;
        if (game->state[0][0] == 'O') return 2;
    }
    if (game->state[0][2] == game->state[1][1] && game->state[1][1] == game->state[2][0]) {
        if (game->state[0][2] == 'X') return 1;
        if (game->state[0][2] == 'O') return 2;
    }
    return 0; // No winner
}

int check_draw(struct game *game) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game->state[i][j] != 'X' && game->state[i][j] != 'O') {
                return 0; // At least one cell is empty
            }
        }
    }
    return 1; // All cells are filled
}
