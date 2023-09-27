
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define WORD_LENGHT 5 //condition for the words on the list, has to be 5 letters only

char	**dictionary = NULL; //Global variable to store the list of words with 5 letters
int		num_words_in_dictionary = 0; // Global variable to keep track of the number of words

//Could have used structure/or Hash maps and for's but wanted to go raw and with 42's methodology given the timeframes
//Global variables to store statistics
int gamesPlayed = 0;
int gamesWon = 0;
int bestTry = 0;
int actualWins = 0;
int bestWins = 0;

//could have used an array here as well but decided to do it by direct assignment given time constraints and debugging
//Global Variables for the scores of each try
float	life_6_score;
float	life_5_score;
float	life_4_score;
float	life_3_score;
float	life_2_score;
float	life_1_score;

//-------- best tries from all games count
// didn't have time to finish the stats for best try for each life but
// I created a ranking system to choose the best try with yellow's being 0,5 score and greens 1 score
int	life_6_tries;
int	life_5_tries;
int	life_4_tries;
int	life_3_tries;
int	life_2_tries;
int	life_1_tries;

/*this functions makes sure that the input is converted to lower-case
for both readability and scalability*/
char	*put_input_in_lower_case(char *input)
{
	int i;
	int max_lenght;

	i = 0;
	max_lenght = strlen(input);
	while (i < max_lenght)
	{
		input[i] = tolower(input[i]);
		i++;
	}
	return (input);
}

