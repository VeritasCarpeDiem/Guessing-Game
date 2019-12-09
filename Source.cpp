#include <iostream>
#include <string>
#include <windows.h>//for color
#include <conio.h> // library for _kbhit() and _getch() functions
#include <stdlib.h>//for rand function
#include <stdio.h>//for RNG
#include <time.h>//for RNG
#include <iomanip> //setw(), used for spacing 

#pragma comment(lib, "Winmm.lib")//static windows library for playing .wav music files

using namespace std;//using standard namespace

struct Player
{
	//string username; //easier to not use this and rather pass username as local variable
	//int totalGuesses = 0; //totalGuesses that user takes to guess number
	//int randomNumber = 0; //the randomnumber user has to guess in Guessing Game
	//int range = 0;
	//int guesses[30] = { 0 };
	//int answerData[30]; //for reverseguessinggame
	//int gameDifficulty{};
	//int* scoresArr = nullptr;
};


enum class GameDifficulty { Normal = 1, Hard };

enum class GameTypes { GuessingGame = 1, ReverseGuessingGame = 2, Scoreboard = 3, HowToPlay = 4, Exit = 5 };

enum class Colors //strongly typed class of enum colors 
{
	
	//Explanation: The 2 hexadecimal values Foreground_Intensity(0x0008) and Foreground_blue(0x0001) are added(the "|" or operator) and
	//				produces Blue(0x0009). The value 9 is then assigned to Blue in a enum class 

	//(copied from microsoft website)
	BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

void changeTextColor(int color)//function to change text color
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color); //cast the enum to int color
}

//overloaded function
//this allows me to enter a color of type Colors class for code readibility
//ex. changeTextcolor(Colors:: WHITE);
void changeTextColor(Colors color)
{
	changeTextColor((int)color);
}

bool isBackgroundMusicPlaying(bool isplaying)
{
	if (isplaying == true)
	{
		PlaySound("Creative Minds.wav", NULL, SND_ASYNC | SND_LOOP);//music playing on loop using"winn.lib" windows library
		return true;
	}
	else //isplaying==false
	{
		PlaySound(NULL, NULL, NULL); //terminate music from playing
		return false;
	}
}

int getGameDifficulty()
{
	int gameDifficulty = 0;
	GameDifficulty Level = (GameDifficulty)gameDifficulty;

	cout << "Choose game difficulty:\n1. Normal\n2. Hard\n";
	cin >> gameDifficulty;
	return gameDifficulty;
}

