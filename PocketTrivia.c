#include <allegro.h>
#include "PocketTrivia.h"
#include <stdio.h>

using namespace std;

// To do:
// Change Questions answers to char* and malloc correct amount of space.
// Do this in conjuction with a cleanup function (free)

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
    
    //Initialize Graphics
    int ret;
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return 0;
    }
    /*
    //Print initial Start Screen with instructions
    textout_centre_ex(screen, font, "Pocket Trivia", WIDTH/2, HEIGHT/4, WHITE, BLACK);
    
    textout_centre_ex(screen, font, "Press ENTER to Start!", WIDTH/2, HEIGHT/2, YELLOW, BLACK);
    
    textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to navigate menu selection", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "2. Use the ENTER key to select an option", WIDTH/8, HEIGHT/2 + 70, WHITE, BLACK);
    textout_ex(screen, font, "3. Then use the NUMBER keys to choose an answer for a question", WIDTH/8, HEIGHT/2 + 80, WHITE, BLACK);
    textout_ex(screen, font, "4. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    
    //Wait until user press Enter
    while(!key[KEY_ENTER]) {
		rest(100);
	};
	// Clear screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    */
    struct Option topics[TOPICNUMBER];
    //topics = (struct Option*) malloc(TOPICNUMBER * sizeof(struct Option));
    
    parse_options(topics, TOPIC);
    
    position = 60;
    textout_ex(screen, font, "Please select a topic!", WIDTH/8, HEIGHT/2 + 50, WHITE, BLACK);
    
    for(i = 0; i < TOPICNUMBER; i++) {
    	position += 10;
    	textprintf_ex(screen, font, WIDTH/8, HEIGHT/2 + position, WHITE, BLACK, "   %s", topics[i].str_name);
    }
    
    
    //textout_ex(screen, font, "2. Use the ENTER key to select an option", WIDTH/8, HEIGHT/2 + 80, WHITE, BLACK);
    //textout_ex(screen, font, "3. Then use the NUMBER keys to choose an answer for a question", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    
    //Wait until user press Enter
    while(!key[KEY_ESC]) {
		rest(100);
	};
    
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
    
    
    //free(question1);
    allegro_exit();
	return 0;
}     
END_OF_MAIN()
