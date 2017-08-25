#include <allegro.h>
#include <stdio.h>
#include <string.h>

// Make a Question Struct in a .h file
// Make Parse function to grab question from text file
// Make example for 1 Question and see how it goes. Proof of concept.

int main(void)
{
    //initialize Allegro
	allegro_init();
    set_gfx_mode(GFX_SAFE, 640, 480, 0, 0);
    install_keyboard();
    
    while(!keypressed());
    allegro_exit();
	return 0;     
}     
END_OF_MAIN()
