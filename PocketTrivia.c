#include <allegro.h>
#include "PocketTrivia.h"
#include <stdio.h>

using namespace std;

// To do:
// Change Questions answers to char* and malloc correct amount of space.
// Do this in conjuction with a cleanup function (free)

void parse_questions(Question *questions, const char* textfile) 
{
	FILE *file = fopen(textfile, "r");
	char line[256];
	
	int answer = 0;
	int end_position = 0;
	
	if (file != NULL) {
		while(fgets(line, sizeof(line), file)) {
			end_position = strcspn(line, "\n") - 1;
			
			if (strncmp(line, "-", 1) == 0) {
				strncpy(questions->str_question, &line[1], end_position);
				strncpy(&questions->str_question[end_position], "\0", 1);
			}
			else {
				if (strncmp(line, "*", 1) == 0) {
					questions->answer = answer;
				}
				strncpy(questions->str_answer[answer], &line[1], end_position);
				strncpy(&questions->str_answer[answer][end_position], "\0", 1);
				answer++;
			}
		}
	
		fclose(file);
	}
	else {
		textprintf_ex(screen, font, 0, 70, 15, -1, "%s", "File not found");
	}
}

int main(void)
{
    //initialize Allegro
	allegro_init();
    install_keyboard();
    install_timer();
    srand(time(NULL));
    
    int ret;
    
    printf("test\n");
    
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return 0;
    }
    
    Question* question1;
    question1 = (struct Question*) malloc(1 * sizeof(struct Question));
    parse_questions(question1, "Questions\\chapter1x.txt");
    
    textprintf_ex(screen, font, 0, 30, 15, -1, "%s", question1->str_question);
    textprintf_ex(screen, font, 0, 40, 15, -1, "A. %s", question1->str_answer[0]);
    textprintf_ex(screen, font, 0, 50, 15, -1, "B. %s", question1->str_answer[1]);
    textprintf_ex(screen, font, 0, 60, 15, -1, "C. %s", question1->str_answer[2]);
    textprintf_ex(screen, font, 0, 70, 15, -1, "D. %s", question1->str_answer[3]);
    
    while(!keypressed()) {
	};
    
    free(question1);
    allegro_exit();
	return 0;
}     
END_OF_MAIN()
