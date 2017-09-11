// Pocket Trivia
// Kevin Wong

#ifndef _POCKETTRIVIA_H
#define _POCKETTRIVIA_H

#include "allegro.h"
#include <string>
using namespace std;

//define some game constants
#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 800
#define HEIGHT 600
#define WHITE makecol(255,255,255)
#define YELLOW makecol(255,216,0)
#define RED makecol(255,0,0)
#define BLACK makecol(0,0,0)
#define MAX_WORD 256
#define MAX_ANSWER 4
#define MAX_FILE 10
#define TOPIC "topics.txt"
#define TOPICNUMBER 2
#define TEXT "texts.txt"
#define TEXTNUMBER 1
#define UNIT "units.txt"
#define MAX_UNITNUMBER 4
#define CHAPTER "chapters.txt"
#define QUESTION_FOLDER "Questions"
#define MAX_CHAPTERNUMBER 22
#define MAX_QUESTIONS_PER_FILE 10
#define SELECTION_BUFFER 70
#define CHAR_PER_LENGTH 8

int selection;
int max_selection;
char folder_topic[MAX_WORD];

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
	char str_filename[MAX_FILE][MAX_WORD];	
	int selected;
	int files;
};

Question *questions;
Option *text_options;

//functions
void parse_questions(Question *questions, const char* textfile);
#endif
