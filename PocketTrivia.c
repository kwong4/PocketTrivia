#include <allegro.h>
#include "PocketTrivia.h"
#include <stdio.h>

using namespace std;

// Obtain input from user
// Actions differ based on gamemode.
// Selection - gamemode == 0
// Questions - gamemode == 1
int getinput(int gamemode) {
	
	// If user quits game
	if (key[KEY_ESC]) {
		allegro_exit();
		exit(0);
	}
	
	// Move cursor for selection
	if (gamemode == 0) {
		if (key[KEY_DOWN] && selection != max_selection) {
			rectfill(screen, WIDTH/12 + 1, HEIGHT/4 + selection * 15 - 1, WIDTH/12 + 9, HEIGHT/4 + selection * 15 + 7, BLACK);
			selection++;
			rectfill(screen, WIDTH/12 + 1, HEIGHT/4 +  selection * 15 - 1, WIDTH/12 + 9, HEIGHT/4 + selection * 15 + 7, WHITE);
		}
		else if (key[KEY_UP] && selection != 0) {
			rectfill(screen, WIDTH/12 + 1, HEIGHT/4 + selection * 15 - 1, WIDTH/12 + 9, HEIGHT/4 +  selection * 15 + 7, BLACK);
			selection--;
			rectfill(screen, WIDTH/12 + 1, HEIGHT/4 + selection * 15 - 1, WIDTH/12 + 9, HEIGHT/4 + selection * 15 + 7, WHITE);
		}
		else if (key[KEY_ENTER]) {
			return -1;
		}
	}
	// Answer questions
	else {
		if (key[KEY_1] || key[KEY_1_PAD]) {
			return 1;
		}
		else if (key[KEY_2] || key[KEY_2_PAD]) {
			return 2;
		}
		else if (key[KEY_3] || key[KEY_3_PAD]) {
			return 3;
		}
		else if (key[KEY_4] || key[KEY_4_PAD]) {
			return 4;
		}
	}
	// None of the actions keys selected
	return 0;
}

// Print in correct format
void print_formated(const char* text, int x1, int x2, int y, int col, int bg) {
	
	// Buffer and initial variables
	char line[256];
	int end_position = strcspn(text, "\0");
	int current_y = y;
	int length = x2 - x1;
	
	// Check if we need to format
	if ((end_position * CHAR_PER_LENGTH) > length) {
		
		// Inital variables
		int start = 0;
		int end;
		int curr_space = 0;
		int next_space = 0;
		
		// Cycle through spaces and print on next line if doesn't fit
		while(next_space < end_position - 1) {
			next_space += strcspn(&text[curr_space], " ") + 1;
			if (((next_space - start) *  CHAR_PER_LENGTH) < length) {
				curr_space = next_space;
			}
			else {
				strncpy(line, &text[start], curr_space - start);
				strncpy(&line[curr_space - 1 - start], "\0", 1);
				textprintf_ex(screen, font, x1, y, col, bg, "%s", line);
				start = curr_space;
				y += 10;
			}
		}
		strncpy(line, &text[start], end_position - start);
		strncpy(&line[end_position - start], "\0", 1);
		textprintf_ex(screen, font, x1, y, col, bg, "%s", line);
	}
	else {
		textprintf_ex(screen, font, x1, y, col, bg, "%s", text);
	}
}

// Obtain options from textfile
void parse_options(struct Option options[], const char* textfile) {
	
	// Open file and initalize variables
	FILE *file = fopen(textfile, "r");
	char line[256];
	int current_option = -1;
	int end_position = 0;
	int current_file = 0;
	int i;
	
	// Check if file is opened successfully
	if (file != NULL) {
		
		// Parse the Option
		while(fgets(line, sizeof(line), file)) {
			end_position = strcspn(line, "\n") - 1;
			if (strncmp(line, "-", 1) == 0) {
				if (current_option != -1) {
					options[current_option].files = current_file;	
				}
				current_option++;
				current_file = 0;
				strncpy(options[current_option].str_name, &line[1], end_position);
				strncpy(&options[current_option].str_name[end_position], "\0", 1);
			}
			else {
				strncpy(options[current_option].str_filename[current_file], &line[1], end_position);
				strncpy(&options[current_option].str_filename[current_file][end_position], "\0", 1);
				current_file++;
			}
		}
		options[current_option].files = current_file;
		fclose(file);
	}
	else {
		exit(0);
	}
}

