#include <iostream>
#include <cstdlib>
#include <ctime>

const int horizontal_line = 205, vertical_line = 186, horizontal_vertical = 206;

using namespace std;

struct Game_Data { 
	char atoms_board[12][12];
	char board[12][12];
	char bufor[12][12];
	char move;
	char beam_direction;
	int guessed_atoms = 0;
	int detour_number = 49;
	bool ending_game = false;
	bool if_end = false;
	char detour = char(detour_number);
	unsigned atoms, x_cursor = 1, y_cursor = 1, x_beam, y_beam, board_size, score = 0;
	char cell[5] = { ' ', ' ', 'T', ' ', ' '};
};

Game_Data initialization(unsigned atoms, unsigned board_size)
{
    Game_Data state;
	state.atoms = atoms;
	state.board_size = board_size;

	for (unsigned i = 0; i < state.board_size + 2; i++) 
	{
		for (unsigned j = 0; j < state.board_size + 2; j++)
		{
			state.board[i][j] = ' ';
		}
	}
	state.board[state.y_cursor][state.x_cursor] = 'Y';

	for (unsigned i = 0; i < state.board_size + 2; i++) 
	{
		for (unsigned j = 0; j < state.board_size + 2; j++)
		{
			state.bufor[i][j] = ' ';
		}
	}
	for (unsigned i = 0; i < state.board_size + 2; i++) 
	{
		for (unsigned j = 0; j < state.board_size + 2; j++)
		{
			state.atoms_board[i][j] = ' ';
		}
	}
	while (atoms > 0) 
	{
		int row = rand() % state.board_size + 1; 
		int column = rand() % state.board_size + 1;

		if (state.atoms_board[row][column] == ' ')
		{
			state.atoms_board[row][column] = 'T';
			atoms--;
		}
	}

	return state;
}

void guess(Game_Data& state); void move_up(Game_Data& state);
void move_down(Game_Data& state); void move_right(Game_Data& state);
void move_left(Game_Data& state); void end(Game_Data& state);
void beam(Game_Data& state); void print_menu(); void difficulty_choosing();

void print_board( Game_Data& state)
{
	system("cls");
	if (state.move != 'K')
    {
        cout << "CONTROLS\n";
        cout << "W, S, A, D and (w, s, a, d) - move around the board: up, down, left, and right respectively\n";
        cout << "Q and q - exit the game\n";
        cout << "Spacebar - fire a shot when the cursor is on the wall\n";
        cout << "o - mark a suspected atom position\n";
        cout << "k - end the game and display the solution along with your score\n";
        cout << "p - display the solution\n";
        cout << "H - temporarily show the positions of atoms on the board\n\n";
    }


	if(state.board[state.y_cursor][state.x_cursor] == ' ' && state.if_end == false)
		state.board[state.y_cursor][state.x_cursor] = 'Y';
	
	cout << '\n';
	 
	for (unsigned i = 0; i < state.board_size + 2; i++)													
	{																									
		for (unsigned j = 0; j < state.board_size + 2; j++)												
		{																								
			if ((i == 0 || i == state.board_size + 1) && j == 0)                                        
			{																							
				state.cell[2] = state.board[i][j];
				for (int i = 0; i < 5; i++)
					cout << state.cell[i];
				cout << char(vertical_line);

			}
			else if(j == 0)
			{
				state.cell[2] = state.board[i][j];
				for (int i = 0; i < 5; i++)
					cout << state.cell[i];
				if(j != state.board_size + 1)
					cout << char(vertical_line);
			}
			else
			{
				state.cell[2] = state.board[i][j];
				for (int i = 0; i < 5; i++)
					cout << state.cell[i];
				if (j != state.board_size + 1)
					cout << char(vertical_line);
			}

			if (j == state.board_size + 1)
				if (i == 0 || i == state.board_size + 1)
					cout << endl;
				else
					cout  << endl;
		}																															
																																	
		for (unsigned j = 0; j < state.board_size + 2; j++) 													
		{																															
			if (i == state.board_size + 1 && (j == 0 || j == state.board_size + 1))													
				cout << "      ";																									
			else
			{
				if (i != state.board_size + 1) 
				{
					if (j != state.board_size + 1)
					{
						for (int k = 0; k < 6; k++)
						{
							if (k < 5)
								cout << char(horizontal_line);
							else
								cout << char(horizontal_vertical);
						}
					}					
					else
					{
						for (int k = 0; k < 6; k++) 
						{
							cout << char(horizontal_line);
						}
					}

				}
			}

		}
		if ((i == (state.board_size + 1) / 2) && state.move != 'K' && state.move != 'k')
			cout << "           You marked " << state.guessed_atoms << " atoms out of " << state.atoms;
		cout << endl;
	}

	if (state.move == 'K' || state.move == 'k')	
    {
        if (state.score == state.atoms)
            cout << "You won!!!";
        else if (state.score > 1 && state.score < state.atoms)
            cout << "\n\nYou managed to guess the positions of " << state.score << " atoms out of " << state.atoms;
        else if (state.score == 1)
            cout << "\n\nYou managed to guess the position of " << state.score << " atom out of " << state.atoms;
        else
            cout << "You didn't manage to guess the position of any atoms :(";
    }


}

