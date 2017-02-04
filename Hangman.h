#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
using namespace std;
void gotoXY(int, int);					//For moving control on console
void printBox(int, int, int, int);		//For printing box as border
void loading();							//For displaying loading
void clearScreen();						//For clearing screen
void hangmanGame();							//For displaying menu
void startGame();						//For selecting difficulty and starting game
void playGame(int);						//For starting and playing game
char* getWord(int);						//For getting diffirent words
void encryptWord(char*, char*);			//For converting word into astericks
void hanging(int);						//For displaying man hanging
bool checkGuess(char*, char*, char);	//For checking guess
bool checkWord(char*, char*);			//For checking word
void instructions();					//For displaying instructions
void about();							//For displaying about
										//Getting console output handle
HANDLE H_Console = GetStdHandle(STD_OUTPUT_HANDLE);
void gotoXY(int X, int Y) {
	COORD position;
	position.X = X;
	position.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
void printBox(int X, int Y, int width, int length) {
	/*
	U_L_C = Upper Left Corner
	U_R_C = Upper Right Corner
	L_L_C = Lower Left Corner
	L_R_C = Lower Right Corner
	H_L	= Horizontal Line
	V_L = Vertical Line
	*/
	char U_L_C = 201, U_R_C = 187, L_L_C = 200, L_R_C = 188;
	char H_L = 205, V_L = 186;
	width -= 2;	length -= 2;
	//Printing horizontal line
	gotoXY(X, Y);
	cout << U_L_C;
	for (int i = 0; i < width; i++)
		cout << H_L;
	cout << U_R_C;
	//Printing vertical lines
	for (int i = 0; i < length; i++) {
		gotoXY(X, Y + (i + 1));
		cout << V_L;
		gotoXY(X + (width + 1), Y + (i + 1));
		cout << V_L;
	}
	//Printing horizontal line
	gotoXY(X, Y + length);
	cout << L_L_C;
	for (int i = 0; i < width; i++)
		cout << H_L;
	cout << L_R_C;
}
void loading() {
	/*
	F_B = Filled Box
	H_B = Hollow Box
	*/
	char F_B = 254, H_B = 207;
	gotoXY(32, 12);
	cout << "Loading HANGMAN!";
	//Displaying loading
	for (int i = 0; i < 6; i++) {
		Sleep(150);	gotoXY(36, 14);
		cout << F_B << "  " << H_B << "  " << H_B;
		Sleep(150);	gotoXY(36, 14);
		cout << H_B << "  " << F_B << "  " << H_B;
		Sleep(150);	gotoXY(36, 14);
		cout << H_B << "  " << H_B << "  " << F_B;
	}
}
void clearScreen() {
	//Clearing screen from point (3,3) to (77,27)
	for (int i = 0; i < 24; i++) {
		gotoXY(3, 3 + i);
		for (int j = 0; j < 74; j++)
			cout << " ";
	}
}
void hangmanGame() {
	clearScreen();
	char logo[7][75];
	strcpy(logo[0], "HH    HH     AA     NN    NN     GGG    MM          MM     AA     NN    NN");
	strcpy(logo[1], "HH    HH   AA  AA   NNN   NN   GG   GG  MMM        MMM   AA  AA   NNN   NN");
	strcpy(logo[2], "HH    HH  AA    AA  NNNN  NN  GG        MMMM      MMMM  AA    AA  NNNN  NN");
	strcpy(logo[3], "HHHHHHHH  AAAAAAAA  NN NN NN  GG        MM MM    MM MM  AAAAAAAA  NN NN NN");
	strcpy(logo[4], "HH    HH  AA    AA  NN  NNNN  GG   GGG  MM  MM  MM  MM  AA    AA  NN  NNNN");
	strcpy(logo[5], "HH    HH  AA    AA  NN   NNN   GG   GG  MM   MMMM   MM  AA    AA  NN   NNN");
	strcpy(logo[6], "HH    HH  AA    AA  NN    NN     GGG    MM    MM    MM  AA    AA  NN    NN");
	//Printing logo
	for (int i = 0; i < 7; i++) {
		SetConsoleTextAttribute(H_Console, 12);
		gotoXY(3, 5 + i);	puts(logo[i]);
	}
	SetConsoleTextAttribute(H_Console, 9);
	printBox(29, 15, 21, 10);
	gotoXY(31, 16);	cout << "(1)->  Play game ";
	gotoXY(31, 18);	cout << "(2)-> How to play";
	gotoXY(31, 20);	cout << "(3)->   Credits  ";
	gotoXY(31, 22);	cout << "(4)->    Exit    ";
	//Selecting option
	while (true) {
		gotoXY(0, 29);	char ch = getch();
		if (ch == '1')
			startGame();
		else if (ch == '2')
			instructions();
		else if (ch == '3')
			about();
		else if (ch == '4')
			exit(0);
		else
			continue;
	}
}
void startGame()
{
	//Providing time as seed to rand function
	srand(time(0));
	gotoXY(31, 16);	cout << "Difficulty level!";
	gotoXY(31, 18);	cout << "(1)->    EASY    ";
	gotoXY(31, 20);	cout << "(2)->   MEDIUM   ";
	gotoXY(31, 22);	cout << "(3)->    HARD    ";
	int difficulty;
	//Selecting difficulty
	while (true) {
		gotoXY(0, 29); char ch = getch();
		if (ch == '1' || ch == '2' || ch == '3') {
			difficulty = int(ch) - 48;
			break;
		}
		else
			continue;
	}
	//Starting game
	while (true) {
		playGame(difficulty);
		//Waiting for 'y' or 'n'
		int check = 0;
		while (true) {
			gotoXY(15, 22);	cout << "Play again :: ";
			char ch = getche();
			if (ch == 'y' || ch == 'Y') {
				check = 1;
				break;
			}
			if (ch == 'n' || ch == 'N')
				break;
		}
		if (check == 1)	//If want to play again
			continue;
		else	//If don't want to play again
			break;
	}
	hangmanGame();
}
void playGame(int difficulty)
{
	clearScreen();
	int index, lifes = 0, check = 0;
	//Generating index according to difficulty
	if (difficulty == 1)
		index = rand() % 10;
	else if (difficulty == 2)
		index = (rand() % 10) + 10;
	else
		index = (rand() % 10) + 20;
	char guess[8], input, *word;
	//Getting and storing word
	word = getWord(index);
	//Converting word into astericks
	encryptWord(word, guess);
	printBox(10, 3, 19, 4);  //Box 1
	printBox(56, 3, 14, 4);  //Box 2
	printBox(46, 9, 24, 17); //Box 3
	printBox(10, 9, 25, 9);  //Box 4
	while (true) {
		//Printing word to guess
		gotoXY(12, 4); cout << "WORD :: ";
		puts(guess);
		//Printing remaining lifes
		gotoXY(58, 4); cout << "LIFES :: " << (7 - lifes);
		//Displaying man hanging
		hanging(lifes);
		if (lifes == 7)	//If lifes are finished
			break;
		if (checkWord(word, guess)) {	//If word has been guessed
			check = 1;
			break;
		}
		//Taking guess
		gotoXY(12, 11);	cout << "Enter your guess :: ";
		input = getche();
		gotoXY(14, 14);
		if (checkGuess(word, guess, input))	//If guessed character is found
			cout << "You saved a life!";
		else {	//If guessed character isn't found
			cout << "You loose a life!";
			lifes++;
		}
	}
	//Printing result
	printBox(10, 18, 25, 8);
	SetConsoleTextAttribute(H_Console, 12);
	gotoXY(18, 20);
	if (check == 1)
		cout << "!YOU WIN!";
	else
		cout << "!YOU LOSE!";
	SetConsoleTextAttribute(H_Console, 9);
}
char* getWord(int index) {
	static char words[][8] = {
		//3 characters words
		"PEN","EYE","CAR","HEN","MAN","SKY","EAR","EGG","PIN","FAN",
		//5 characters words
		"FIGHT","FLOOR","PLANE","ROBOT","PHONE","HOUSE","ELBOW","NOKIA","WATER","TRAIN",
		//7 characters words
		"BALANCE","TEACHER","STUDENT","CRICKET","SAMSUNG","PROGRAM","EDITING","MESSAGE","HACKING","LECTURE"
	};
	return words[index];
}
void encryptWord(char *word, char *guess) {
	int i = 1;
	//Storing first character of word as it is
	guess[0] = word[0];
	//Storing all other characters of word as astericks
	while (word[i] != '\0') {
		guess[i] = '*';
		i++;
	}
	//Storing NULL at the last index
	guess[i] = '\0';
}
void hanging(int wrong) {
	static char hang[12][20];
	if (wrong == 0) {
		strcpy(hang[0], " _____________    ");
		strcpy(hang[1], " |           |    ");
		strcpy(hang[2], " |           |    ");
		strcpy(hang[3], "             |    ");
		strcpy(hang[4], "             |   ");
		strcpy(hang[5], "             |    ");
		strcpy(hang[6], "             |   ");
		strcpy(hang[7], "             |    ");
		strcpy(hang[8], "             |    ");
		strcpy(hang[9], "             |    ");
		strcpy(hang[10], "             |    ");
		strcpy(hang[11], "         ---------");
	}
	else if (wrong == 1)
		strcpy(hang[3], " O           |    ");
	else if (wrong == 2)
		strcpy(hang[4], " |           |   ");
	else if (wrong == 3)
		strcpy(hang[4], "/|           |   ");
	else if (wrong == 4)
		strcpy(hang[4], "/|\\          |   ");
	else if (wrong == 5)
		strcpy(hang[5], " |           |    ");
	else if (wrong == 6)
		strcpy(hang[6], "/            |   ");
	else
		strcpy(hang[6], "/ \\          |   ");
	//Displaying man hanging
	for (int i = 0; i < 12; i++) {
		if (wrong == 7)
			SetConsoleTextAttribute(H_Console, 12);
		gotoXY(49, 11 + i);	puts(hang[i]);
	}
	if (wrong == 7)
		SetConsoleTextAttribute(H_Console, 9);
}
bool checkGuess(char *word, char *guess, char ch) {
	bool check = false;	int i = 0;
	//Converting character to uppercase
	ch = toupper(ch);
	while (word[i] != '\0') {
		if (word[i] == ch) {	//If character is found in word
								//Stroing character into the guessed word
			guess[i] = word[i];
			check = true;
		}
		i++;
	}
	return check;
}
bool checkWord(char *word, char *guess) {
	int i = 0;
	while (word[i] != '\0') {
		if (word[i] != guess[i])	//If any character didn't matched
			return false;
		i++;
	}
	return true;
}
void instructions() {
	SetConsoleTextAttribute(H_Console, 12);
	char about[19][78];
	strcpy(about[0], "                                :: HANGMAN ::                                ");
	strcpy(about[1], "                                                                             ");
	strcpy(about[2], "                   In this game, you have to guess a word!                   ");
	strcpy(about[3], "             Any word will be given according to the difficulty.             ");
	strcpy(about[4], "                                                                             ");
	strcpy(about[5], "                 You will have 7 lifes for guessing the word                 ");
	strcpy(about[6], "                        If you guessed the word right                        ");
	strcpy(about[7], "                       man will be saved from hanging.                       ");
	strcpy(about[8], "                                                                             ");
	strcpy(about[9], "                But if you're not able to guess the word and,                ");
	strcpy(about[10], "                   lifes finish then person will be hanged                   ");
	strcpy(about[11], "                                                                             ");
	strcpy(about[12], "                     Difficulty levels are as follows :-                     ");
	strcpy(about[13], "                                                                             ");
	strcpy(about[14], "                          EASY :: 3 character words                          ");
	strcpy(about[15], "                                                                             ");
	strcpy(about[16], "                         MEDIUM :: 5 character words                         ");
	strcpy(about[17], "                                                                             ");
	strcpy(about[18], "                          HARD :: 7 character words                          ");
	//printing Instruction
	for (int i = 0; i < 19; i++) {
		gotoXY(1, 5 + i); puts(about[i]);
	}
	gotoXY(0, 29);
	getch();
	hangmanGame();
}
void about() {
	SetConsoleTextAttribute(H_Console, 12);
	char about[23][78];
	strcpy(about[0], "                                                                             ");
	strcpy(about[1], "                                                                             ");
	strcpy(about[2], "                                                                             ");
	strcpy(about[3], "                                                                             ");
	strcpy(about[4], "                                                                             ");
	strcpy(about[5], "                               :: About Me ::                                ");
	strcpy(about[6], "                                                                             ");
	strcpy(about[7], "                    I'm IRTZA MAJID student of BSSE(HONS)                    ");
	strcpy(about[8], "                                (15007065040)                                ");
	strcpy(about[9], "                                                                             ");
	strcpy(about[10], "             Studying in University of Management and Technology             ");
	strcpy(about[11], "                                Sialkot Campus                               ");
	strcpy(about[12], "                                                                             ");
	strcpy(about[13], "                                                                             ");
	strcpy(about[14], "                                                                             ");
	strcpy(about[15], "                                                                             ");
	strcpy(about[16], "                                                                             ");
	strcpy(about[17], "                                                                             ");
	strcpy(about[18], "                                                                             ");
	//Printing about
	for (int i = 0; i < 19; i++) {
		gotoXY(1, 5 + i); puts(about[i]);
	}
	gotoXY(0, 29);
	getch();
	hangmanGame();
}