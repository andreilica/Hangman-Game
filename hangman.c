#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WND_ROW 33  // Numarul de randuri ale ferestrei
#define WND_COL 120 // Numarul de coloane ale ferestrei
#define TRUE 1

int find (char* string, char* hidden, char* mistake, int character);
void new_game (WINDOW *wnd, char *string, int* flag);
void choice (WINDOW *wnd, int* flag, char* string);
void menu (WINDOW *wnd, int* flag, char* string);
void hidden_make (char* string, char* hidden);
void print_hangman (WINDOW *wnd, int lose);
void print_win (WINDOW *wnd);
void print_lose (WINDOW *wnd);
char *edit_string (char* input);


/* Functie care este folosita pentru a incepe un nou joc */

void new_game (WINDOW *wnd, char *string, int *flag)
{
	int found, win = 0, lose = 0, i;
	unsigned int length;
	length = strlen (string);
	*flag = 0;
	int character, big_character;
	char hidden[50] = "", mistake[50] = "", mistake_add[4] = "";
	time_t current_time;
	char* c_time_string;
    /* Initializare screen nou */

	werase (wnd);
	noecho ();
	keypad (wnd, TRUE);
	curs_set (FALSE);
	wattron (wnd,COLOR_PAIR(2));
	box (wnd, 0, 0);
	wattroff (wnd,COLOR_PAIR(2));
	current_time = time (NULL);
	c_time_string = ctime (&current_time);
	c_time_string[strlen(c_time_string) - 1] = 0;
	/* Desenare ecran joc */

	
	
	mvwprintw(wnd, 3, 1, "                        ##  ##  ######  ####                   _______Greseli_______");
	mvwprintw(wnd, 4, 1, "                        ##  ##  ##  ##  ## ##                 |                     |");
	mvwprintw(wnd, 5, 1, "                        ##  ##  ######  #####                 |                     |");
	mvwprintw(wnd, 6, 1, "                        ##  ##  ##      ## ##                 |_____________________|");
	mvwprintw(wnd, 7, 1, "                        ######  ##      ####");
	mvwprintw(wnd, 10, 1, "                                                                              _________________");
	mvwprintw(wnd, 11, 1, "                                                                             | _______________ |");
	mvwprintw(wnd, 12, 1, "                                                                             ||               ||");
	mvwprintw(wnd, 13, 1, "                                                                             ||");
	mvwprintw(wnd, 14, 1, "                                                                             ||");
	mvwprintw(wnd, 15, 1, "                                                                             ||");
	mvwprintw(wnd, 16, 1, "                                                                             ||");
	mvwprintw(wnd, 17, 1, "                                                                             ||");
	mvwprintw(wnd, 18, 1, "     __________________________________________________                      ||");
	mvwprintw(wnd, 19, 1, "    |                                                  |                     ||");
	mvwprintw(wnd, 20, 1, "    |  Introduceti un caracter de la tastatura pentru  |                     ||");
	mvwprintw(wnd, 21, 1, "    |  a fi verificat in expresie.                     |                     ||");
	mvwprintw(wnd, 22, 1, "    |                                                  |                     ||");
	mvwprintw(wnd, 23, 1, "    |  Apasati Q pentru revenire la meniul principal.  |                     ||");
	mvwprintw(wnd, 24, 1, "    |                                                  |          ___________||___________ ");
	mvwprintw(wnd, 25, 1, "    |__________________________________________________|         /________________________\\");
	mvwprintw(wnd, 28, 1, "______________________________________________________________________________________________________________________");
	wattron(wnd,COLOR_PAIR(1));
	mvwprintw(wnd, 30, 1, "       Hangman Game                                                          Date & Time:  %s", c_time_string);
	wattroff(wnd,COLOR_PAIR(1));

	hidden_make (string, hidden);

	while(TRUE)
	{
		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		c_time_string[strlen(c_time_string) - 1] = 0;
		wattron(wnd,COLOR_PAIR(1));
		mvwprintw(wnd, 30, 1, "       Hangman Game                                                          Date & Time:  %s", c_time_string);
		wattroff(wnd,COLOR_PAIR(1));

		mvwprintw(wnd, 13, 15, "%s", hidden);
		print_hangman(wnd, lose);
		mvwprintw(wnd, 5, 66, "%s", mistake);
		wrefresh(wnd);

		if(win == 1)
		{
			sleep(3);
			print_win(wnd);
			break;
		}
		else
			if(lose == 6)
			{
				sleep(3);
				print_lose(wnd);
				break;
			}
		
		character = wgetch (wnd);

		if(character == 'Q')
		{
			*flag = 1;
			break;
		}
		
		if(isprint(character) != 0)
		{
			big_character = toupper(character);
			character = tolower(character);
			found = find(string, hidden, mistake, character);
			if (found != 2)
			{
				if(found == 1)
				{
					win = 1;
					for(i = 0; i < length; i++)
						if(hidden[i] == '_')
							win = 0;
				}
				else
				{
					lose++;
					if (isalpha(character) != 0)
						mistake_add[0] = big_character;
					else
						mistake_add[0] = character;
					mistake_add[1] = ',';
					mistake_add[2] = ' ';
					mistake_add[3] = '\0';
					strcat(mistake, mistake_add);
				}
			}
		}
		
	}
	
}
/* Functie folosita pentru a reveni la jocul curent */

