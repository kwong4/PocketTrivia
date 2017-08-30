// Pocket Trivia
// Kevin Wong

#ifndef _POCKETTRIVIA_H
#define _POCKETTRIVIA_H

#include "allegro.h"
#include <string>
using namespace std;

//define some game constants
#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define MAX_WORD 256
#define MAX_ANSWER 4
#define TEXT texts.txt
#define TEXTNUMBER 1
#define UNIT units.txt
#define UNITNUMBER 4
#define CHAPTER chapters.txt
#define CHAPTERNUMBER 22

//define question structure
struct Question
{
	char str_question[MAX_WORD];
	char str_answer[MAX_ANSWER][MAX_WORD];
	int answer;
};

struct Option
{
	char str_name[MAX_WORD];
	char** str_filename;	
};

//functions
void parse_questions(Question *questions, const char* textfile);
#endif
