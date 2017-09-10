#include <allegro.h>
#include "PocketTrivia.h"
#include <stdio.h>

using namespace std;

// To do:
// Change Questions answers to char* and malloc correct amount of space.
// Do this in conjuction with a cleanup function (free)

int getinput(int gamemode) {
	if (key[KEY_ESC]) {
		allegro_exit();
		exit(0);
	}
	
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
	
	return 0;
}

void parse_options(struct Option options[], const char* textfile) {
	
	FILE *file = fopen(textfile, "r");
	char line[256];
	int current_option = -1;
	int end_position = 0;
	int current_file = 0;
	int i;
	if (file != NULL) {
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

char* concat(const char* foldername, const char* filename) 
{
	char* result = (char*) malloc(strlen(foldername) + strlen(filename) + 2);
	strcpy(result, foldername);
	strcat(result, "\\");
	strcat(result, filename);
	return result;
}

void parse_questions(Question questions[], const char* textfile) 
{
	FILE *file = fopen(textfile, "r");
	char line[MAX_WORD];
	
	int answer = 0;
	int end_position = 0;
	int current_question = -1;
	
	if (file != NULL) {
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

void draw_startscreen() {
	textout_centre_ex(screen, font, "Pocket Trivia", WIDTH/2, HEIGHT/4, WHITE, BLACK);
    textout_centre_ex(screen, font, "Press ENTER to Start!", WIDTH/2, HEIGHT/2, YELLOW, BLACK);
    textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to navigate menu selection", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "2. Use the ENTER key to select an option", WIDTH/8, HEIGHT/2 + 70, WHITE, BLACK);
    textout_ex(screen, font, "3. Then use the NUMBER keys to choose an answer for a question", WIDTH/8, HEIGHT/2 + 80, WHITE, BLACK);
    textout_ex(screen, font, "4. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
}

void print_options(Option options[], const char* title, int max, int cont) {
	int i;
	int position = 0;
	
	// Print title
	textprintf_ex(screen, font, WIDTH/12, HEIGHT/4 - 20, WHITE, BLACK, "%s", title);
	
	for(i = 0; i < max; i++) {
    	rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    	textprintf_ex(screen, font, WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK, "%s", options[i].str_name);
    	position += 15;
    }
    
    if (cont == 1) {
    	rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
	    textout_ex(screen, font, "Continue", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    }
    
    selection = 0;
    rectfill(screen, WIDTH/12 + 1, HEIGHT/4 - 1, WIDTH/12 + 9, HEIGHT/4 + 7, WHITE);
}

void print_textbook_options() {
	int position = 0;
    textout_ex(screen, font, "Please select an option to choose your questions!", WIDTH/12, HEIGHT/4 - 20, WHITE, BLACK);
    
    rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "All Questions (Whole textbook)", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    position += 15;
    rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "Choose by Unit", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    position += 15;
    rect(screen, WIDTH/12, HEIGHT/4 + position - 2, WIDTH/12 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "Choose by Chapter", WIDTH/12 + 24, HEIGHT/4 + position, WHITE, BLACK);
    
    rectfill(screen, WIDTH/12 + 1, HEIGHT/4 - 1, WIDTH/12 + 9, HEIGHT/4 + 7, WHITE);
}

void parse_all_questions(Question questions[], const char* folderpath) {
	FILE *file = fopen(concat(folder_topic, TEXT), "r");
	char line[256];
	int end_position;
	int current_file = 0;
	
	if (file != NULL) {
    	while(fgets(line, sizeof(line), file)) {
    		end_position = strcspn(line, "\n");
    		strncpy(&line[end_position], "\0", 1);
    		parse_questions(&questions[current_file * MAX_QUESTIONS_PER_FILE], concat(folderpath, line));
    	}
	}
	else {
		allegro_exit();
	}
	fclose(file);
}

void start_questions(Question questions[], int num_questions) {
	int i;
	int correct = 0;
	int user_guess;
	for (i = 0; i < num_questions; i++) {
		
		rest(500);
		// Clear screen
    	rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    	textprintf_centre_ex(screen, font, WIDTH/2, 0, WHITE, BLACK, "User Score: %i/%i", correct, i);
		textprintf_ex(screen, font, 30, 30, WHITE, BLACK, "%s", questions[i].str_question);
		textprintf_ex(screen, font, 30, 40, WHITE, BLACK, "A. %s", questions[i].str_answer[0]);
	    textprintf_ex(screen, font, 30, 50, WHITE, BLACK, "B. %s", questions[i].str_answer[1]);
	    textprintf_ex(screen, font, 30, 60, WHITE, BLACK, "C. %s", questions[i].str_answer[2]);
	    textprintf_ex(screen, font, 30, 70, WHITE, BLACK, "D. %s", questions[i].str_answer[3]);
	    
	    while(1) {
	    	user_guess = getinput(1);
	    	if (user_guess > 0) {
	    		if (user_guess == questions[i].answer) {
	    			correct++;	
	    		}
	    		break;
	    	}
	    }
	}
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
    
    //Initialize Graphics
    int ret;
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return 0;
    }
    
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
    
    max_selection = TOPICNUMBER - 1;

    rest(1000);
    
    //Wait until user press Enter
    while(1) {
    	if (getinput(0) == -1) {
    		break;
    	}
		rest(100);
	};
    
    rest(1000);
    
    // Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    strcpy(folder_topic, topics[selection].str_filename[0]);
    
    // Print textbook options for user
    print_textbook_options();
    
    max_selection = 2;
    selection = 0;
    
    rest(1000);
    
    while(1) {
		if (getinput(0) == -1) {
    		break;
    	}
		rest(100);
	};
	
    // Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    if (selection == 0) {
    	questions = (Question *) malloc(sizeof(Question) * MAX_QUESTIONS_PER_FILE *  MAX_CHAPTERNUMBER);
    	
    	parse_all_questions(questions, concat(folder_topic, QUESTION_FOLDER));
    	
    	num_files = MAX_QUESTIONS_PER_FILE;
    	
    	all_questions = 1;
    }
    else if (selection == 1) {
    	text_options = (Option *) malloc(sizeof(Option) * MAX_UNITNUMBER);
    	parse_options(text_options, concat(folder_topic, UNIT));
    	
    	// Print options for user
    	print_options(text_options, "Please select all units you wish to be tested on!", MAX_UNITNUMBER, 1);
    	max_selection = MAX_UNITNUMBER;
    }
    else {
    	text_options = (Option *) malloc(sizeof(Option) * MAX_CHAPTERNUMBER);
    	parse_options(text_options, concat(folder_topic, CHAPTER));
    	
    	// Print options for user
    	print_options(text_options, "Please select all chapters you wish to be tested on!", MAX_CHAPTERNUMBER, 1);
    	max_selection = MAX_CHAPTERNUMBER;
    }
	
	
	
	if (all_questions == 0) {
		
		rest(500);
		
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
		
		questions = (Question *) malloc(sizeof(Question) * MAX_QUESTIONS_PER_FILE * num_files);
		current_file = -1;
		
		for (i = 0; i < max_selection; i++) {
			if (text_options[i].selected == 1) {
				for (j = 0; j < text_options[i].files; j++) {
					current_file++;
					parse_questions(&questions[current_file * MAX_QUESTIONS_PER_FILE], concat(concat(folder_topic, QUESTION_FOLDER), text_options[i].str_filename[j]));
				}
			}
		}
	}
    
    rest(1000);
    
    start_questions(questions, num_files * MAX_QUESTIONS_PER_FILE);
    
    free(text_options);
    free(questions);
	allegro_exit();
	return 0;
}     
END_OF_MAIN()