void end(Game_Data& state)
{
		for (unsigned i = 1; i <= state.board_size; i++)	
		{														
			for (unsigned j = 1; j <= state.board_size; j++)
			{
				if (state.board[i][j] == 'o' && state.atoms_board[i][j] == ' ')
					state.board[i][j] = 'X';
				else if (state.board[i][j] == ' ' && state.atoms_board[i][j] == 'T')
					state.board[i][j] = 'O';
				else if (state.board[i][j] == 'o' && state.atoms_board[i][j] == 'T')
				{
					state.board[i][j] = 'O';
					state.score += 1;
				}
				else if (state.board[i][j] == 'Y')
				{
					state.board[i][j] = ' ';
					state.if_end = true;
				}

			}
		}
	print_board(state);
}

void hit_checking(Game_Data& state, bool& beam_continue)
{
	if (state.atoms_board[state.y_beam][state.x_beam] == 'T')
	{
		state.board[state.y_cursor][state.x_cursor] = 'H';
		beam_continue = false;
	}
}

void edge_reverse_checking(Game_Data& state, bool& beam_continue, char& beam_placement)
{
	switch (beam_placement)
	{
	case 'B':
		if (state.atoms_board[state.y_beam][state.x_beam - 1] == 'T' || state.atoms_board[state.y_beam][state.x_beam + 1] == 'T')
		{
			state.board[state.y_cursor][state.x_cursor] = 'R';				  
			beam_continue = false;
		}
		break;
	case 'S':
		if (state.atoms_board[state.y_beam - 1][state.x_beam] == 'T' || state.atoms_board[state.y_beam + 1][state.x_beam] == 'T')
		{
			state.board[state.y_cursor][state.x_cursor] = 'R';				 
			beam_continue = false;
		}
		break;
	}
}