//this function checks for extra grammar or esoteric letters
int	word_checker(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (isalpha(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

//will check if the existing word exists in the dictionary
int	is_word_in_dictionary(const char *User_input)
{
	int	i;

	i = 0;
	while (i < num_words_in_dictionary)
	{
		if (strcmp(dictionary[i], User_input) == 0)
			return (1); //found the word in dictionary
		i++;
	}
	return (0); //Didn't find the word in the dictionary
}

//filters the user's input for non-existing words or esoteric grammar
char	*is_guess_five_chars(char	*str)
{
	if (strlen(str) != 5 || word_checker(str) == 0)
	{
		printf("Hey! Please make sure the guess has 5 letters only (punctuation does not count), good luck!\n");
		printf("Try again: ");
		scanf("%s", str);
		str = put_input_in_lower_case(str);
		is_guess_five_chars(str);
	}
	if (is_word_in_dictionary(str) == 0)
	{
		printf("It seems this word doesn't exist in our dictionary, please try another one:\n");
		scanf("%s", str);
		str = put_input_in_lower_case(str);
		is_guess_five_chars(str);
	}
	return (str);
}

//frees the list of words and corrects memory leaks
void	free_the_words_list(char **wordlist, int number_of_words)
{
	int	i;

	i = 0;
	while (i < number_of_words)
		free(wordlist[i++]);
	free(wordlist);
}

//this function provides the hints with color grading to be a bit more user friendly and a score
//for the best tries placement
float	hints_provider(char *secret_sauce, char *user_input)
{
	int		i;
	int		j;
	int 	letter;
	float	score;

	i = 0;
	j = 0;
	letter = 1;
	while (secret_sauce[i] != '\0')
	{
		if (secret_sauce[i] == user_input[j])
		{
			printf("The %d letter is \033[32mcorrect\033[0m.\n", letter);
			i++;
			j++;
			letter++;
			score += 1;
		}
		else if (strchr(secret_sauce, user_input[j]) != NULL)
		{
			printf("The %d letter is \033[33mpresent, but not correct\033[0m, check the placement or repetition!\n", letter);
			i++;
			j++;
			letter++;
			score += 0.5;
		}
		else
		{
			printf("The %d letter is \033[31mincorrect and does not exist in the word\033[0m.\n", letter);
			i++;
			j++;
			letter++;
			score += 0;
		}
	}
	return (score);
}

//this function randomly chooses the word to guess from a list
char	*sauce_randomizer(void)
{
	int		random_n;
	int		word_lenght;
	char	word[WORD_LENGHT + 1]; // letters cap + null
	char	*new_word;
	char	**temp;

	FILE *popular = fopen("popular.txt", "r");
	if (popular == NULL){
		perror("Error opening the file");
		return NULL;
	}
	while (fscanf(popular, "%s", word) != EOF){
		//printf("Reading word: %s\n", word); //debugging to check the list
		word_lenght = strlen(word);
		if (word_lenght == WORD_LENGHT){ // checks the lenght of the words and ignores the rest
			new_word = strdup(word);
			if (new_word == NULL){
				perror("Error allocating memory to the new word");
				free_the_words_list(dictionary, num_words_in_dictionary);
				fclose(popular);
				return NULL;
				}
			strcpy(new_word, word);
			temp = (char **)realloc(dictionary, (num_words_in_dictionary + 1) * sizeof(char *));
			if (temp == NULL) {
				perror("Error reallocating memory to the array");
				free_the_words_list(dictionary, num_words_in_dictionary);
				free(new_word);
				fclose(popular);
				return (NULL);
				}
			///printf("New word: %s\n", new_word); //debugging for seg faults
			dictionary = temp;
			dictionary[num_words_in_dictionary] = new_word;
			num_words_in_dictionary++;
		}
	}
	fclose(popular);
	if (num_words_in_dictionary == 0){
		printf("No words with the specifics, found in the file.\n");
		free_the_words_list(dictionary, num_words_in_dictionary);
		return NULL;
	}
	srand(time(NULL)); //clears the previous value in case it has been used before
	random_n = rand() % num_words_in_dictionary;
	printf("This is the chosen word \033[1;4m%s\033[0m!\n", dictionary[random_n]);
	return (dictionary[random_n]);
}

//checks if the stats file exists already
int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // File exists
    }
    return 0; // File doesn't exist
}

void	initializingStatsFile(void)
{
	FILE *statsFile = fopen("Statistics.txt", "w");
	if (statsFile == NULL)
	{
		perror("Error opening the statistics file");
		return;
	}
    fprintf(statsFile, "Games played: 0\n");
    fprintf(statsFile, "Games won: 0\n");
	fprintf(statsFile, "Percentage of Won games: 0.00%%\n");
    fprintf(statsFile, "Current wins in a row: 0\n");
    fprintf(statsFile, "Best wins in a row: 0\n");
	fprintf(statsFile, "Best try: #0\n");
	fclose(statsFile);
}

void	updateStatistics(void)
{
	int		value; //to store the values from the file
	char	line[256];
	double	percentageWon;

	FILE *statsFile = fopen("Statistics.txt", "r+");
	if (statsFile == NULL)
	{
		perror("Error opening the statistics file");
		return;
	}
	while (fgets(line, sizeof(line), statsFile) != NULL)
	{
		if (sscanf(line, "Games played: %d", &value) == 1) //games played for stats
			gamesPlayed += value;
		if (sscanf(line, "Games won: %d", &value) == 1) // games won for stats
			gamesWon += value;
		if (sscanf(line, "Current wins in a row: %d", &value) == 1) //current winning streak
		{
			if (actualWins != 0)
				actualWins += value;
		}
		if (sscanf(line, "Best wins in a row: %d", &value) == 1) //best winning streak ever done
		{
			if (actualWins > value)
				bestWins = actualWins;
			else
				bestWins = value;
		}
	}
	fseek(statsFile, 0, SEEK_SET);
	fprintf(statsFile, "Games played: %d\n", gamesPlayed);
	fprintf(statsFile, "Games won: %d\n", gamesWon);
	percentageWon = (double)gamesWon / gamesPlayed * 100.0;
	fprintf(statsFile, "Percentage of Won games: %.2f%%\n", percentageWon);
	fprintf(statsFile, "Current wins in a row: %d\n", actualWins);
	fprintf(statsFile, "Best wins in a row: %d\n", bestWins);

	if (bestTry != 0)
		fprintf(statsFile, "The best try was on life %d\n", bestTry);
	fclose(statsFile);
}

void	displayStatistics(void)
{
	char	line[256];
	double	percentageWon;

	FILE* statsFile = fopen("statistics.txt", "r");
    if (statsFile == NULL) {
        perror("Error opening statistics file");
        return;
    }
	percentageWon = 0.0;
	while (fgets(line, sizeof(line), statsFile) != NULL)
	{
		if (sscanf(line, "Games played: %d", &gamesPlayed) == 1)
			printf("Games played: %d\n", gamesPlayed);
		if (sscanf(line, "Games won: %d", &gamesWon) == 1)
            printf("Games won: %d\n", gamesWon);
    	if (sscanf(line, "Percentage of Won games: %lf%%\n", &percentageWon) == 1)
            printf("Percentage of Won games: %.2f%%\n", percentageWon);
		if (sscanf(line, "Current wins in a row: %d", &actualWins) == 1)
            printf("Current wins in a row: %d\n", actualWins);
        if (sscanf(line, "Best wins in a row: %d", &bestWins) == 1)
        	printf("Best wins in a row: %d\n", bestWins);
	}
    printf("Best try: #%d\n", bestTry);
	fclose(statsFile);
}

int	main_game(char *user_name)
{
	char 	*word;
	char 	*User_input;
	int 	result;
	int		lives;
	float 	currentLife_score;

	gamesPlayed = 0;
	gamesWon = 0;
	actualWins = 0;

	lives = 6;
	User_input = (char *)malloc(5);
	word = sauce_randomizer();
	while (lives != -1)
	{
		if (lives == 6)
			printf("Welcome %s! You will start with \033[1;4;34m%d\033[0m tries, hopefully it's enough to beat me!\n", user_name, lives);
		else if (lives == 4)
			printf("You have \033[1;4;34m%d\033[0m tries left, rest easy, you can do this!\n", lives);
		else if (lives == 2)
			printf("You have \033[1;4;34m%d\033[0m tries left, don't quit might be simpler than it is!\n", lives);
		else if (lives == 0)
			printf("You have \033[1;4;34m%d\033[0m tries left, shoot for the stars!\n", lives);
		else
			printf("You have \033[1;4;34m%d\033[0m tries left.\n", lives);
		if (lives == 6)
			printf("Please provide your initial guess: ");
		if (lives <= 5 && lives >= 0)
			printf("Try again: ");
		scanf("%s", User_input);
		User_input = put_input_in_lower_case(User_input); //puts the input in all lower caps
		is_guess_five_chars(User_input);
		result = strcmp(word, User_input);
		if (result == 0) {
			printf("Congratulations! You win, here is a cookie!\n");
			printf( "									  \n"
					"         ⢀⣠⣴⣶⣿⣿⡿⠿⠷⣶⣤⣄⡀        \n"
					"      ⢀⣴⣾⣿⣿⣿⣿⣿⣿⣇  ⢸⣿⣿⣿⣦⡀       \n"
					"    ⢀⣴⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀    \n"
					"   ⢠⣿⡟⠁  ⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀   \n"
					"  ⢠⣿⣿⣿⣦⣄⣠⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣷   \n"
					"  ⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏  ⢹⣿⣿⣿⡇  \n"
					"  ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣿⣿⣿⣿⣿  \n"
					"  ⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟  \n"
					"  ⠈⢿⣿⣿⣿⣿⠟⠻⣿⣿⠋ ⠉⣿⣿⣿⣿⣿⣿⣿⣿⡏ ⢙⣿⠃  \n"
					"   ⠈⢿⣿⣿⠁  ⠘⣿⣆ ⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏   \n"
					"     ⠙⢿⣦⣤⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋    \n"
					"       ⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠉⢹⣿⣿⡿⠟⠁     \n"
					"          ⠉⠛⠿⠿⣿⣿⣿⣷⡤⠾⠛⠉        \n"
					"									  \n");
			gamesPlayed += 1;
			gamesWon += 1;
			actualWins += 1;
			bestTry = lives;
			return (0);
		} else {
			if (lives != 0)
			{
				printf("Not quite! Here's some hints: \n");
				currentLife_score = hints_provider(word, User_input);
				printf("The attempt \033[1;4m%d\033[0m has a score of: \033[1;4m%0.1lf\033[0m\n" , lives, currentLife_score);
				if (lives == 6)
					life_6_score = currentLife_score;
				else if (lives == 5)
					life_5_score = currentLife_score;
				else if (lives == 4)
					life_4_score = currentLife_score;
				else if (lives == 3)
					life_3_score = currentLife_score;
				else if (lives == 2)
					life_2_score = currentLife_score;
				else if (lives == 1)
					life_1_score = currentLife_score;
			}
		}
		lives--;
	}
	printf("\033[1;4;31mGame Over\033[0m, the correct word was \033[1;4m%s\033[0m, better luck next time!\n", word);
	gamesPlayed += 1;
	actualWins = 0;
	free(User_input);
	return (0);
}

int	main(void)
{
	char	*User_name;
	int		user_choice;
	int		play_again;

	if (fileExists("Statistics.txt") == 0)
	{
        initializingStatsFile();
        printf("\033[1mStatistics file created.\033[0m\n");
    }
	User_name = (char *)malloc(sizeof(int) * WORD_LENGHT);
	printf("Hey there! Nice to meet you, this is \033[1;4mWordle82\033[0m, can I get your name?\n");
	scanf("%s", User_name);
	do {
		printf("Thank you \033[1;4m%s\033[0m, let's start a new game or do you want to see the stats?\n", User_name);
		printf("Press \033[1;4m1\033[0m to start a new game, or \033[1;4m2\033[0m, to see the stats so far!\n");
		scanf("%d", &user_choice);
		if (user_choice == 1){
			do {
				play_again = main_game(User_name);
				updateStatistics();
			} while (play_again);
		}
		else if (user_choice == 2)
			displayStatistics();
		else
			printf("Invalid choice, please press \033[1;4m1\033[0m to start a new game, or \033[1;4m2\033[0m to view your stats!\n");
		printf("Do you wish to go back to the dashboard or to quit the game? (Press \033[1;4m1\033[0m to \033[1;4mgo to the dashboard\033[0m, \033[1;4m0\033[0m to \033[1;4mend the program\033[0m): ");
		scanf("%d", &user_choice);
	} while (user_choice == 1);
	free(User_name);
	return (0);
}