// Concatenate a filepath
char* concat_filepath(const char* foldername, const char* filename) {
	char* result = (char*) malloc(strlen(foldername) + strlen(filename) + 2);
	strcpy(result, foldername);
	strcat(result, "\\");
	strcat(result, filename);
	return result;
}

// Concatenate a string
char* concat(const char* str1, const char* str2) {
	char* result = (char*) malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(result, str1);
	strcat(result, str2);
	return result;
}

// Parse Questions file
void parse_questions(Question questions[], const char* textfile) {
	
	// Open file and initial variables
	FILE *file = fopen(textfile, "r");
	char line[MAX_WORD];
	int answer = 0;
	int end_position = 0;
	int current_question = -1;
	
	// Check if file opened successfully
	if (file != NULL) {
		
		// Parse the Questions file
		while(fgets(line, sizeof(line), file)) {
			end_position = strcspn(line, "\n") - 1;
			if (strncmp(line, "-", 1) == 0) {
				current_question++;
				answer = 0;
				strncpy(questions[current_question].str_question, &line[1], end_position);
				strncpy(&questions[current_question].str_question[end_position], "\0", 1);
			}
			else {
				if (strncmp(line, "*", 1) == 0) {
					questions[current_question].answer = answer + 1;
				}
				strncpy(questions[current_question].str_answer[answer], &line[1], end_position);
				strncpy(&questions[current_question].str_answer[answer][end_position], "\0", 1);
				answer++;
			}
		}
		fclose(file);
	}
	else {
		allegro_exit();
		exit(0);
	}
}