void beam(Game_Data& state)
{
	bool beam_continue = true;
	char beam_placement;

	if ((state.x_cursor == 0 || state.x_cursor == state.board_size + 1) || (state.y_cursor == 0 || state.y_cursor == state.board_size + 1))
	{																																	   
		state.y_beam = state.y_cursor;																									   
		state.x_beam = state.x_cursor;
		if (state.y_cursor == 0) 
		{
			state.beam_direction = 'D';
			beam_placement = 'B';
			state.y_beam++;
			hit_checking(state, beam_continue);
			if (beam_continue == true)
			{
				edge_reverse_checking(state, beam_continue, beam_placement);
			}
		}
		if (state.y_cursor == state.board_size + 1) 
		{
			state.beam_direction = 'T';
			beam_placement = 'B';
			state.y_beam--;
			hit_checking(state, beam_continue);
			if (beam_continue == true)
			{
				edge_reverse_checking(state, beam_continue, beam_placement);
			}
		}
		if (state.x_cursor == 0) 
		{
			state.beam_direction = 'R';
			beam_placement = 'S';
			state.x_beam++;
			hit_checking(state, beam_continue);
			if (beam_continue == true)
			{
				edge_reverse_checking(state, beam_continue, beam_placement);
			}
		}
		if (state.x_cursor == state.board_size + 1) 
		{
			state.beam_direction = 'L';
			beam_placement = 'S';
			state.x_beam--;
			hit_checking(state, beam_continue);
			if (beam_continue == true)
			{
				edge_reverse_checking(state, beam_continue, beam_placement);
			}
		}
		while (state.x_beam > 0 && state.x_beam < state.board_size + 1 && state.y_beam > 0 && state.y_beam < state.board_size + 1 && beam_continue == true)
		{
			switch (state.beam_direction)
			{
				case 'D': 
					
					if (state.atoms_board[state.y_beam][state.x_beam] != 'T') 
					{
						if (state.atoms_board[state.y_beam + 1][state.x_beam] == 'T')
						{
							state.board[state.y_cursor][state.x_cursor] = 'H';				           
							beam_continue = false;											  
						}
						else if (state.atoms_board[state.y_beam + 1][state.x_beam - 1] == 'T' && state.atoms_board[state.y_beam + 1][state.x_beam + 1] == 'T')  
						{																																 	  
							state.board[state.y_cursor][state.x_cursor] = 'R';                
							beam_continue = false;                                            
						}
						else if (state.atoms_board[state.y_beam + 1][state.x_beam - 1] == 'T')
						{
							state.beam_direction = 'R';										 
							state.x_beam++;                                                  
						}
						else if (state.atoms_board[state.y_beam + 1][state.x_beam + 1] == 'T')
						{
							state.beam_direction = 'L';										  
							state.x_beam--;                                                  
						}
						else
						{
							state.y_beam++;
						}
					}
					else
					{
						state.board[state.y_cursor][state.x_cursor] = 'H';
						beam_continue = false;
					}
					break;
				case 'T':
					
					if (state.atoms_board[state.y_beam][state.x_beam] != 'T')
					{
						if (state.atoms_board[state.y_beam - 1][state.x_beam] == 'T')         
						{																	  
							state.board[state.y_cursor][state.x_cursor] = 'H';
							beam_continue = false;
						}
						else if (state.atoms_board[state.y_beam - 1][state.x_beam - 1] == 'T' && state.atoms_board[state.y_beam - 1][state.x_beam + 1] == 'T')
						{
							state.board[state.y_cursor][state.x_cursor] = 'R';                 
							beam_continue = false;                                            
						}
						else if (state.atoms_board[state.y_beam - 1][state.x_beam - 1] == 'T')
						{
							state.beam_direction = 'R';										  
							state.x_beam++;                                                   
						}
						else if (state.atoms_board[state.y_beam - 1][state.x_beam + 1] == 'T')
						{
							state.beam_direction = 'L';										  
							state.x_beam--;                                                   
						}
						else
						{
							state.y_beam--;
						}
					}
					else
					{
						state.board[state.y_cursor][state.x_cursor] = 'H';
						beam_continue = false;
					}
					break;
				case 'L': 
					
					if (state.atoms_board[state.y_beam][state.x_beam] != 'T')
					{
						if (state.atoms_board[state.y_beam][state.x_beam - 1] == 'T')         
						{																	      
							state.board[state.y_cursor][state.x_cursor] = 'H';                
							beam_continue = false;
						}
						else if (state.atoms_board[state.y_beam + 1][state.x_beam - 1] == 'T' && state.atoms_board[state.y_beam - 1][state.x_beam - 1] == 'T')
						{
							state.board[state.y_cursor][state.x_cursor] = 'R';                  
							beam_continue = false;                                             
						}                                                                      
						else if (state.atoms_board[state.y_beam + 1][state.x_beam - 1] == 'T')
						{
							state.beam_direction = 'T';										  
							state.y_beam--;                                                   
						}                                                                      
						else if (state.atoms_board[state.y_beam - 1][state.x_beam - 1] == 'T')
						{
							state.beam_direction = 'D';										  
							state.y_beam++;                                                   
						}																	  
						else
						{
							state.x_beam--;
						}
					}
					else
					{
						state.board[state.y_cursor][state.x_cursor] = 'H';
						beam_continue = false;
					}
					break;
				case 'R': 
					
					if (state.atoms_board[state.y_beam][state.x_beam] != 'T')
					{
						if (state.atoms_board[state.y_beam][state.x_beam + 1] == 'T')       
						{																	
							state.board[state.y_cursor][state.x_cursor] = 'H';				
							beam_continue = false;
						}
						else if (state.atoms_board[state.y_beam + 1][state.x_beam + 1] == 'T' && state.atoms_board[state.y_beam - 1][state.x_beam + 1] == 'T')
						{
							state.board[state.y_cursor][state.x_cursor] = 'R';                 
							beam_continue = false;                                             
						}                                                                     
						else if (state.atoms_board[state.y_beam + 1][state.x_beam + 1] == 'T')
						{                                                                      
							state.beam_direction = 'T';										   
							state.y_beam--;                                                   
						}																	  
						else if (state.atoms_board[state.y_beam - 1][state.x_beam + 1] == 'T')
						{																	  
							state.beam_direction = 'D';										    
							state.y_beam++;                                                    
						}																	  
						else
						{
							state.x_beam++;
						}
					}
					else
					{
						state.board[state.y_cursor][state.x_cursor] = 'H';
						beam_continue = false;
					}
					break;
			}
		}
		if (beam_continue == true)
		{
				state.detour = char(state.detour_number);
				state.board[state.y_cursor][state.x_cursor] = state.detour;
				state.board[state.y_beam][state.x_beam] = state.detour;
				state.detour_number++;
				if (state.detour_number == 58)
					state.detour_number = 65;
		}
	} 
	print_board(state);
}

