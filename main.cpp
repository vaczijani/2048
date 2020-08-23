#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

#define BOARD_EDGE 4
#define BOARD_SIZE (BOARD_EDGE*BOARD_EDGE)
#define WINNING 2048
#define EMPTY 0

uint32_t board[BOARD_EDGE][BOARD_EDGE];
bool combined[BOARD_EDGE][BOARD_EDGE];


bool is_won()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		if (board[0][i] == WINNING)
			return true;
	}
	return false;
}


bool is_lost()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		if (board[0][i] == EMPTY)
			return false;
	}
	return true;
}


int first_empty_pos()
{
	int i = 0;
	while (true)
	{
		if (board[0][i] == EMPTY)
			return i;
		i = (i+1) % BOARD_SIZE;
	}
}


int next_empty_pos(int lastpos)
{
	int i = lastpos;
	while (true)
	{
		i = (i+1) % BOARD_SIZE;
		if (board[0][i] == EMPTY)
			return i;
	}
}


int random_steps()
{
	return rand() % BOARD_SIZE;
}


uint32_t random_two_or_four()
{
	return (rand() % 100) > 49 ? 2 : 4;
}


void add_number(int steps, uint32_t number)
{
	//int steps = rand() % BOARD_SIZE;
	//uint32_t number = (rand() % 100) > 49 ? 2 : 4;
	int pos = first_empty_pos();
	for (int i = 0; i < steps; ++i)
	{
		pos = next_empty_pos(pos);
	}
	board[0][pos] = number;
}


void print_board()
{
	printf("\n\n");
	for (int row = 0; row < BOARD_EDGE; ++row)
	{
		for (int col = 0; col < BOARD_EDGE; ++col)
		{
			if (board[row][col] == EMPTY)
				printf("%5s", ".");
			else
				printf("%5u", board[row][col]);
		}
		printf("\n\n");
	}
}


void empty_combined()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
		combined[0][i] = false;
}



void tilt(int outer_start, int outer_end, int outer_step, int inner_end, int inner_step, int opposite_end)
{
	empty_combined();
	for (int outer = outer_start; outer != outer_end; outer += outer_step)
	{
		for (int inner = 0; inner != inner_end; inner += inner_step)
		{
			if (board[0][inner+outer] != EMPTY)
			{
				for (int i = outer - outer_step; i != opposite_end; i -= outer_step)
				{
					int whereto = i + inner;
					int wherefrom = i + inner + outer_step;
					if (board[0][whereto] != EMPTY)
					{
						if (board[0][whereto] == board[0][wherefrom] && !combined[0][whereto])
						{
							board[0][whereto] += board[0][wherefrom];
							combined[0][whereto] = true;
							board[0][wherefrom] = EMPTY;
						}
						break;
					}
					else
					{
						board[0][whereto] = board[0][wherefrom];
						board[0][wherefrom] = EMPTY;
					}					
				}
			}
		}
	}
}

void tilt_left()
{
	tilt(1, BOARD_EDGE, 1, BOARD_EDGE*BOARD_EDGE, BOARD_EDGE, -1);
}

void tilt_right()
{
	tilt(BOARD_EDGE-2, -1, -1, BOARD_EDGE*BOARD_EDGE, BOARD_EDGE, BOARD_EDGE);
}

void tilt_up()
{
	tilt(BOARD_EDGE, BOARD_EDGE*BOARD_EDGE, BOARD_EDGE, BOARD_EDGE, 1, -BOARD_EDGE);
}

void tilt_down()
{
	tilt((BOARD_EDGE-2)*BOARD_EDGE, -BOARD_EDGE, -BOARD_EDGE, BOARD_EDGE, 1, BOARD_EDGE*BOARD_EDGE);
}

void test()
{
	add_number(0, 2);
	add_number(3, 2);
	print_board();
	//tilt(1, BOARD_EDGE, 1, BOARD_EDGE*BOARD_EDGE, BOARD_EDGE, -1); // = tilt_left
	//tilt(BOARD_EDGE-2, -1, -1, BOARD_EDGE*BOARD_EDGE, BOARD_EDGE, BOARD_EDGE); // = tilt_right
	//tilt(BOARD_EDGE, BOARD_EDGE*BOARD_EDGE, BOARD_EDGE, BOARD_EDGE, 1, -1); // = tilt_up
	//tilt((BOARD_EDGE-2)*BOARD_EDGE, -BOARD_EDGE, -BOARD_EDGE, BOARD_EDGE, 1, BOARD_EDGE*BOARD_EDGE); // = tilt_down
	print_board();
}

int main()
{
	//test();
	//return 0;

	printf("Instructions:\n2048 game\ntilt board keys: j:left, l:right, i:up, k:down\nexit:q\nreaching 2048 wins\n");
	srand(time(NULL));
	add_number(random_steps(), random_two_or_four());
	add_number(random_steps(), random_two_or_four());
	print_board();
	int step = 0;
	while (!is_won() && !is_lost())
	{
		printf("Step #%d is ", ++step);
		int key = getch();
		if (key == 'q' || key == 'Q')
		{
			printf("Quit\n");
			break;
		}
		switch (key)
		{
		case 'j':
		case 'J':
			printf("left");
			tilt_left();
			break;
		case 'l':
		case 'L':
			printf("right");
			tilt_right();
			break;
		case 'i':
		case 'I':
			printf("up");
			tilt_up();
			break;
		case 'k':
		case 'K':
			printf("down");
			tilt_down();
			break;
		default:
			--step;
			continue;
		}
		add_number(random_steps(), random_two_or_four());
		print_board();
	}
	if (is_won())
		printf("You won!\n");
	if (is_lost())
		printf("You lost!\n");
}