void resume_game()
{

}

/* Functie folosita pentru a face alegerea optiunilor din meniu */

void choice (WINDOW *wnd, int* flag, char* string)
{
	int i, y = 3;
	char menu[4][15] = { "New Game", "Resume Game", "Quit" };
	char line[15];
	int character;
	i = y + 3;

	while (TRUE)
	{
		character = wgetch(wnd);
		sprintf (line,"%s", menu[i - (y + 3)]);
		mvwprintw (wnd, i, (WND_COL - strlen(line)) / 2, "%s", line);
		switch (character)
		{
			case KEY_UP:
				i--;
				if (i < (y + 3))
					i = y + 5;
				if(*flag == 0)
					if(i == y + 4)
						i--;
				break;
			case KEY_DOWN:
				i++;
				if (i > (y + 5))
					i = y + 3;
				if(*flag == 0)
					if(i == y + 4)
						i++;	
				break;
		}
    	wattron (wnd, COLOR_PAIR(3));
		sprintf (line,"%s",menu[i - (y + 3)]);
		mvwprintw (wnd, i, (WND_COL - strlen(line)) / 2, "%s", line);
    	wattroff (wnd, COLOR_PAIR(3));
    	if (character == 10)
    	{
    		if (i == y + 3)
    		{
    			new_game(wnd, string, flag);
    			break;
    		}
    		else
    			if (i == y + 4)
    			{
    				resume_game();
    				break;
    			}
    			else
    				if (i == y + 5)
    				{
    					werase (wnd);
    					delwin (wnd);
    					endwin ();
    					exit(0);
    				}
    	}
	}
}

/*Functie folosita pentru a crea meniul jocului */