void goToXY(int column, int row)//center objects,texts,etc to a certain coordinate
{
	COORD coord;
	coord.X = column;
	coord.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void displayDeadHangMan(); //function prototype

void displayQuestionMark()
{
	const int row = 10;
	const int col = 10;
	int questionmark[row][col] = {
		{0,0,0,0,3,1,1,1,4,0},
		{0,0,0,0,2,0,0,0,2,0},
		{0,0,0,0,2,0,0,0,2,0},
		{0,0,0,0,0,0,0,0,3,0},
		{0,0,0,0,0,0,0,3,0,0},
		{0,0,0,0,0,0,3,0,0,0},
		{0,0,0,0,0,0,2,0,0,0},
		{0,0,0,0,0,0,2,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,5,0,0,0},
	};
	for (int row = 0; row < 10; row++)
	{
		goToXY(80 + col, 5 + row);
		for (int col = 0; col < 10; col++)
		{
			if (questionmark[row][col] == 5)
			{
				cout << char(219);
			}
			else if (questionmark[row][col] == 2)
			{
				cout << char(179);
			}
			else if (questionmark[row][col] == 1)
			{
				cout << char(196);
			}
			else if (questionmark[row][col] == 3)
			{
				cout << char(47);
			}
			else if (questionmark[row][col] == 4)
			{
				cout << char(92);
			}
			else
				cout << " ";
		}
		cout << endl;
	}
}

/*
Menu:
1. Guessing Game
2. Reverse Guessing Game
3. Scoreboard
4. How to Play
5. Exit
*/
void displayMenu(string username)//function to display menu
{
	system("cls");
	displayDeadHangMan();
	displayQuestionMark();
	changeTextColor(Colors::WHITE); //:: is a scope resolution operator which accesses the enum White within the class Colors
	goToXY(50, 1);
	cout << "Welcome to Hangman Guessing Game, ";
	cout << username << endl;
	goToXY(65, 3);
	changeTextColor(Colors::WHITE);
	cout << "Menu:";
	goToXY(50, 4);
	changeTextColor(Colors::BLUE);
	cout << "1. Guessing Game \n";
	goToXY(50, 6);
	changeTextColor(Colors::RED);
	cout << "2. Reverse Guessing Game \n";
	goToXY(50, 8);
	changeTextColor(Colors::YELLOW);
	cout << "3. Top Scores\n";
	goToXY(50, 10);
	changeTextColor(Colors::GREEN);
	cout << "4. How To Play\n";
	goToXY(50, 12);
	changeTextColor(Colors::MAGENTA);
	cout << "5. Exit" << endl;
	goToXY(50, 14);
	changeTextColor(15);
}

string getUsername(string message)
{
	string username;
	cout << message;
	getline(cin, username); //Read the entire line for the username
	return username;
}

int getNumberFromUser(string message)
{
	int number;
	cout << message;
	cin >> number;
	return number;
}

bool isRangeValid(int min, int max, int minAllowedRange) //input validation
{
	int range = max - min + 1;

	if (range < minAllowedRange)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int generateRandomNumberforGuessingGame(int min, int range)
{
	srand((unsigned int)time(NULL));
	int randomNumber = min + rand() % range; //generate a random number inclusive of range of numbers plus min number
	return randomNumber;
}

int calculateAllowedAttemptsForNormal(int range)
{
	return (int)(round((log(range) / log(2))) * 2.5);
}

int calculateAllowedAttemptsForHard(int range)
{
	return (int)(round((log(range) / log(2))) * 1.5);
}

int leastNumberofGuesses(int range)
{
	return (int)round(log(range) / log(2));//shortest number of guesses to guess a number using binary search
	//time complexity of binary search is Olog(n), where n is the range
}

void displayHappyHangMan()//when user wins
{
	const int row = 18;
	const int col = 22;
	int hangMan[row][col] = {
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2},
			{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},
			{2,0,0,0,0,0,2,0,0,3,0,0,0,0,3,0,0,2,0,0,0,2},
			{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},
			{2,0,0,0,0,0,2,0,0,4,0,0,0,0,5,0,0,2,0,0,0,2},
			{2,0,0,0,0,0,2,0,0,1,1,1,1,1,0,0,0,2,0,0,0,2},
			{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},
			{2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,4,0,0,2,0,0,5,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,4,0,2,0,5,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,0,4,2,5,0,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,0,5,0,4,0,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,5,0,0,0,4,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,5,0,0,0,0,0,4,0,0,0,0,0,2},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};//each number in array represents the total number of guesses the user has made
	changeTextColor(Colors::GREEN);
	for (int row = 0; row < 18; row++)
	{
		for (int col = 0; col < 22; col++)
		{
			if (hangMan[row][col] == 1)
			{
				cout << char(196);
			}
			else if (hangMan[row][col] == 2)
			{
				cout << char(179);
			}
			else if (hangMan[row][col] == 3)
			{
				cout << char(219);
			}
			else if (hangMan[row][col] == 4)
			{
				cout << char(92);
			}
			else if (hangMan[row][col] == 5)
			{
				cout << char(47);
			}
			else
				cout << " ";
		}
		cout << endl;
	}
	changeTextColor(Colors::WHITE);
}

void displayDeadHangMan()//when user loses
{
	const int row = 22;
	const int col = 22;
	int hangMan[row][col] = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
			{2,0,2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,2},
			{2,0,2,0,0,4,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2},
			{2,0,2,0,4,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2},
			{2,0,2,4,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2},
			{2,0,2,0,0,0,2,0,3,0,4,0,0,3,0,4,0,2,0,0,0,2},
			{2,0,2,0,0,0,2,0,0,3,0,0,0,0,3,0,0,2,0,0,0,2},
			{2,0,2,0,0,0,2,0,4,0,3,0,0,4,0,3,0,2,0,0,0,2},
			{2,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},
			{2,0,2,0,0,0,2,0,0,1,1,1,1,1,0,0,0,2,0,0,0,2},
			{2,0,2,0,0,0,2,0,4,0,0,0,0,0,3,0,0,2,0,0,0,2},
			{2,0,2,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2},
			{2,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2},
			{2,0,2,0,0,0,1,1,1,1,1,1,2,1,1,1,1,1,1,0,0,2},
			{2,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2},
			{2,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2},
			{2,1,1,1,1,1,1,0,0,0,0,4,0,3,0,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,0,4,0,0,0,3,0,0,0,0,0,0,2},
			{2,0,0,0,0,0,0,0,0,4,0,0,0,0,0,3,0,0,0,0,0,2},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	changeTextColor(Colors::WHITE);
	for (int row = 0; row < 22; row++)
	{
		goToXY(5, row);
		for (int col = 0; col < 22; col++)
		{
			if (hangMan[row][col] == 1)
			{
				cout << char(196);
			}
			else if (hangMan[row][col] == 2)
			{
				cout << char(179);
			}
			else if (hangMan[row][col] == 3)
			{
				cout << char(92);
			}
			else if (hangMan[row][col] == 4)
			{
				cout << char(47);
			}
			else if (hangMan[row][col] == 5)
			{
				cout << char(219);
			}
			else
				cout << " ";
		}
		cout << endl;
	}
	changeTextColor(Colors::WHITE);
}

bool didUserGetLucky(int range, int totalGuesses, int guesses[], int RandomNumber) 
{
	int smallestNumberOfGuesses = leastNumberofGuesses(range);

	if (totalGuesses < smallestNumberOfGuesses && guesses[totalGuesses - 1] == RandomNumber)
	{
		changeTextColor(11);
		cout << "You got lucky! You guessed my number in " << totalGuesses << " guess(es)." << endl << "It should take you at least ";
		cout << smallestNumberOfGuesses << " guesses using binary search" << endl;
		changeTextColor(15);
		system("pause");
		return true;
	}
	else
	{
		return false;
	}
}

bool didUserWin(int RandomNumber, int totalGuesses, int allowedAttempts, int range, int guesses[])
{
	if (didUserGetLucky(range, totalGuesses, guesses, RandomNumber) == true)//when user gets lucky
	{
		return false;
	}
	else if (guesses[totalGuesses - 1] == RandomNumber && totalGuesses < allowedAttempts) //when user wins
	{
		isBackgroundMusicPlaying(false);
		displayHappyHangMan();
		cout << "Congratulations! You guessed my number- It's ";
		changeTextColor(13);
		cout << RandomNumber;
		changeTextColor(15);
		cout << "!" << endl;
		PlaySound("Win EndScreen", NULL, SND_SYNC); //Play win endscreen music
		isBackgroundMusicPlaying(true);//restart background music
		return true;
	}
	//when user loses
	else //when (guess != RandomNumber && totalGuesses == allowedAttempts) 
	{
		isBackgroundMusicPlaying(false);
		displayDeadHangMan();
		cout << "Sorry, you did not guess my number-It's " << RandomNumber << "!" << endl;
		PlaySound("Lose Endscreen", NULL, SND_SYNC); //play lose endscreen music
		isBackgroundMusicPlaying(true);
		return false;
	}
}

void printGuesses(int guesses[], int totalGuesses)
{
	cout << endl << "Here are your " << totalGuesses << " guess(es): ";
	for (int i = 0; i < totalGuesses; i++)//loop through guesses[] to display values of the array
	{
		cout << guesses[i];
		if (i == totalGuesses - 1) //when it reaches max totalGuesses
		{
			cout << " ";
			break;
		}
		cout << ", ";
	}
	cout << endl;
}

void displayPartsOfHangMan(int totalGuesses)
{
	const int row = 21;
	const int col = 21;

	int hangMan[row][col] = {
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
		{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
		{-1,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,-1},
		{-1,0,2,0,0,4,0,0,0,0,5,0,0,0,0,0,0,0,0,0,-1},
		{-1,0,2,0,4,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,-1},
		{-1,0,2,4,0,0,0,6,6,6,6,6,6,6,6,6,6,0,0,0,-1},
		{-1,0,2,0,0,0,0,7,0,10,0,0,0,0,11,0,8,0,0,0,-1},
		{-1,0,2,0,0,0,0,7,0,0,0,0,0,0,0,0,8,0,0,0,-1},
		{-1,0,2,0,0,0,0,7,0,0,0,12,13,0,0,0,8,0,0,0,-1},
		{-1,0,2,0,0,0,0,7,0,0,12,0,0,13,0,0,8,0,0,0,-1},
		{-1,0,2,0,0,0,0,7,0,12,0,0,0,0,13,0,8,0,0,0,-1},
		{-1,0,2,0,0,0,0,9,9,9,9,9,9,9,9,9,9,0,0,0,-1},
		{-1,0,2,0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,-1},
		{-1,0,2,0,0,0,15,15,15,15,15,15,15,16,16,16,16,16,16,0,-1},
		{-1,0,2,0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,-1},
		{-1,0,2,0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,-1},
		{-1,1,1,1,1,1,1,1,1,0,0,17,0,18,0,0,0,0,0,0,-1},
		{-1,0,0,0,0,0,0,0,0,0,17,0,0,0,18,0,0,0,0,0,-1},
		{-1,0,0,0,0,0,0,0,0,17,0,0,0,0,0,18,0,0,0,0,-1},
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
	};

	//array for different ascii characters
	char displayChars[20] = { char(179), char(196), char(179), char(196), char(47), char(179), char(196), char(179), char(179), char(196),char(219), char(219), char(47), char(92), char(179), char(196), char(196), char(47), char(92),' ' };

	changeTextColor(Colors::YELLOW);
	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 21; col++)
		{
			goToXY(15 + col, row + 3);
			int charIndex = hangMan[row][col]; //the value of the hangMan array is assigned to charIndex

			if (charIndex > totalGuesses)
			{
				continue; //skip code below and return to beginning of loop
			}
			if (charIndex == -1)
			{
				charIndex = 0;
			}
			else if (charIndex == -2)
			{
				charIndex = 1;
			}
			else if (charIndex == 0) //blank space
			{
				charIndex = 19;
			}
			cout << displayChars[charIndex];
		}
		cout << endl;
	}
	changeTextColor(Colors::WHITE);
}

