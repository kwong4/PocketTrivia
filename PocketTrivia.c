#include <allegro.h>
#include "PocketTrivia.h"
#include <stdio.h>

using namespace std;

Question parse_questions(const char* textfile) 
{
	FILE *file = fopen(textfile, "r");
	char line[256];
	
	Question question;
	
	int answer = 0;
	
	if (file != NULL) {
		while(fgets(line, sizeof(line), file)) {
			
			if (strncmp(line, "-", 1) == 0) {
				strncpy(question.str_question, &line[1], strcspn(line, "\n") - 1);
			}
			else {
				if (strncmp(line, "*", 1) == 0) {
					question.answer = answer;
				}
				strncpy(question.str_answer[answer], &line[1], strcspn(line, "\n") - 1);
				answer++;
			}
		}
	
		fclose(file);
	}
	else {
		textprintf_ex(screen, font, 0, 70, 15, -1, "%s", "File not found");
	}
	
	return question;
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
    
    Question question1;
    question1 = parse_questions("Questions\\chapter1x.txt");
    
    textprintf_ex(screen, font, 0, 30, 15, -1, "%s", question1.str_question);
    textprintf_ex(screen, font, 0, 40, 15, -1, "A. %s", question1.str_answer[0]);
    textprintf_ex(screen, font, 0, 50, 15, -1, "B. %s", question1.str_answer[1]);
    textprintf_ex(screen, font, 0, 60, 15, -1, "C. %s", question1.str_answer[2]);
    textprintf_ex(screen, font, 0, 70, 15, -1, "D. %s", question1.str_answer[3]);
    
    while(!keypressed()) {
	};
    
    allegro_exit();
	return 0;     
}     
END_OF_MAIN()
