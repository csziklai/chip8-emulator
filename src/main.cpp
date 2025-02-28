#include "Chip8.hpp"
#include <stdio.h>
#include <GL/glut.h>

Chip8 prog;
#define DRAWWITHTEXTURE

void display() {
    prog.emulateCycle();

    if (prog.drawFlag) {
        glClear(GL_COLOR_BUFFER_BIT); //not sure about what this is doing

    #ifdef DRAWWITHTEXTURE 
        updateTexture(prog);
    #else
        updateQuads(prog);
    #endif

        glutSwapBuffers();

        prog.drawFlag = false;
        
    }

}


int main(int argc, char **argv) {

    // set up graphics
    // set up input

    prog.init();
    prog.loadGame(argv[1]);
    glutInit(&argc, argv);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("Chip-8 Emulator");

    glutDisplayFunc(display);
    glutIdleFunc(display)

    // TODO need to define the following functions
    // glutReshapeFunc(reshape_window);        
	// glutKeyboardFunc(keyboardDown);
	// glutKeyboardUpFunc(keyboardUp);

#ifdef DRAWWITHTEXTURE
	setupTexture();			
#endif	

	glutMainLoop(); 

	return 0;
    


    // std::string line;

    // while (std::getline(std::cin, line)) {
    //     prog.emulateCycle();

    //     prog.setKeys();

    // }
    return 0;
}