bool isUserGuessAnInteger(int min, int max, int totalGuesses, int guess) //input validation
{
	while (guess<min || guess>max)
	{
		cout << "Please enter a integer value within " << min << " and " << max << endl;
		cin >> guess;
	}
	//If user guess is within the min and max, return true
	return true;
}

//function prototype
bool doesUserWantToPlayGuessingGameAgain(string message, int gameCount, int scores[]);
int calculateGuessingGameScores(int totalGuesses, int range, int allowedAttempts);


//int* storeScoreIntoDynamicArray(int gameCount,int score[])
//{
//	int* newScoreArr =new int[gameCount+1];
//	//int& refnum = score;
//	//scoresArr[gameCount] = score;
//	
//	for (int i = 0; i < gameCount; i++)
//	{
//		newScoreArr[i] = scoreArr[i];
//	}
//	delete[] scoreArr;
//	scoreArr = newScoreArr; //make scoresArr point to same mem. location as newScoreArr(they have same address)
//	newScoreArr[gameCount] = score;
//
//	for (int i = 0; i < gameCount + 1; i++) //display current and previous scores
//	{
//		cout << "Score for Game " << i + 1 << ": " << *(scoreArr+i) << endl; //add i to the index of newScoreArr and deference it
//	}
//	return newScoreArr;
//}

