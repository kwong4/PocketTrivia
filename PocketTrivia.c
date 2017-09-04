#include <allegro.h>
#include "PocketTrivia.h"
#include <stdio.h>

using namespace std;

// To do:
// Change Questions answers to char* and malloc correct amount of space.
// Do this in conjuction with a cleanup function (free)

int getinput() {
	if (key[KEY_ESC]) {
		allegro_exit();
		exit(0);
	}
	
	if (key[KEY_DOWN] && selection != max_selection) {
		rectfill(screen, WIDTH/8 + 1, HEIGHT/4 + SELECTION_BUFFER + selection * 15 - 1, WIDTH/8 + 9, HEIGHT/4 + SELECTION_BUFFER + selection * 15 + 7, BLACK);
		selection++;
		rectfill(screen, WIDTH/8 + 1, HEIGHT/4 + SELECTION_BUFFER + selection * 15 - 1, WIDTH/8 + 9, HEIGHT/4 + SELECTION_BUFFER + selection * 15 + 7, WHITE);
	}
	else if (key[KEY_UP] && selection != 0) {
		rectfill(screen, WIDTH/8 + 1, HEIGHT/4 + SELECTION_BUFFER + selection * 15 - 1, WIDTH/8 + 9, HEIGHT/4 + SELECTION_BUFFER + selection * 15 + 7, BLACK);
		selection--;
		rectfill(screen, WIDTH/8 + 1, HEIGHT/4 + SELECTION_BUFFER + selection * 15 - 1, WIDTH/8 + 9, HEIGHT/4 + SELECTION_BUFFER + selection * 15 + 7, WHITE);
	}
	else if (key[KEY_ENTER]) {
		return 1;
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
				strncpy(questions[current_question].str_question, &line[1], end_position);
				strncpy(&questions[current_question].str_question[end_position], "\0", 1);
			}
			else {
				if (strncmp(line, "*", 1) == 0) {
					questions[current_question].answer = answer;
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

int main(void)
{
	
    //Initialize Allegro
	allegro_init();
    install_keyboard();
    install_timer();
    srand(time(NULL));
    set_color_depth(desktop_color_depth());
    
    //Initalize variables
    int i;
    int position;
    int current_file;
    int end_position;
    
    //Initialize Graphics
    int ret;
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return 0;
    }
    
    //Print initial Start Screen with instructions
    textout_centre_ex(screen, font, "Pocket Trivia", WIDTH/2, HEIGHT/4, WHITE, BLACK);
    
    textout_centre_ex(screen, font, "Press ENTER to Start!", WIDTH/2, HEIGHT/2, YELLOW, BLACK);
    
    textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to navigate menu selection", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "2. Use the ENTER key to select an option", WIDTH/8, HEIGHT/2 + 70, WHITE, BLACK);
    textout_ex(screen, font, "3. Then use the NUMBER keys to choose an answer for a question", WIDTH/8, HEIGHT/2 + 80, WHITE, BLACK);
    textout_ex(screen, font, "4. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    
    //Wait until user press Enter
    while(1) {
    	if (key[KEY_ENTER]) {
    		break;
    	}
    	else if (key[KEY_ESC]) {
    		allegro_exit();
    		exit(0);
    	}
		rest(100);
	};
	
	// Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    struct Option topics[TOPICNUMBER];
    //topics = (struct Option*) malloc(TOPICNUMBER * sizeof(struct Option));
    
    parse_options(topics, TOPIC);
    
    position = SELECTION_BUFFER;
    textout_ex(screen, font, "Please select a topic!", WIDTH/8, HEIGHT/4 + SELECTION_BUFFER - 20, WHITE, BLACK);
    
    for(i = 0; i < TOPICNUMBER; i++) {
    	rect(screen, WIDTH/8, HEIGHT/4 + position - 2, WIDTH/8 + 10, HEIGHT/4 + position + 8, WHITE);
    	textprintf_ex(screen, font, WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK, "%s", topics[i].str_name);
    	position += 15;
    }
    
    rectfill(screen, WIDTH/8 + 1, HEIGHT/4 + SELECTION_BUFFER - 1, WIDTH/8 + 9, HEIGHT/4 + SELECTION_BUFFER + 7, WHITE);
    max_selection = TOPICNUMBER - 1;
    //textout_ex(screen, font, "2. Use the ENTER key to select an option", WIDTH/8, HEIGHT/2 + 80, WHITE, BLACK);
    //textout_ex(screen, font, "3. Then use the NUMBER keys to choose an answer for a question", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    rest(1000);
    //Wait until user press Enter
    while(1) {
    	if (getinput() == 1) {
    		break;
    	}
		rest(100);
	};
    
    rest(1000);
    
    // Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    strcpy(folder_topic, topics[selection].str_filename[0]);
    
    position = SELECTION_BUFFER;
    textout_ex(screen, font, "Please select an option to choose your questions!", WIDTH/8, HEIGHT/4 + SELECTION_BUFFER - 20, WHITE, BLACK);
    
    rect(screen, WIDTH/8, HEIGHT/4 + position - 2, WIDTH/8 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "All Questions (Whole textbook)", WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK);
    position += 15;
    rect(screen, WIDTH/8, HEIGHT/4 + position - 2, WIDTH/8 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "Choose by Unit", WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK);
    position += 15;
    rect(screen, WIDTH/8, HEIGHT/4 + position - 2, WIDTH/8 + 10, HEIGHT/4 + position + 8, WHITE);
    textout_ex(screen, font, "Choose by Chapter", WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK);
    
    rectfill(screen, WIDTH/8 + 1, HEIGHT/4 + SELECTION_BUFFER - 1, WIDTH/8 + 9, HEIGHT/4 + SELECTION_BUFFER + 7, WHITE);
    
    max_selection = 2;
    selection = 0;
    
    //position += 15;
    
    //textprintf_ex(screen, font, WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK, "%s", concat(folder_topic, "test"));
    
    rest(1000);
    
    while(1) {
		if (getinput() == 1) {
    		break;
    	}
		rest(100);
	};
    // Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    current_file = 0;
    if (selection == 0) {
    	questions = (Question *) malloc(sizeof(Question) * MAX_CHAPTERNUMBER);
    	FILE *file = fopen(concat(folder_topic, TEXT), "r");
    	char line[256];
    	
    	if (file != NULL) {
	    	while(fgets(line, sizeof(line), file)) {
	    		end_position = strcspn(line, "\n");
	    		strncpy(&line[end_position], "\0", 1);
	    		textprintf_ex(screen, font, WIDTH/8 + 24, HEIGHT/4 + position + 40, WHITE, BLACK, "%s", concat(folder_topic, concat(QUESTION_FOLDER, line)));
	    		while(!key[KEY_LEFT]) {
	    			rest(100);	
	    		}
	    		parse_questions(&questions[current_file * MAX_QUESTIONS_PER_FILE], concat(folder_topic, concat(QUESTION_FOLDER, line)));
	    	}
    	}
    	else {
    		while(!key[KEY_LEFT]) {
    			rest(100);	
    		}
    		allegro_exit();
		    return 0;
    	}
    	fclose(file);
    	/*
	    	for (i = 0; i < MAX_CHAPTERNUMBER; i++) {
	    		current_file = 0;
	    		FILE *file = fopen(concat(folder_topic, TEXT), "r");
	    		if (file != NULL) {
					char line[256];
					while(fgets(line, sizeof(line), file)) {
						
					}
					int current_option = -1;
					int end_position = 0;
					int current_file = 0;
					int i;
				}
				else {
		    		allegro_exit();
		    		return 0;
		    	}
	    	}
    	fclose(file);
    	*/
    	//parse_questions(questions, concat(folder_topic, TEXT));
    	//parse_questions(questions, 
    	//load all
    	//use concat
    	textprintf_ex(screen, font, 0, 30, WHITE, BLACK, "%s", questions[0].str_question);
	    textprintf_ex(screen, font, 0, 40, WHITE, BLACK, "A. %s", questions[0].str_answer[0]);
	    textprintf_ex(screen, font, 0, 50, WHITE, BLACK, "B. %s", questions[0].str_answer[1]);
	    textprintf_ex(screen, font, 0, 60, WHITE, BLACK, "C. %s", questions[0].str_answer[2]);
	    textprintf_ex(screen, font, 0, 70, WHITE, BLACK, "D. %s", questions[0].str_answer[3]);
    }
    else if (selection == 1) {
    	struct Option units[MAX_UNITNUMBER];
    	//questions = (Question *) malloc(sizeof(Question) * MAX_UNITNUMBER);
    	position = 0;
    	textout_ex(screen, font, "Please select all units you wish to be tested on!", WIDTH/8, HEIGHT/4 - 20, WHITE, BLACK);
    	parse_options(units, concat(folder_topic, UNIT));
    	for (i = 0; i < MAX_UNITNUMBER; i++) {
    		rect(screen, WIDTH/8, HEIGHT/4 + position - 2, WIDTH/8 + 10, HEIGHT/4 + position + 8, WHITE);
	    	textprintf_ex(screen, font, WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK, "%s", units[i].str_name);
	    	position += 15;
    	}
    }
    else {
    	struct Option chapters[MAX_CHAPTERNUMBER];
    	//questions = (Question *) malloc(sizeof(Question) * MAX_UNITNUMBER);
    	position = 0;
    	textout_ex(screen, font, "Please select all chapters you wish to be tested on!", WIDTH/8, HEIGHT/4 - 20, WHITE, BLACK);
    	parse_options(chapters, concat(folder_topic, CHAPTER));
    	for (i = 0; i < MAX_CHAPTERNUMBER; i++) {
    		rect(screen, WIDTH/8, HEIGHT/4 + position - 2, WIDTH/8 + 10, HEIGHT/4 + position + 8, WHITE);
	    	textprintf_ex(screen, font, WIDTH/8 + 24, HEIGHT/4 + position, WHITE, BLACK, "%s", chapters[i].str_name);
	    	position += 15;
    	}
    }
    
    /*
    Question* question1;
    question1 = (struct Question*) malloc(1 * sizeof(struct Question));
    parse_questions(question1, "Questions\\chapter1x.txt");
    
    textprintf_ex(screen, font, 0, 30, WHITE, BLACK, "%s", question1->str_question);
    textprintf_ex(screen, font, 0, 40, WHITE, BLACK, "A. %s", question1->str_answer[0]);
    textprintf_ex(screen, font, 0, 50, WHITE, BLACK, "B. %s", question1->str_answer[1]);
    textprintf_ex(screen, font, 0, 60, WHITE, BLACK, "C. %s", question1->str_answer[2]);
    textprintf_ex(screen, font, 0, 70, WHITE, BLACK, "D. %s", question1->str_answer[3]);
    */
    
    while(!key[KEY_ESC]) {
		rest(100);
	};
    free(questions);
    //free(question1);
    allegro_exit();
	return 0;
}     
END_OF_MAIN()