void guess(Game_Data& state)
{
	if (state.x_cursor > 0 && state.x_cursor < state.board_size + 1 && state.y_cursor > 0 && state.y_cursor < state.board_size + 1) 
	{																																
		if (state.board[state.y_cursor][state.x_cursor] == 'o')																		
		{
			state.board[state.y_cursor][state.x_cursor] = ' ';
			state.guessed_atoms--;
		}

		else
		{
			state.board[state.y_cursor][state.x_cursor] = 'o';
			state.guessed_atoms++;
		}
		state.ending_game = (state.guessed_atoms == state.atoms) ? true : false;
	}
	print_board(state);
}

void help(Game_Data& state)
{
	for (unsigned i = 1; i < state.board_size + 1; i++)
	{
		for (unsigned j = 1; j < state.board_size + 1; j++)
		{
			if (state.atoms_board[i][j] == 'T')
			{
				state.bufor[i][j] = state.board[i][j];
				state.board[i][j] = state.atoms_board[i][j];
			}
			else if (state.board[i][j] == 'Y' || state.board[i][j] == 'o')
			{
				state.if_end = true; 
				state.bufor[i][j] = state.board[i][j];
				state.board[i][j] = ' ';
			}
		}
	}

	print_board(state);
	clock_t displaying_time;
	displaying_time = clock() + 3 * CLOCKS_PER_SEC;
	while (clock() < displaying_time) {}
	state.if_end = false;
	for (unsigned i = 1; i < state.board_size + 1; i++)
	{
		for (unsigned j = 1; j < state.board_size + 1; j++)
		{
			state.board[i][j] = state.bufor[i][j];
		}
	}
	for (unsigned i = 0; i < state.board_size + 2; i++) 
	{
		for (unsigned j = 0; j < state.board_size + 2; j++)
		{
			state.bufor[i][j] = ' ';
		}
	}
	print_board(state);
}

void solution_display(Game_Data& state)
{
	char back_to_menu;

	system("cls");
	cout << "Game solution: \n\n";

	for (unsigned i = 0; i < state.board_size + 2; i++)
	{
		for (unsigned j = 0; j < state.board_size + 2; j++)
		{
			state.board[i][j] = state.atoms_board[i][j];
		}
	}
	state.if_end = true;
	print_board(state);
	cout << endl;
	do
	{
		cout << "Do you want to return to the main menu? (Y - Yes, N - No) "; cin >> back_to_menu;
		if (back_to_menu == 'Y')
		{
			print_menu();
			difficulty_choosing();
		}
		else if(back_to_menu == 'N')
			exit(0);
		cout << '\n';
	} while (back_to_menu != 'Y' && back_to_menu != 'N');

}

void moving_check(Game_Data& state, bool& whitespace)
{
	if (state.board[state.y_cursor][state.x_cursor] != 'o' && state.board[state.y_cursor][state.x_cursor] != 'H' && state.board[state.y_cursor][state.x_cursor] != 'R')
	{
		whitespace = true;
	}
	for (unsigned i = 49; i < 72; i++)
	{
		if (state.board[state.y_cursor][state.x_cursor] == char(i))
		{
			whitespace = false;
			break;
		}
	}
}

void moving(Game_Data& state)
{
	do
	{
		do
		{
				cin.get(state.move);
				cout << "Make your move: ";
		} while (state.move != 'H' && state.move != ' ' && state.move != 'W' && state.move != 'S' && state.move != 'A' &&
				 state.move != 'D' && state.move != 'Q' && state.move != 'O' && state.move != 'K' && state.move != 'o' &&
			     state.move != 'k' && state.move != 'p' && state.move != 'h' && state.move != 'w' && state.move != 's' &&
				 state.move != 'a' && state.move != 'd' && state.move != 'q' && state.move != 'P');

		switch (state.move)
		{
		case 'W': move_up(state);		break;
		case 'w': move_up(state);		break;
		case 'S': move_down(state);		break;
		case 's': move_down(state);		break;
		case 'A': move_left(state);		break;
		case 'a': move_left(state);		break;
		case 'D': move_right(state);	break;
		case 'd': move_right(state);	break;
		case 'O': guess(state);			break;
		case 'o': guess(state);			break;
		case 'H': help(state);			break;
		case 'h': help(state);			break;
		case 'P': solution_display(state);			                                            break;
		case 'p': solution_display(state);			                                            break;
		case ' ': if (state.board[state.y_cursor][state.x_cursor] == 'Y')beam(state);			break;
		}

	} while (state.move != 'q' && state.move != 'Q' && (state.move != 'K' || state.ending_game == false) && (state.move != 'k' || state.ending_game == false) && state.move != 'P' && state.move != 'p');

	if (state.move == 'Q' || state.move == 'q')
	{
		print_menu();
		difficulty_choosing();
	}
	else if (state.move == 'K' || state.move == 'k')
	{
		end(state);
	}
}