void GuessingGame(int gameCount, int scores[])
{
	int guesses[30];
	int min = 0;
	int max = 0;
	int minAllowedRange = 0;
	int totalGuesses = 0;
	int guess = 0;
	int gameDifficulty = 0;
	int allowedAttempts = 0;

	gameDifficulty = getGameDifficulty();

	if (gameDifficulty == 1)
	{
		minAllowedRange = 10; //if normal mode, range of numbers has to be at least 50
	}
	else
	{
		minAllowedRange = 100;//if hard mode, range of numbers has to be at least 100
	}

	while (true) //range validation
	{
		changeTextColor(Colors::CYAN);
		min = getNumberFromUser("Please Enter a minimum number(-): ");
		changeTextColor(Colors::RED);
		max = getNumberFromUser("Please Enter a maximum number(+): ");
		changeTextColor(Colors::WHITE);
		if (isRangeValid(min, max, minAllowedRange))
		{
			break;
		}
		cout << "Sorry, that range is too small! Make sure min and max are at least " << minAllowedRange << " apart!" << endl;
	}
	int range = max - min + 1;

	int RandomNumber = generateRandomNumberforGuessingGame(min, range);
	if (gameDifficulty == 1)
	{
		allowedAttempts = calculateAllowedAttemptsForNormal(range);
	}
	else
	{
		allowedAttempts = calculateAllowedAttemptsForHard(range);
	}
	system("cls");
	do
	{
		string message = "Guess a number (attempt " + to_string(totalGuesses + 1) + " of " + to_string(allowedAttempts) + "): ";
		int guess = getNumberFromUser(message);
		guesses[totalGuesses] = guess;
		isUserGuessAnInteger(min, max, totalGuesses, guess); //check if user guess is within min and max range
		totalGuesses++;

		system("cls");
		displayPartsOfHangMan(totalGuesses);
		cout << endl;

		if (guess < RandomNumber) //when user guess is lower than randomNumber
		{
			changeTextColor(10);
			cout << "Too low, try again..." << endl;
			changeTextColor(15);
		}
		else if (guess > RandomNumber) //when user guess is higher than randomNumber
		{
			changeTextColor(12);
			cout << "Too high, try again..." << endl;
			changeTextColor(15);
		}
		else //when user guess=randomNumber
		{
			system("cls");

			break;
		}
	} while (totalGuesses < allowedAttempts && RandomNumber != guesses[totalGuesses]);

	system("cls");
	didUserWin(RandomNumber, totalGuesses, allowedAttempts, range, guesses);
	printGuesses(guesses, totalGuesses);
	
	int score = 0;
	score = calculateGuessingGameScores(totalGuesses, range, allowedAttempts);
	scores[gameCount-1] = score;
	gameCount++;
	cout << "Your score for this game is " << calculateGuessingGameScores(totalGuesses, range, allowedAttempts) << endl;
	/*int* newScoreArr = nullptr;
	int* scoreArr = nullptr;*/
	/*oldScoreArr = getOldScoreArr(gameCount,score);
	newScoreArr = getNewScoreArr(gameCount, score,oldScoreArr);*/
	//delete[] oldScoreArr;
	//newScoreArr=storeScoreIntoDynamicArray(gameCount, scores);
	if(doesUserWantToPlayGuessingGameAgain("Do you want to play again?", gameCount, scores) == true)
	{
		gameCount++;
	}
}