void menu (WINDOW *wnd, int* flag, char* string)
{
	int i, y = 3;
	char menu[4][15] = { "New Game", "Resume Game", "Quit" };
	char line[15];

	while(TRUE)
	{
		werase (wnd);
		noecho ();
		keypad (wnd, TRUE);
		wattron(wnd,COLOR_PAIR(2));
		box(wnd, 0, 0);
		wattroff(wnd,COLOR_PAIR(2));      
		mvwprintw (wnd, y + 8, (WND_COL - 82) / 2, "                                                   _________________________");
		mvwprintw (wnd, y + 9, (WND_COL - 82) / 2, "                                                  //                      ||");
		mvwprintw (wnd, y + 10, (WND_COL - 82) / 2, "						    ||                       ||");
		mvwprintw (wnd, y + 11, (WND_COL - 82) / 2, "                                                 ||                       ||");
		mvwprintw (wnd, y + 12, (WND_COL - 82) / 2, "                                                                       ########");
		mvwprintw (wnd, y + 13, (WND_COL - 82) / 2, "$$   $$    $$$    $$   $$   $$$$$   $$   $$    $$$    $$   $$         ## @  @ ##");
		mvwprintw (wnd, y + 14, (WND_COL - 82) / 2, "$$   $$   $$ $$   $$$  $$  $$   $$  $$$ $$$   $$ $$   $$$  $$        ##        ##");
		mvwprintw (wnd, y + 15, (WND_COL - 82) / 2, "$$$$$$$  $$$$$$$  $$ $ $$  $$       $$ $ $$  $$$$$$$  $$ $ $$        ##  \\__/  ##");
		mvwprintw (wnd, y + 16, (WND_COL - 82) / 2, "$$   $$  $$   $$  $$  $$$  $$  $$$  $$   $$  $$   $$  $$  $$$         ##      ##");
		mvwprintw (wnd, y + 17, (WND_COL - 82) / 2, "$$   $$  $$   $$  $$   $$   $$$$$   $$   $$  $$   $$  $$   $$          ########");
		mvwprintw (wnd, y + 18, (WND_COL - 82) / 2, "                                                 ||");
		mvwprintw (wnd, y + 19, (WND_COL - 82) / 2, "                                                 ||");
		mvwprintw (wnd, y + 20, (WND_COL - 82) / 2, "                                        _________||_________");
		mvwprintw (wnd, y + 21, (WND_COL - 82) / 2, "                                       /____________________\\ ");      

		for (i = y + 3; i < y + 6; i++)
		{
			if (i == y + 3)
	    		wattron (wnd, COLOR_PAIR(3));
	    	else
	    		wattroff (wnd, COLOR_PAIR(3));
			sprintf (line,"%s",menu[i - (y + 3)]);
			mvwprintw (wnd, i, (WND_COL - strlen(line)) / 2, "%s", line);
		}

		choice (wnd, flag, string);
	}
}

/* Functie folosita pentru a crea sirul ascuns folosit */

void hidden_make (char* string, char* hidden)
{
	int i, j, letters = 0;
	unsigned int length;
	length = strlen (string);

	for (i = 0; i < length; i++)
		if (string[i] != ' ')
			hidden[i] = '_';
		else
			hidden[i] = ' ';	

	hidden[length] = '\0';

	if(length >= 3)
	{
		i = 0;
		if(string[i + 1] == ' ')
			hidden[i] = '_';
		else
		{
			hidden[i] = toupper(string[i]);
			for(j = 0; j < length; j++)
			{
				if(string[i] == string[j])
					hidden[j] = toupper(string[i]);
			}
		}
		i = length - 1;

		if (string[i - 1] == ' ' || string[i - 2] == ' ')
			hidden[i] = '_';
		else
		{
			hidden[i] = toupper(string[i]);
			for(j = 0; j < length; j++)
				if (string[i] == string[j])
					hidden[j] = toupper(string[i]);
		}

		for(i = 1; i < length - 1; i++)
			if(string[i] == ' ')
			{
				if(string[i + 2] != ' ')
				{
					hidden[i + 1] = toupper(string[i + 1]);
					for (j = 0; j < length; j++)
						if(string[i + 1] == string[j])
							hidden[j] = toupper(string[i + 1]);
				}
				else
					hidden[i + 1] = '_';

				letters = 0;
				j = i - 1; 

				while(string[j] != ' ' && i != 0)
				{
					letters++;
					j--;
				}

				if(letters  > 2)
				{
					hidden[i - 1] = toupper(string[i - 1]);
					for(j = 0; j < length; j++)
						if(string[i - 1] == string[j])
							hidden[j] = toupper(string[i - 1]);
				}
			}
	}

	if (length == 2)
	{
		hidden[0] = toupper(string[0]);
		if(string[0] == string[1])
			hidden[1] = toupper(string[1]);
	}
}

/* Functie folosita pentru a cauta caracterul introdus printre sirurile existente */

int find (char* string, char* hidden, char* mistake, int character)
{
	int i;
	int big_character;
	big_character = toupper(character);
	unsigned int length;
	length = strlen (string);
	if (strchr(string, character))
		if(strchr(hidden, big_character) == 0)
		{
			for(i = 0; i < length; i++)
				if(string[i] == character)
					hidden[i] = big_character;
			return 1;
		}
		else
			return 2;
	else
		if(strchr(mistake, big_character))
			return 2;
		else
			return 3;
}

/* Functie folosita pentru a afisa desenul omului spanzurat */