void move_up(Game_Data& state)
{
	bool whitespace = false;
		if (state.y_cursor > 0)
		{
			if (state.y_cursor == 1 && (state.x_cursor == 0 || state.x_cursor == state.board_size + 1)){}
			else
			{
				moving_check(state, whitespace);
				if (whitespace == true)
				{
					state.board[state.y_cursor][state.x_cursor] = ' ';
				}
				state.y_cursor--;
			}
		}
	print_board(state);
}

void move_down(Game_Data& state)
{
	bool whitespace = false;
	if (state.y_cursor < state.board_size+1)
	{
		if (state.y_cursor == state.board_size && (state.x_cursor == 0 || state.x_cursor == state.board_size + 1)) {}
		else
		{
			moving_check(state, whitespace);
			if (whitespace == true)
			{
				state.board[state.y_cursor][state.x_cursor] = ' ';
			}
			state.y_cursor += 1;
		}

	}
	print_board(state);
}

void move_left(Game_Data& state)
{
	bool whitespace = false;
	if (state.x_cursor > 0)
	{
		if (state.x_cursor == 1 && (state.y_cursor == 0 || state.y_cursor == state.board_size + 1)) {}
		else
		{
			moving_check(state, whitespace);
			if (whitespace == true)
			{
				state.board[state.y_cursor][state.x_cursor] = ' ';
			}
			state.x_cursor += -1;
		}

	}
	print_board(state);
}

void move_right(Game_Data& state)
{
	bool whitespace = false;
	if (state.x_cursor < state.board_size+1)
	{
		if (state.x_cursor == state.board_size && (state.y_cursor == 0 || state.y_cursor == state.board_size + 1)) {}
		else
		{
			moving_check(state, whitespace);
			if (whitespace == true)
			{
				state.board[state.y_cursor][state.x_cursor] = ' ';
			}
			state.x_cursor += 1;
		}

	}
	print_board(state);
}

void difficulty_choosing()
{
	char difficulty;
	bool difficulty_bool = false;
	do
	{
		cin >> difficulty;
		if (difficulty != '1' && difficulty != '2' && difficulty != '3' && difficulty != '4' && difficulty != '5' && difficulty != '6')
		{
			cout << "There is no such level. Please try again: ";
		}
		else
		{
			difficulty_bool = true;
		}

	} while (!difficulty_bool);

	if (difficulty == '1')
	{
		Game_Data state = initialization(3, 5);
		print_board(state);
		moving(state);
	}
	else if (difficulty == '2')
	{
		Game_Data state = initialization(6, 5);
		print_board(state);
		moving(state);
	}
	else if (difficulty == '3')
	{
		Game_Data state = initialization(5, 8);
		print_board(state);
		moving(state);
	}
	else if (difficulty == '4')
	{
		Game_Data state = initialization(8, 10);
		print_board(state);
		moving(state);
	}
	else if (difficulty == '5')
	{
		Game_Data state = initialization(12, 10);
		print_board(state);
		moving(state);
	}
	else
		exit(0);
}

void print_menu()
{	
	system("cls");
	cout << "     BLACK BOX";
    cout << "\n\n";

    cout << "Welcome to the Black Box game!\n";
    cout << "Game author: Karol Puczynski\n\n";
    cout << "Choose a difficulty level:\n";
    cout << "1. Very easy (5x5 board with three atoms)\n";
    cout << "2. Easy (5x5 board with six atoms)\n";
    cout << "3. Medium (8x8 board with five atoms)\n";
    cout << "4. Hard (10x10 board with eight atoms)\n";
    cout << "5. Very hard (10x10 board with twelve atoms)\n";
    cout << "6. Exit game\n";
    cout << "Which level do you choose? ";

}

int main()
{
	system("cls");
	srand(time(NULL));

	print_menu();
	difficulty_choosing();

	return 0;
}