void sleepDot(int milliseconds)
{
	for (int i = 0; i < 4; i++)
	{
		cout << ". ";
		Sleep(milliseconds);
	}
}

void countdownTimer(int milliseconds)
{
	for (int i = 5; i > 0; i--)
	{
		cout << i << ", ";
		Sleep(milliseconds);
	}
	cout << endl;
}

bool doesUserWantToPlayGuessingGameAgain(string message, int gameCount, int scores[]) //function for playing GuessingGame again or exiting back to menu
{
	cout << message << endl;
	string answer;
	cin >> answer;
	if (answer == "yes" || answer == "y" || answer == "Y" || answer == "Yes") //game repeats
	{
		cout << "Restarting game";
		sleepDot(500);
		cout << "\n";
		system("cls");
		GuessingGame(gameCount, scores);
		return true;
	}
	else //if (answer == "no" || answer == "n" || answer == "N" || answer == "No")
	{
		system("cls");
		return false; //game exits back to main menu
	}
}

bool didUserLieToComputer(string username, int randomNumber, int guess, int totalGuesses, int allowedAttempts, int guesses[], int answerData[], int timesUserLied)
{
	cout << "Now, I will check if you lied or helped me guess your number!" << endl << endl;
	//iterate through each guess and check if user user input makes logical sense with computer guess
	for (int i = 0; i < totalGuesses; i++)// we know how many guesses the comp. made, so use a for loop
	{
		if (randomNumber > guesses[i] && answerData[i] == 2) //when user lies that comp. guess < randomNumber
		{
			cout << "Guess # " << i + 1 << ": I guessed ";
			cout << guesses[i] << endl;
			cout << "You told me that my guess was too high, but it's lower than ";
			changeTextColor(Colors::MAGENTA);
			cout << randomNumber << endl;
			changeTextColor(Colors::WHITE);
			system("pause");
			cout << endl;
			timesUserLied++;
			if (i == totalGuesses - 1)
			{
				return true;
			}
		}
		if (randomNumber < guesses[i] && answerData[i] == 1) //when user lies that comp. guess >randomNumber
		{
			cout << "Guess # " << i + 1 << " I guessed: ";
			cout << guesses[i] << endl;
			cout << "You told me that my guess was too low, but it's higher than ";
			changeTextColor(Colors::MAGENTA);
			cout << randomNumber << endl;
			changeTextColor(Colors::WHITE);
			system("pause");
			cout << endl;
			timesUserLied++;
			if (i == totalGuesses - 1)
			{
				return true;
			}
		}
		else if (randomNumber != guesses[i] && answerData[i] == 3)//when user lies that comp. guess is correct when it isnt
		{
			cout << username << " ,you lied that Guess # " << i + 1 << " was correct when the randomNumber was ";
			changeTextColor(Colors::MAGENTA);
			cout << randomNumber << endl;
			changeTextColor(Colors::WHITE);
			system("pause");
			cout << endl;
			timesUserLied++;
			return true;
		}
		else if (randomNumber == guesses[i] && answerData[i] != 3)//when user lies that comp. guess is wrong when its correct
		{
			cout << username << " , you lied that Guess # " << i + 1 << " was wrong.\nI guessed " << guesses[i] << " but the";
			cout << " randomNumber was ";
			changeTextColor(Colors::MAGENTA);
			cout << randomNumber << endl;
			changeTextColor(Colors::WHITE);
			system("pause");
			cout << endl;
			timesUserLied++;
			return true;
		}
	}
	return false;
}

bool didComputerWin(int guess, int totalGuesses, int allowedAttempts, string username, int randomNumber, int guesses[], int answerData[], int timesUserLied)
{
	if (didUserLieToComputer(username, randomNumber, guess, totalGuesses, allowedAttempts, guesses, answerData, timesUserLied) == true)//when user lies
	{
		return false;
	}
	else if (guess == randomNumber && totalGuesses < allowedAttempts) //when computer wins
	{
		isBackgroundMusicPlaying(false);
		displayHappyHangMan();
		cout << "Yay! I won! The random number was ";
		changeTextColor(Colors::MAGENTA);
		cout << randomNumber << endl;
		changeTextColor(Colors::WHITE);
		PlaySound("Win EndScreen", NULL, SND_SYNC); //Play win endscreen music
		isBackgroundMusicPlaying(true);//restart background music
		return true;
	}
	else if (guess != randomNumber && totalGuesses == allowedAttempts) //when computer loses
	{
		isBackgroundMusicPlaying(false);
		displayDeadHangMan();
		cout << "I lost. The number was ";
		changeTextColor(Colors::MAGENTA);
		cout << randomNumber << endl;
		changeTextColor(Colors::WHITE);
		PlaySound("Lose Endscreen", NULL, SND_SYNC); //play lose endscreen music
		isBackgroundMusicPlaying(true);
		return false;
	}
	return false;
}

