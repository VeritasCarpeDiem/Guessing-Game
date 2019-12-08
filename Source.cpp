#include <iostream>
#include <string>
#include <windows.h>//for color
#include <conio.h> // library for _kbhit() and _getch() functions
#include <stdlib.h>//for rand function
#include <stdio.h>//for RNG
#include <time.h>//for RNG
#include <iomanip> //setw(), used for spacing 

#include "User.h"
#pragma comment(lib, "Winmm.lib")//static windows library for playing .wav music files

using namespace std;//using standard namespace

struct Player
{
	string username; //easier to not use this and rather pass username as local variable
	int totalGuesses = 0; //totalGuesses that user takes to guess number
	int randomNumber = 0; //the randomnumber user has to guess in Guessing Game
	int range = 0;
	int guesses[30] = { 0 };
	int answerData[30]; //for reverseguessinggame
	int gameDifficulty{};
};

enum class GameTypes { GuessingGame = 1, ReverseGuessingGame = 2, Scoreboard = 3, HowToPlay = 4, Exit = 5 };

enum class Colors //strongly typed class of enum colors 
{
	//(copied from microsoft website)
	//Explanation: The 2 hexadecimal values Foreground_Intensity(0x0008) and Foreground_blue(0x0001) are added(the "|" or operator) and
	//				produces Blue(0x0009). The value 9 is then assigned to Blue in a enum class 

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

void changeTextColor(Colors color)//overloaded function
								  //this allows me to enter a color of type Colors class for code readibility
								//ex. changeTextcolor(WHITE);
{
	changeTextColor((int)color);

}
bool isBackgroundMusicPlaying(bool isplaying)
{
	if(isplaying == true)
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

int getGameDifficulty(int playerCount)
{
	Player User[5];//limit to max 5 players
	cout << "Choose game difficulty:\n1. normal\n2.hard";
	cin >> User[playerCount].gameDifficulty;
	return User[playerCount].gameDifficulty;
}

int getPlayerCount()
{
	int playerCount = 0;
	if (false)
	{
		playerCount++;
	}
	return playerCount;
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
	changeTextColor(Colors::CYAN);
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
	Player User[10];
	int playerCount = getPlayerCount();
	cout << message;
	getline(cin, User[playerCount].username); //Read the entire line for the username
	return User[playerCount].username;

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

int calculateAllowedAttempts(int range)
{
	return (int)(round((log(range) / log(2))) * 2.5); 
}

int leastNumberofGuesses(int range)
{
	return (int)round(log(range) / log(2));//shortest number of guesses to guess a number using binary search
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
}

bool didUserGetLucky(int range, int totalGuesses, int guesses[], int RandomNumber) //Note to self: Check this again
{
	int smallestNumberOfGuesses = leastNumberofGuesses(range);

	if (totalGuesses < smallestNumberOfGuesses && guesses[totalGuesses-1] == RandomNumber)
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
	else if (guesses[totalGuesses-1] == RandomNumber && totalGuesses < allowedAttempts) //when user wins
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
	cout << endl << "Here are your " << totalGuesses << " guesses: ";
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

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 21; col++)
		{
			goToXY(10 + col, row + 3);
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

bool doesUserWantToPlayGuessingGameAgain(string message);

void GuessingGame()
{
	int guesses[30];
	int min = 0;
	int max = 0;
	int minAllowedRange = 50;
	int totalGuesses = 0;
	int guess = 0;
	Player User[10];

	//initializeGameDifficulty(playerCount);
	while (true) //range validation
	{
		changeTextColor(10);
		min = getNumberFromUser("Please Enter a minimum number: ");
		changeTextColor(12);
		max = getNumberFromUser("Please Enter a maximum number: ");
		changeTextColor(15);
		if (isRangeValid(min, max, minAllowedRange))
		{
			break;
		}
		cout << "Sorry, that range is too small! Make sure min and max are at least " << minAllowedRange << " apart!" << endl;
	}
	int range = max - min + 1;

	int RandomNumber = generateRandomNumberforGuessingGame(min, range);
	int allowedAttempts = calculateAllowedAttempts(range);

	do
	{
		displayPartsOfHangMan(totalGuesses + 1);
		cout << endl;
		string message = "Guess a number (attempt " + to_string(totalGuesses + 1) + " of " + to_string(allowedAttempts) + "): ";
		int guess = getNumberFromUser(message);
		guesses[totalGuesses] = guess;

		isUserGuessAnInteger(min, max, totalGuesses, guess); //check if user guess is within min and max range
		totalGuesses++;

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
		else//when user guess=randomNumber
		{
			system("cls");
			
			break;
		}
	} 	while (totalGuesses < allowedAttempts && RandomNumber != guesses[totalGuesses]);

	system("cls");
	didUserWin(RandomNumber, totalGuesses, allowedAttempts, range, guesses);
	printGuesses(guesses, totalGuesses);
	doesUserWantToPlayGuessingGameAgain("Do you want to play again?");
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

bool doesUserWantToPlayGuessingGameAgain(string message) //function for playing GuessingGame again or exiting back to menu
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
		GuessingGame();
		return true;
	}
	else if (answer == "no" || answer == "n" || answer == "N" || answer == "No")
	{
		system("cls");
		return false; //game exits back to main menu
	}
	else
	{
		return true;
	}
}

bool didUserLieToComputer(int answer, string username, int randomNumber, int guess, int totalGuesses, int allowedAttempts, int guesses[], int answerData[])
{
	//iterate through each guess and check if user user input makes logical sense with computer guess
	int i = 0;
	while (i < totalGuesses)
	{
		if (randomNumber != guesses[i] && answerData[i] == 3)//when user lies that comp. guess is correct when it isnt
		{
			cout << username << " ,you lied that Guess # " << i + 1 << " was correct when the randomNumber was " << randomNumber << endl;
			system("pause");
			return true;
		}
		else if (randomNumber == guesses[i] && answerData[i] != 3)//when user lies that comp. guess is wrong when its correct
		{
			cout << username << " , you lied that Guess # " << i + 1 << " was wrong when I guessed " << guesses[i] << " and the";
			cout << " randomNumber was " << randomNumber << endl;
			system("pause");
			return true;
		}
		else if (randomNumber > guesses[i] && answerData[i] == 2) //when user lies that comp. guess is high when guess is too low
		{
			cout << "Guess # " << i + 1 << ": I guessed " << guesses[i] << endl;
			cout << "You told me that my guess was too high, but it's lower than " << randomNumber << endl;
			system("pause");
			if (i == totalGuesses - 1)
			{
				return true;
			}
		}
		else if (randomNumber < guesses[i] && answerData[i] == 1) //when user lies that comp. guess is low when guess is too high
		{
			cout << "Guess # " << i + 1 << " I guessed: " << guesses[i] << endl;
			cout << "You told me that my guess was too low, but it's higher than " << randomNumber << endl;
			system("pause");
			if (i == totalGuesses - 1)
			{
				return true;
			}
		}
		else
		{
		}
		i++;
	}
	return false;
}

bool didComputerWin(int guess, int totalGuesses, int allowedAttempts, int answer, string username, int randomNumber, int guesses[], int answerData[])
{
	if (didUserLieToComputer(answer, username, randomNumber, guess, totalGuesses, allowedAttempts, guesses, answerData) == true)
		//when user lies
	{
		return false;
	}
	else if (guess == randomNumber && totalGuesses < allowedAttempts) //when computer wins
	{
		displayHappyHangMan();
		cout << "Yay! I won!" << endl;
		return true;
	}
	else if (guess != randomNumber && totalGuesses == allowedAttempts) //when computer loses
	{
		displayDeadHangMan();
		cout << "I lost. The number was: " << randomNumber << "!" << endl;
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

bool doesUserWantToPlayReverseGuessingGameAgain(string message, string username); //function prototype/function declaration

void ReverseGuessingGame(string username)
{
	int min = 0;
	int max = 0;
	int initialMin = 0;
	int initialMax = 0;
	int guesses[30] = { 0 };
	int minAllowedRange = 50;
	int totalGuesses = 0;
	int randomNumber = 0;
	int answer;
	int answerData[30] = { 0 };
	int guess = 0;

	while (true) //get min and max number from user
	{
		changeTextColor(10);
		min = getNumberFromUser("Enter min number: ");
		changeTextColor(12);
		max = getNumberFromUser("Enter max number: ");
		changeTextColor(15);
		initialMin = min;
		initialMax = max;
		if (isRangeValid(min, max, minAllowedRange))
		{
			break;
		}
		cout << "Sorry, that range is too small! Make sure min and max are at least " << minAllowedRange << " apart!" << endl;
	}
	int range = static_cast<int>(max + min - 1);
	int allowedAttempts = calculateAllowedAttempts(range);

	cout << "Think of a random number, " << username << ": ";
	//countdownTimer(750);
	cin >> randomNumber;

	do
	{
		if (totalGuesses == 0) //special case for 1st iteration
		{
			guess = static_cast<int>((min + max - 1) / 2.0f);
		}
		else if (guess - initialMin == 1) //when max and min are 1 apart(special case)
		{
			guess = static_cast<int>(round((min + max - 1) / 2.0f));
		}
		else if (initialMax - guess == 1)
		{
			guess = static_cast<int>(round((min + max + 1) / 2.0f));
		}
		else
		{
			guess = static_cast<int> (round((min + max) / 2.0f));
		}
		guesses[totalGuesses] = guess;

		string message = "(attempt " + to_string(totalGuesses + 1) + " of " + to_string(allowedAttempts) + "): ";
		cout << message << "Is your number " << guess << " ?" << endl;
		cout << "Enter 1 if my guess is low, 2 if it's high, or 3 if it's correct" << endl;
		cin >> answer;
		answerData[totalGuesses] = answer;
		totalGuesses++;
		system("cls");
		displayPartsOfHangMan(totalGuesses);
		cout << endl;

		if (answer == 1)//when guess is low
		{
			min = guess;
			if ((max - min == 2 || max - min == 1) && guess < max && guess > min&& guess - initialMax != 1)
			{
				cout << "You said the number has to be higher than " << min << " and lower than " << max << endl;
				cout << "The number should be " << guess << endl;
				system("pause");
				break;
			}
			else if (initialMax == guess)
			{
				break;
			}
		}
		else if (answer == 2)//when guess is high
		{
			max = guess;
			if ((max - min == 2 || max - min == 1) && guess < max && guess > min&& guess - initialMin != 1)
			{
				cout << "You said the number has to be higher than " << min << " and lower than " << max << endl;
				cout << "The number should be " << guess << endl;
				system("pause");
				break;
			}
			else if (initialMin == guess)
			{
				break;
			}
		}
		else if (answer == 3)//when guess is correct
		{
			system("cls");
			break;
		}
		else
		{
			break;
		}

	} while (totalGuesses < allowedAttempts && guesses[totalGuesses] != randomNumber);

	system("cls");

	didComputerWin(guess, totalGuesses, allowedAttempts, answer, username, randomNumber, guesses, answerData);
	printComputerGuesses(guesses, totalGuesses);
	doesUserWantToPlayReverseGuessingGameAgain("Do you want to play again?", username);
}

bool doesUserWantToPlayReverseGuessingGameAgain(string message, string username)
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
		ReverseGuessingGame(username);
		return true;
	}
	else if (answer == "no" || answer == "n" || answer == "N" || answer == "No")
	{
		system("cls");
		return false; //game exits back to main menu
	}
	else
	{
		return true;
	}
}

int calculateGuessingGameScores(int totalGuesses, int range, int allowed)
{
	int score = 0;
	//static cast<int>
	return score;
}

int calculateReverseGuessingGameScores()
{
	int score = 0;
	return score;
}

//void countdownTimer(int totalGuesses) //note to self: screen buffer
//{
//	double d = 0;
//
//	while (true)
//	{
//		d = d + 0.00001;
//		if (_kbhit())
//		{
//			totalGuesses = 0;
//		}
//		
//		if (d > 100)
//		{
//			d = 0;
//			totalGuesses++;
//		}
//	}
//}

void displayScoreboard() // Display Top 5 scores
{
	/*
	difficulty(more allowedattempts, totalGuesses, range, guess_data(tell whether user is making educated guesses)

	easier algorithm for reverseguessinggame(normal,hard(binary search comp)), numberoftimes user lied
	normal: 10 pts for each educated guess
	hard: 20 pts for each educated guess

	formula for score:
	BASED on normal or hard difficulty, calculates user score.
	score= (totalGuesses*range)+allowedattempts/10 additional points everytime user makes educated guess + # of times user lied
	*/
	system("pause");

}

void HowToPlay(int gamechoice)
{
	GameTypes gameType = (GameTypes)gamechoice;
	cout << "Choose between:\n1. Guessing Game\n2. Reverse Guessing Game\n(Press Enter when done)\n";
	cin >> gamechoice;

	switch (gamechoice)
	{
	case 1: //gamechoice==1(Guessing Game)
		system("cls");
		cout << "The goal of this game is to try to guess the computer's random number in the least number of guesses to prevent the hangman from dying\n";
		cout << "1. First, enter the min and max number to determine the range of numbers\n\n";
		cout << "2. If the range of numbers is too narrow or if the range is not numeric, you will be asked to re-enter the range\n\n";
		cout << "3. The user is then asked to guess the number. The guess is stored in an array\n\n";
		cout << "4. Based on the user's guess, the computer will notify you if your number is too high, too low, or correct\n\n";
		cout << "5. If the user guesses the correct number, the computer congratulates the user and prints out all the guesses the user took and the number of guesses\n\n";
		cout << "6. Otherwise, if the user cannot guess the correct number within a certain number of attempts, the computer ends the game early, tells the user what the number was, and prints a list of the user's guesses\n";
		system("pause");
		break;
	case 2: //gamechoice == 2 (Reverse Guessing Game)
		system("cls");
		cout << " ";
		system("pause");
		break;
	default: //return back to main menu
		system("cls");
		cout << "Returning back to main menu";
		sleepDot(500);
	}
}

int main()
{
	isBackgroundMusicPlaying(true); 
	
	goToXY(40, 0);
	changeTextColor(15);
	string username = getUsername("Please Enter your username: ");
	int playerCount = 0;
	while(playerCount=)
	{
		while (true)
		{
			int gamechoice = 0;
			GameTypes gameType = (GameTypes)gamechoice; //cast strongly typed enum GameTypes to int gamechoice
			displayMenu(username);
			cin >> gamechoice;

			if (gamechoice == 1)
			{
				system("cls");
				GuessingGame();
			}
			else if (gamechoice == 2)
			{
				system("cls");
				ReverseGuessingGame(username);
			}
			else if (gamechoice == 3)
			{
				system("cls");
				displayScoreboard();
			}
			else if (gamechoice == 4)
			{
				system("cls");
				HowToPlay(gamechoice);
			}
			else
			{
				system("cls");
				cout << "Thanks for playing!";
				break;
			}
		}
		playerCount++;
	}
	return 0;
}