void print_hangman (WINDOW *wnd, int lose)
{
	if(lose == 1)
	{
		wattron(wnd, COLOR_PAIR(4));
		mvwprintw(wnd, 12, 93, "__"); 
		mvwprintw(wnd, 12, 97, "__");
		mvwprintw(wnd, 13, 92, "/      \\");
		mvwprintw(wnd, 14, 91, "/  ^  ^  \\");
		mvwprintw(wnd, 15, 91, "\\___  ___/");
		wattroff(wnd, COLOR_PAIR(4));
	}
	if(lose == 2)
	{
		wattron(wnd, COLOR_PAIR(4));
		mvwprintw(wnd, 16, 95, "||");
		mvwprintw(wnd, 17, 95, "||");
		mvwprintw(wnd, 18, 95, "||");
		mvwprintw(wnd, 19, 95, "||");
		mvwprintw(wnd, 20, 95, "||");
		wattroff(wnd, COLOR_PAIR(4));
	}
	if(lose == 3)
	{
		wattron(wnd, COLOR_PAIR(4));
		mvwprintw(wnd, 16, 90, "\\\\");
		mvwprintw(wnd, 17, 91, "\\\\__");
		wattroff(wnd, COLOR_PAIR(4));
	}
	if(lose == 4)
	{
		wattron(wnd, COLOR_PAIR(4));
		mvwprintw(wnd, 16, 100, "//");
		mvwprintw(wnd, 17, 97, "__//");
		wattroff(wnd, COLOR_PAIR(4));
	}
	if(lose == 5)
	{
		wattron(wnd, COLOR_PAIR(4));
		mvwprintw(wnd, 21, 94, "//");
		mvwprintw(wnd, 22, 93, "//");
		mvwprintw(wnd, 23, 92, "//");
		wattroff(wnd, COLOR_PAIR(4));
	}
	if(lose == 6)
	{
		wattron(wnd, COLOR_PAIR(5));
		mvwprintw(wnd, 14, 94, "X  X");	
		wattroff(wnd, COLOR_PAIR(5));
		wattron(wnd, COLOR_PAIR(4));
		mvwprintw(wnd, 21, 96, "\\\\");
		mvwprintw(wnd, 22, 97, "\\\\");
		mvwprintw(wnd, 23, 98, "\\\\");
		wattroff(wnd, COLOR_PAIR(4));
	}
}

/* Functie folosita pentru a afisa un mesaj corespunzator in cazul in care jocul a fost pierdut */

void print_lose(WINDOW *wnd)
{
	int character;
	werase(wnd);
	wattron(wnd, COLOR_PAIR(2));
	box(wnd, 0, 0);
	wattroff(wnd, COLOR_PAIR(2));
	mvwprintw(wnd, 3, (WND_COL - 19) / 2, " _________________ ");
	mvwprintw(wnd, 4, (WND_COL - 19) / 2, "/                 \\");
	mvwprintw(wnd, 5, (WND_COL - 19) / 2, "|     X     X     |");
	mvwprintw(wnd, 6, (WND_COL - 19) / 2, "|                 | ");
	mvwprintw(wnd, 7, (WND_COL - 19) / 2, "|       ___       |");
	mvwprintw(wnd, 8, (WND_COL - 19) / 2, "|      /   \\      |");
	mvwprintw(wnd, 9, (WND_COL - 19) / 2, "\\_________________/");
	mvwprintw(wnd,(WND_ROW - 5) / 2    , (WND_COL - 71) / 2, " #####     ###    ##   ##  #######    #####   ##   ##  #######  ###### ");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 1, (WND_COL - 71) / 2, "##   ##   ## ##   ### ###  ##        ##   ##  ##   ##  ##       ##  ## ");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 2, (WND_COL - 71) / 2, "##       #######  ## # ##  #####     ##   ##  ##   ##  #####    #####  ");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 3, (WND_COL - 71) / 2, "##  ###  ##   ##  ##   ##  ##        ##   ##   ## ##   ##       ##  ## ");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 4, (WND_COL - 71) / 2, " #####   ##   ##  ##   ##  #######    #####     ###    #######  ##   ##");
	mvwprintw(wnd, 22, (WND_COL - 46) / 2, "Apasati Q pentru a reveni la meniul principal");

	while (TRUE)
	{
		character = wgetch (wnd);
		if(character == 'Q')
			break;
	}	
}