void printComputerGuesses(int guesses[], int totalGuesses)
{
	cout << endl << "Here are my " << totalGuesses << " guess(es): ";
	for (int i = 0; i < totalGuesses; i++) //loop through guesses[] to display the computer's guesses
	{
		cout << guesses[i];

		if (i == totalGuesses - 1)
		{
			cout << " ";
			break;
		}
		cout << ", ";
	}
	cout << endl;
}

bool doesUserWantToPlayReverseGuessingGameAgain(string message, string username, int scores[], int gameCount); //function prototype

int getComputerGuess(int totalGuesses, int guess, int min, int max, int initialMin, int initialMax, int guesses[])
{
	if (totalGuesses == 0) //special case for 1st iteration
	{
		return (int)((min + max - 1) / 2.0f);  //2.0f means a float number that is 2.0 
												//using a float because it is a 4byte value that yields a 4 byte value(int guess)
	}
	else if (guess - initialMin == 1) //when guess and initialmin are 1 apart(special case)
	{
		return (int)(round((min + max - 1) / 2.0f));
	}
	else if (initialMax - guess == 1)//when initialMax and guess are 1 apart(special case)
	{
		return (int)(round((min + max + 1) / 2.0f));
	}
	else
	{
		return (int)(round((min + max) / 2.0f));
	}
}

//When User says comp. guess > randomnumber
//function to check if user answer makes logical sense with computer guesses
bool doesPlayerAnswerMakeSenseWhenCompGuessIsHigh(int guess, int max, int min, int initialMin)
{
	if (guess < max && guess > min&& max - min == 2 && guess - initialMin != 1)
		//Ex. Player said 50 is too low, 52 is too high, the comp. should say the correct number is 51
	{
		cout << "You said the number has to be higher than " << min << " and lower than " << max << endl;
		cout << "The number should be " << guess << "!" << endl;
		system("pause");
		return false;
	}
	else if (initialMin == guess)//when comp. reaches min number it can guess
	{
		cout << "Hmm... You said that " << guess << " is too high, but I can't guess a lower number than that which means you lied!" << endl;
		system("pause");
		return false;
	}
	return true;
}

//When User says comp. guess < randomnumber
//function to check if user answer makes logical sense with computer guesses
bool doesPlayerAnswerMakeSenseWhenCompGuessIsLow(int guess, int max, int min, int initialMax)
{
	if (guess < max && guess > min&& max - min == 2 && initialMax - guess != 1)
		//Ex. Player said 52 is too high, 50 is too low, the comp. should say the correct number is 51
	{
		cout << "You said the number has to be higher than " << min << " and lower than " << max << endl;
		cout << "The number should be " << guess << "!" << endl;
		system("pause");
		return false;
	}
	else if (initialMax == guess) //when comp. reaches max number it can guess
	{
		cout << "Hmm... You said that " << guess << " is too low, but I can't guess a higher number than that which means you lied!" << endl;
		system("pause");
		return false;
	}
	return true;
}

