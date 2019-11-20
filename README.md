c++ Console Game
# Hangman Guessing-Game and Reverse Guessing Game
{
	concepts used: If/else, loops, arrays, functions, structs, enum class, pointers, binary search algorithm
}
Guessing Game(Work in Progress)
1. When the program runs, the user is asked for their name
2. User chooses between "guessing game", "reverse guessing game" , and "exit"
  These 2 games are implemented as distinct function;however, you should creat common functions for both of these games
3. Depending on user's choice, appropriate game starts
5. Upon finishing a game, user is asked to play again; If they don not want to play again, user is returned to main menu.

Guessing game:

  {
  1. User is asked for range of numbers- min_value and max_value
  2. valid range: if thr range between the max and min number is less than or equal to 50(seems fair enough), range is not valid
  3. If the range is valid, computer generates a random number, inclusive of min and max number.
  4. User guesses the number. Number stored in array.
  4.5: Computer notifies if number is low, high, or correct. If the guess does not equal random_num, user is asked to guess again. 
  5. Once user correctly guesses the number, computer congratulates the user, and displays all of the user's previous guesses
  6. If the user cannot guess the number within a set amount of attemps called max_allowed_guess, the game exits, computer displays all your
  previous guesses and tells user what the random number was. 
  7. User is offered to play again or return to main menu.
  
  }
 
  
  