/* Functie folosita pentru a afisa un mesaj corespunzator in cazul in care jocul a fost castigat */

void print_win(WINDOW *wnd)
{
	int character;
	werase(wnd);
	wattron(wnd, COLOR_PAIR(2));
	box(wnd, 0, 0);
	wattroff(wnd, COLOR_PAIR(2));
	mvwprintw(wnd, 3, (WND_COL - 19) / 2, " _________________ ");
	mvwprintw(wnd, 4, (WND_COL - 19) / 2, "/                 \\");
	mvwprintw(wnd, 5, (WND_COL - 19) / 2, "|     ^     ^     |");
	mvwprintw(wnd, 6, (WND_COL - 19) / 2, "|                 | ");
	mvwprintw(wnd, 7, (WND_COL - 19) / 2, "|                 |");
	mvwprintw(wnd, 8, (WND_COL - 19) / 2, "|      \\___/      |");
	mvwprintw(wnd, 9, (WND_COL - 19) / 2, "\\_________________/");
	mvwprintw(wnd,(WND_ROW - 5) / 2    , (WND_COL - 63) / 2, " #####     ###    ##   ##  #######    ##   ##   #####   ##   ##");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 1, (WND_COL - 63) / 2, "##   ##   ## ##   ### ###  ##         ##   ##  ##   ##  ###  ##");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 2, (WND_COL - 63) / 2, "##       #######  ## # ##  #####      ##   ##  ##   ##  ## # ##");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 3, (WND_COL - 63) / 2, "##  ###  ##   ##  ##   ##  ##         ## # ##  ##   ##  ##  ###");
	mvwprintw(wnd,(WND_ROW - 5) / 2 + 4, (WND_COL - 63) / 2, " #####   ##   ##  ##   ##  #######     ## ##    #####   ##   ##");
	mvwprintw(wnd, 22, (WND_COL - 46) / 2, "Apasati Q pentru a reveni la meniul principal");

	while (TRUE)
	{
		character = wgetch (wnd);
		if(character == 'Q')
			break;
	}
	
}

/* Functie folosita pentru a edita string-urile gasit in fisierele de input */

char *edit_string(char* input)
{
	int i;
	unsigned int length = strlen(input);
	char* dest = input;
    char* src = input;

    while(*src)
    {
        if (!isprint(*src)) 
        { 
        	src++; 
        	continue; 
        }

    	*dest++ = *src++;
    }

    *dest = '\0';

    for(i = 0; i < length; i++)
    {
		if(isalpha(input[i]))
			input[i] = tolower(input[i]);
    }

    return input;
}

int main(int argc, char const *argv[])
{
	FILE *src;
	char string[5000], dest[5000];
	int flag = 0, i;

	/* Verificare daca s-au introdus argumente in linia de comanda */

	if(argc == 1)
	{
		fprintf(stderr, "[Eroare]: Nu s-au dat argumente de comanda.\n");
		return 1;
	}

	/* Functie folosita pentru a verifica daca fisierele de input sunt valide */

	for (i = 1; i < argc; ++i) 
    {
    	src = fopen(argv[i], "r");
    	if (!src)
    	{
        	fprintf(stderr, "[Eroare]: Fisierul %s nu poate fi deschis.\n", argv[i]);
        	return 1;
        }
        else
        	while (fgets(string, 5000, src) != NULL)
	    	{
               strcpy (dest, string);
        	}

    	fclose (src);
    }
   
	system ("printf '\e[8;33;120t'"); // Functie folosita pentru a redimensiona terminalul exact cat jocul 
	WINDOW *wnd = initscr();
	wnd = newwin (WND_ROW,WND_COL, 0, 0);
	noecho ();
	curs_set (FALSE);

	/* Declararea perechilor de culori */

	start_color ();
	init_pair (1, COLOR_GREEN, COLOR_BLACK);
	init_pair (2, COLOR_RED, COLOR_BLACK);
	init_pair (3, COLOR_WHITE, COLOR_GREEN);
	init_pair (4, COLOR_GREEN, COLOR_BLACK);
	init_pair (5, COLOR_YELLOW, COLOR_BLACK);        
	menu (wnd, &flag, edit_string(dest));
	werase (wnd);
	delwin (wnd);
	endwin ();

	return 0;
}	