bool isRandomNumberValid(int randomNumber, int initialMin, int initialMax) //function for input validation
{
	if (randomNumber >= initialMin && randomNumber <= initialMax)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int calculateReverseGuessingGameScores(int timesUserLied, int range)
{
	int score = 0;
	score = range / (timesUserLied + 1);
	return score;
}

void ReverseGuessingGame(string username, int gameCount, int scores[])
{
	//intialize variables
	int min = 0;
	int max = 0;
	int initialMin = 0;
	int initialMax = 0;
	int guesses[30] = { 0 };
	int minAllowedRange = 0;
	int totalGuesses = 0; //counter variable
	int randomNumber = 0;
	int answer;
	int answerData[30] = { 0 };
	int guess = 0;
	int gameDifficulty = 0;
	int allowedAttempts = 0;
	int timesUserLied = 0;

	gameDifficulty = getGameDifficulty();

	if (gameDifficulty == 1)//normal
	{
		minAllowedRange = 10;
	}
	else//hard
	{
		minAllowedRange = 100;
	}

	while (true) //get min and max number from user
	{
		changeTextColor(Colors::CYAN);
		min = getNumberFromUser("Enter min number(-): ");
		changeTextColor(Colors::RED);
		max = getNumberFromUser("Enter max number(+): ");
		changeTextColor(Colors::WHITE);

		initialMin = min;
		initialMax = max;

		if (isRangeValid(min, max, minAllowedRange) == true)//input validation
		{
			break;
		}
		cout << "Sorry, that range is too small! Make sure min and max are at least " << minAllowedRange << " apart!" << endl;
	}
	int range = max + min - 1;

	if (gameDifficulty == 1)
	{
		allowedAttempts = calculateAllowedAttemptsForNormal(range);
	}
	else
	{
		allowedAttempts = calculateAllowedAttemptsForHard(range);
	}

	cout << "Think of a random number, " << username << ": ";
	//countdownTimer(750);

	while (true) //input validation
	{
		cin >> randomNumber;
		if (isRandomNumberValid(randomNumber, initialMin, initialMax) == true)
		{
			break;
		}
		cout << "Please enter a randomNumber within " << initialMin << " and " << initialMax << endl;
	}

	do
	{
		guess = getComputerGuess(totalGuesses, guess, min, max, initialMin, initialMax, guesses);

		string message = "(attempt " + to_string(totalGuesses + 1) + " of " + to_string(allowedAttempts) + "): ";
		cout << message << "Is your number " << guess << " ?" << endl;

		guesses[totalGuesses] = guess; //store comp. guess into an array

		cout << "Enter 1 if my guess is low, 2 if it's high, or 3 if it's correct" << endl;
		cin >> answer;

		answerData[totalGuesses] = answer; //store user answer into answer array
		totalGuesses++;
		system("cls");

		displayPartsOfHangMan(totalGuesses);
		cout << endl;

		if (answer == 1) //when you tell comp. that its guess < randomNumber
		{
			if (doesPlayerAnswerMakeSenseWhenCompGuessIsLow(guess, max, min, initialMax) == false)
			{
				timesUserLied++;
				break;
			}
			min = guess; //halving the search space everytime-binary search
		}
		else if (answer == 2)//when you tell comp. that its guess>randomNumber
		{
			if (doesPlayerAnswerMakeSenseWhenCompGuessIsHigh(guess, max, min, initialMin) == false)
			{
				timesUserLied++;
				break;
			}
			max = guess;//halving the search space everytime-binary search
		}
		else if (answer == 3)//when you tell comp. that its guess is correct
		{
			system("cls");
			break;
		}

	} while (totalGuesses < allowedAttempts && guesses[totalGuesses] != randomNumber);

	system("cls");

	didComputerWin(guess, totalGuesses, allowedAttempts, username, randomNumber, guesses, answerData, timesUserLied);
	printComputerGuesses(guesses, totalGuesses);

	int score = 0;
	score = calculateReverseGuessingGameScores(timesUserLied, range);
	scores[gameCount] = score;
	gameCount++;
	cout << "Your score for this game is " << calculateReverseGuessingGameScores(timesUserLied, range) << endl;
	if (doesUserWantToPlayReverseGuessingGameAgain("Do you want to play again?", username, scores, gameCount) == true)
	{
		gameCount++;
	}
}

bool doesUserWantToPlayReverseGuessingGameAgain(string message, string username, int scores[], int gameCount)
{
	cout << message << endl;
	string answer;
	cin >> answer;
	if (answer == "yes" || answer == "y" || answer == "Y" || answer == "Yes") //game repeats
	{
		cout << "Restarting game";
		sleepDot(500);
		cout << "\n";
		system("cls");
		ReverseGuessingGame(username, gameCount, scores);
		return true;
	}
	else // (answer == "no" || answer == "n" || answer == "N" || answer == "No")
	{
		system("cls");
		return false; //game exits back to main menu
	}
}

int calculateGuessingGameScores(int totalGuesses, int range, int allowedAttempts) //calculate score
{
	int score = 0;
	score = (allowedAttempts * range) / totalGuesses;
	return score;
}

void swap(int *num1, int *num2)
{
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

void sortGuessingGameScoresUsingSelectionSort(int gameCount, int scores[])
{
	for (int i = 0; i < gameCount-1; i++)
	{
		int minIndex=0;
		// Find the max score in array  
		minIndex = i;
		for (int j = i+1 ; j < gameCount; j++)
		{
			if (scores[minIndex] < scores[j])
			{
				minIndex = j;
			}
			// Swap the found max score with the first element  
		}
		int temp;
		temp = scores[minIndex];
		scores[minIndex] = scores[i+1];
		scores[i+1] = temp;
	}
}

void displayHistogramScoreboard(int scores[],int gameCount)
{
	char histogramTexture[5] = { char(254),char(186),char(178),char(176),char(219) };

	for (int i = 0; i < 5; i++)
	{
		cout << "Score " << i + 1 << " : ";
		for (int j = 0; j < scores[i]; j++)
		{
			if (scores[i] == 0)
			{
				continue;
			}
			changeTextColor(9 + i);
			cout << histogramTexture[i];
		}
		changeTextColor(Colors::WHITE);
		cout << endl << endl;
	}
}

void displayScoreboard(int gameCount, int scores[]) // Display Top 5 scores
{
	//int score = scores[gameCount];
	goToXY(40, 0);
	cout << "The Top 5 Scores: " << endl;

	for (int i = 0; i < 5; i++)//display top 5 sorted scores
	{
		goToXY(40, 1 + i);
		cout << "Score " << i + 1 << " : ";
		changeTextColor(9 + i);
		cout << scores[i] << endl;
		changeTextColor(Colors::WHITE);
	}
	displayHistogramScoreboard(scores,gameCount);
	/*
	 guess_data(tell whether user is making educated guesses)

	easier algorithm for reverseguessinggame(normal,hard(binary search comp)), numberoftimes user lied
	normal: 10 pts for each educated guess
	hard: 20 pts for each educated guess

	formula for score:
	BASED on normal or hard difficulty, calculates user score.
	score= (totalGuesses*range)+allowedattempts/10 additional points everytime user makes educated guess + # of times user lied
	*/
	system("pause");
}

void displayHowToPlay()
{
	int gamechoice = 0;
	GameTypes gameType = (GameTypes)gamechoice;
	system("cls");

	while (true)
	{
		cout << "Choose between:\n1. Guessing Game\n2. Reverse Guessing Game\n3. Return to Menu\n\n(Press Enter when done)\n";
		cin >> gamechoice;
		if (gamechoice == 1)//(Guessing Game)
		{
			system("cls");
			cout << "Goal: Try to guess the computer's random number in the least number of guesses to prevent the hangman from dying\n";
			cout << "1. First, enter the min and max number\n\n";
			cout << "2. If the range of numbers is too narrow or if the range is not numeric, you will be asked to re-enter the range\n\n";
			cout << "3. The user is then asked to guess the number. The guess is stored in an array\n\n";
			cout << "4. Based on the user's guess, the computer will notify you if your number is too high, too low, or correct\n\n";
			cout << "5. If the user guesses the correct number, the computer congratulates the user and prints out all the guesses the user took and the number of guesses\n\n";
			cout << "6. Otherwise, if the user cannot guess the correct number within a certain number of attempts, the computer ends the game early, tells the user what the number was, and prints a list of the user's guesses\n\n";
			cout << "You will get a higher score if you choose a larger range of numbers and guess the random number in fewer number of guesses" << endl;
			system("pause");
			system("cls");
		}
		else if (gamechoice == 2)// (Reverse Guessing Game)
		{
			system("cls");
			cout << "Goal: You will think of a number and the computer will try to guess it. You will try to make the computer not guess your number.\n\n ";
			cout << "1. First, enter the min and max number\n\n";
			cout << "2. You will then think of a randomNumber\n\n";
			cout << "3. Based on your Difficulty level, the computer will try to guess your number\n\n";
			cout << "4. You will tell the computer if its guess is too low, too high, or correct (You can lie of course)\n\n";
			cout << "5. Whether the computer wins or loses, it will check if you steered the computer in the right direction\n\n";
			system("pause");
			system("cls");
		}
		else //return back to main menu
		{
			system("cls");
			cout << "Returning back to main menu";
			sleepDot(500);
			break;
		}
	}
}

void main()
{
	int gameCount = 0;
	int scores[30] = { 0 };
	isBackgroundMusicPlaying(true);

	changeTextColor(Colors::WHITE);
	goToXY(40, 0);
	string username = getUsername("Please Enter your username: ");

	while (true)//game loop
	{
		int gamechoice = 0;
	//	GameTypes gameType = (GameTypes)gamechoice; //cast strongly typed enum GameTypes to int gamechoice
		displayMenu(username);
		cin >> gamechoice;

		if (gamechoice == 1)
		{
			system("cls");
			gameCount++;
			GuessingGame(gameCount, scores);
		}
		else if (gamechoice == 2)
		{
			system("cls");
			gameCount++;
			ReverseGuessingGame(username, gameCount, scores);
		}

		else if (gamechoice == 3)
		{
			system("cls");
			sortGuessingGameScoresUsingSelectionSort(gameCount, scores);
			displayScoreboard(gameCount, scores);
		}
		else if (gamechoice == 4)
		{
			system("cls");
			displayHowToPlay();
		}
		else
		{
			system("cls");
			cout << "Thanks for playing!";
			break;
		}
	}
}