// Draw initial start screen instructions
void draw_startscreen() {
	
	textout_centre_ex(screen, font, "Pocket Trivia", WIDTH/2, HEIGHT/4, WHITE, BLACK);
    textout_centre_ex(screen, font, "Press ENTER to Start!", WIDTH/2, HEIGHT/2, YELLOW, BLACK);
    textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to navigate menu selection", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "2. Use the ENTER key to select an option", WIDTH/8, HEIGHT/2 + 70, WHITE, BLACK);
    textout_ex(screen, font, "3. Then use the NUMBER keys to choose an answer for a question", WIDTH/8, HEIGHT/2 + 80, WHITE, BLACK);
    textout_ex(screen, font, "   A. = 1", WIDTH/8, HEIGHT/2 + 90, YELLOW, BLACK);
    textout_ex(screen, font, "   B. = 2", WIDTH/8, HEIGHT/2 + 100, YELLOW, BLACK);
    textout_ex(screen, font, "   C. = 3", WIDTH/8, HEIGHT/2 + 110, YELLOW, BLACK);
    textout_ex(screen, font, "   D. = 4", WIDTH/8, HEIGHT/2 + 120, YELLOW, BLACK);
    textout_ex(screen, font, "4. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 130, WHITE, BLACK);
}

// Print the options in correct format
void print_options(Option options[], const char* title, int max, int cont) {
	
	// Inital variables
	int i;
	int position = 0;
	
	// Print title
	textprintf_ex(screen, font, WIDTH/12, HEIGHT/4 - 20, WHITE, BLACK, "%s", title);
	
	// Print options
	for(i = 0; i < max; i++) {
    	rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    	textprintf_ex(screen, font, WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK, "%s", options[i].str_name);
    	position += 15;
    }
    
    // Check if there is a need for a continue option
    if (cont == 1) {
    	rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
	    textout_ex(screen, font, "Continue", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    }
    
    // Reset Cursor and draw one
    selection = 0;
    rectfill(screen, WIDTH/12 + 1, HEIGHT/4 - 1, WIDTH/12 + 9, HEIGHT/4 + 7, WHITE);
}

// Print textbook options
void print_textbook_options() {
	
	// Initial variable
	int position = 0;
	
	// Title
    textout_ex(screen, font, "Please select an option to choose your questions!", WIDTH/12, HEIGHT/4 - 20, WHITE, BLACK);
    
    // Options
    rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "All Questions (Whole textbook)", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    position += 15;
    rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "Choose by Unit", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    position += 15;
    rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "Choose by Chapter", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    
    // Cursor
    rectfill(screen, WIDTH/12 + 1, HEIGHT/4 - 1, WIDTH/12 + 9, HEIGHT/4 + 7, WHITE);
}

// Parse all of the questions in a given file
void parse_all_questions(Question questions[], const char* folderpath) {
	
	// Open file and inital variables
	FILE *file = fopen(concat_filepath(folder_topic, TEXT), "r");
	char line[256];
	int end_position;
	int current_file = 0;
	
	// Check if file opened successfully
	if (file != NULL) {
		
		// Cycle through all the files and parse them
    	while(fgets(line, sizeof(line), file)) {
    		end_position = strcspn(line, "\n");
    		strncpy(&line[end_position], "\0", 1);
    		parse_questions(&questions[current_file * MAX_QUESTIONS_PER_FILE], concat_filepath(folderpath, line));
    		current_file++;
    	}
	}
	else {
		allegro_exit();
	}
	fclose(file);
}

// Randomize order
void randomize_order(int order[], int size) {
	
	//Initalize variable
	int i;
	int temp;
	int swap;
	
	// Allocate all numbers
	for (i = 0; i < size; i++) {
		order[i] = i;	
	}
	
	// Randomize and swap numbers
	for (i = 0; i < size; i++) {
		swap = rand() % size;
		temp = order[i];
		order[i] = order[swap];
		order[swap] = temp;
	}
}

// Initalize Trivia Game
void start_questions(Question questions[], int num_questions) {
	
	// Initial variables
	int i;
	int correct = 0;
	int user_guess;
	int order[num_questions];
	
	randomize_order(order, num_questions);
	
	// Cycle through all Questions
	for (i = 0; i < num_questions; i++) {
		
		// Slow game down
		rest(500);
		
		// Clear Screen
    	rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    	
    	// Stats
    	textprintf_centre_ex(screen, font, WIDTH/2, 5, WHITE, BLACK, "User Score: %i/%i", correct, i);
    	textprintf_ex(screen, font, 10, 10, WHITE, BLACK, "Question: %i", i + 1);
    	
    	// Boxes
    	rect(screen, 20, 60, WIDTH - 20, HEIGHT * 3 / 7 - 40, WHITE);
    	rect(screen, 20, HEIGHT * 3 / 7 - 10, WIDTH - 20, HEIGHT * 4 / 7 - 20, WHITE);
    	rect(screen, 20, HEIGHT * 4 / 7 - 10, WIDTH - 20, HEIGHT * 5 / 7 - 20, WHITE);
    	rect(screen, 20, HEIGHT * 5 / 7 - 10, WIDTH - 20, HEIGHT * 6 / 7 - 20, WHITE);
    	rect(screen, 20, HEIGHT * 6 / 7 - 10, WIDTH - 20, HEIGHT - 20, WHITE);
    	
    	// Text
    	print_formated(concat("Q. ", questions[order[i]].str_question), 30, WIDTH - 30, 70, WHITE, BLACK);
    	print_formated(concat("A. ", questions[order[i]].str_answer[0]), 30, WIDTH - 30, HEIGHT * 3 / 7, WHITE, BLACK);
    	print_formated(concat("B. ", questions[order[i]].str_answer[1]), 30, WIDTH - 30, HEIGHT * 4 / 7, WHITE, BLACK);
    	print_formated(concat("C. ", questions[order[i]].str_answer[2]), 30, WIDTH - 30, HEIGHT * 5 / 7, WHITE, BLACK);
    	print_formated(concat("D. ", questions[order[i]].str_answer[3]), 30, WIDTH - 30, HEIGHT * 6 / 7, WHITE, BLACK);
	    
	    // Obtain input from user
	    while(1) {
	    	user_guess = getinput(1);
	    	if (user_guess > 0) {
	    		if (user_guess == questions[order[i]].answer) {
	    			correct++;
	    			masked_blit(correct_pic, screen, 0, 0, WIDTH/8, HEIGHT/8, correct_pic->w, correct_pic->h);
					play_sample(correct_sound, 128, 128, 1000, FALSE);	
	    		}
	    		else {
	    			masked_blit(incorrect_pic, screen, 0, 0, WIDTH/8, HEIGHT/8, incorrect_pic->w, incorrect_pic->h);
	    			play_sample(incorrect_sound, 128, 128, 1000, FALSE);	
	    		}
	    		break;
	    	}
	    }
	}
	
	// Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    	
    // Print final score
    textprintf_centre_ex(screen, font, WIDTH/2, HEIGHT/2, WHITE, BLACK, "Final User Score: %i/%i", correct, i);
    textout_centre_ex(screen, font, "Please Press ENTER or ESC to QUIT!", WIDTH/2, HEIGHT/2 + 20, WHITE, BLACK);
}

int main(void)
{
	
    //Initialize Allegro
	allegro_init();
    install_keyboard();
    install_timer();
    srand(time(NULL));
    set_color_depth(desktop_color_depth());
    
    //Initalize variables
    int i, j;
    int position;
    int current_file;
    int end_position;
    int num_files = 0;
    int all_questions = 0;
    background = load_sample(BACKGROUND_FILE);
    correct_sound = load_sample(CORRECT_FILE);
    incorrect_sound = load_sample(INCORRECT_FILE);
    correct_pic = load_bitmap(CHECK_FILE, NULL);
    incorrect_pic = load_bitmap(ERROR_FILE, NULL);
    
    //install a digital sound driver
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
    	allegro_message("Error initalizing sound system");
		return 1;	
    }
    
    // Check if sound files loaded
    if (!background || !correct_sound || !incorrect_sound) {
    	allegro_message("Error reading wave files");
    	return 1;
    }
    
    //Initialize Graphics
    int ret;
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return 0;
    }
    
    // Check if BITMAP files loaded
    if (!incorrect_pic || !correct_pic) {
    	allegro_message("Error reading bitmap files");
    	return 1;
    }
    
    play_sample(background, 128, 128, 1000, TRUE);
    
    //Print initial Start Screen with instructions
    draw_startscreen();
    
    //Wait until user press Enter
    while(1) {
    	if (key[KEY_ENTER]) {
    		break;
    	}
    	else if (key[KEY_ESC]) {
    		allegro_exit();
    		exit(0);
    	}
	};
	
	// Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    // Obtain topic options for the user
    struct Option topics[TOPICNUMBER];
    parse_options(topics, TOPIC);
    
    // Print options for user
    print_options(topics, "Please select a topic!", TOPICNUMBER, 0);
    
    // Slow game and set max option
    max_selection = TOPICNUMBER - 1;
    rest(1000);
    
    //Wait until user selects a topic
    while(1) {
    	if (getinput(0) == -1) {
    		break;
    	}
		rest(100);
	};
    
    // Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    // Set folder_topic based on user selection
    strcpy(folder_topic, topics[selection].str_filename[0]);
    
    // Print textbook options for user
    print_textbook_options();
    
    // Set selection variables
    max_selection = 2;
    selection = 0;
    
    // Slow game down
    rest(500);
    
    // Wait until user selects a option
    while(1) {
		if (getinput(0) == -1) {
    		break;
    	}
		rest(100);
	};
	
    // Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    // If All Questions were selected
    if (selection == 0) {
    	
    	// Create Question space
    	questions = (Question *) malloc(sizeof(Question) * MAX_QUESTIONS_PER_FILE *  MAX_CHAPTERNUMBER);
    	
    	// Parse questions and set variables
    	parse_all_questions(questions, concat_filepath(folder_topic, QUESTION_FOLDER));
    	num_files = MAX_QUESTIONS_PER_FILE;
    	all_questions = 1;
    }
    // Select by Unit
    else if (selection == 1) {
    	
    	// Create Option space
    	text_options = (Option *) malloc(sizeof(Option) * MAX_UNITNUMBER);
    	
    	// Parse Options
    	parse_options(text_options, concat_filepath(folder_topic, UNIT));
    	
    	// Print options for user
    	print_options(text_options, "Please select all units you wish to be tested on!", MAX_UNITNUMBER, 1);
    	max_selection = MAX_UNITNUMBER;
    }
    else {
    	
    	// Create Option space
    	text_options = (Option *) malloc(sizeof(Option) * MAX_CHAPTERNUMBER);
    	
    	// Parse Options
    	parse_options(text_options, concat_filepath(folder_topic, CHAPTER));
    	
    	// Print options for user
    	print_options(text_options, "Please select all chapters you wish to be tested on!", MAX_CHAPTERNUMBER, 1);
    	max_selection = MAX_CHAPTERNUMBER;
    }
	
	// Check if not "All Questions" was selected
	if (all_questions == 0) {
		
		// Allow user to select multiple options
		while(1) {
			if (getinput(0) == -1) {
				if (selection == max_selection) {
					break;
				}
				else {
					if (text_options[selection].selected == 1) {
						text_options[selection].selected = 0;
						num_files -= text_options[selection].files;
						rect(screen, WIDTH/12, HEIGHT/4 + selection * 15 - 2, WIDTH/12 + 10, HEIGHT/4 + selection * 15 + 8, WHITE);
					}
					else {
						text_options[selection].selected = 1;
						num_files += text_options[selection].files;
						rect(screen, WIDTH/12, HEIGHT/4 + selection * 15 - 2, WIDTH/12 + 10, HEIGHT/4 + selection * 15 + 8, RED);
					}
				}
	    	}
			rest(100);
		};	
		
		// Create Questions space and set initial file variable
		questions = (Question *) malloc(sizeof(Question) * MAX_QUESTIONS_PER_FILE * num_files);
		current_file = -1;
		
		// Cycle through the selection made by user and parse questions
		for (i = 0; i < max_selection; i++) {
			if (text_options[i].selected == 1) {
				for (j = 0; j < text_options[i].files; j++) {
					current_file++;
					parse_questions(&questions[current_file * MAX_QUESTIONS_PER_FILE], concat_filepath(concat_filepath(folder_topic, QUESTION_FOLDER), text_options[i].str_filename[j]));
				}
			}
		}
	}
    
    // Slow down game
    rest(500);
    
    // Initialize Question Phase
    start_questions(questions, num_files * MAX_QUESTIONS_PER_FILE);
    
    // Slow down game
    rest(500);
    
    // Wait until user exits game
    while(1) {
		if (getinput(0) == -1) {
    		break;
    	}
		rest(100);
	};
    
    // Free variables and Exit
    free(text_options);
    free(questions);
    destroy_sample(background);
    destroy_sample(correct_sound);
    destroy_sample(incorrect_sound);
    destroy_bitmap(correct_pic);
    destroy_bitmap(incorrect_pic);
    remove_sound();
	allegro_exit();
	return 0;
}     
END_OF_MAIN()
