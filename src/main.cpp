#include "Chip8.hpp"
#include <stdio.h>
#include <GL/glut.h>
#include <iostream>

Chip8 prog;

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define DRAWWITHTEXTURE

uint8_t screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3]; 

int modifier = 10;

// Window size
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;

void setupTexture()
{
	// Clear screen
	for(int y = 0; y < SCREEN_HEIGHT; ++y)		
		for(int x = 0; x < SCREEN_WIDTH; ++x)
			screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;

	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void updateTexture(const Chip8& c8)
{	
	// Update pixels
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
			if(c8.gfx[(y * 64) + x] == 0)
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;	// Disabled
			else 
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255;  // Enabled
		
	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
	//std::cout << "updateTexture" << std::endl;

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
	glEnd();
}

void display() {
    prog.emulateCycle();
	//std::cout << "in display()" << std::endl;

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


void reshape_window(GLsizei w, GLsizei h) {
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		prog.key[0x1] = 1;
	else if(key == '2')	prog.key[0x2] = 1;
	else if(key == '3')	prog.key[0x3] = 1;
	else if(key == '4')	prog.key[0xC] = 1;

	else if(key == 'q')	prog.key[0x4] = 1;
	else if(key == 'w')	prog.key[0x5] = 1;
	else if(key == 'e')	prog.key[0x6] = 1;
	else if(key == 'r')	prog.key[0xD] = 1;

	else if(key == 'a')	prog.key[0x7] = 1;
	else if(key == 's')	prog.key[0x8] = 1;
	else if(key == 'd')	prog.key[0x9] = 1;
	else if(key == 'f')	prog.key[0xE] = 1;

	else if(key == 'z')	prog.key[0xA] = 1;
	else if(key == 'x')	prog.key[0x0] = 1;
	else if(key == 'c')	prog.key[0xB] = 1;
	else if(key == 'v')	prog.key[0xF] = 1;
}

void keyboardUp(unsigned char key, int x, int y)
{

	if(key == '1')		prog.key[0x1] = 0;
	else if(key == '2')	prog.key[0x2] = 0;
	else if(key == '3')	prog.key[0x3] = 0;
	else if(key == '4')	prog.key[0xC] = 0;

	else if(key == 'q')	prog.key[0x4] = 0;
	else if(key == 'w')	prog.key[0x5] = 0;
	else if(key == 'e')	prog.key[0x6] = 0;
	else if(key == 'r')	prog.key[0xD] = 0;

	else if(key == 'a')	prog.key[0x7] = 0;
	else if(key == 's')	prog.key[0x8] = 0;
	else if(key == 'd')	prog.key[0x9] = 0;
	else if(key == 'f')	prog.key[0xE] = 0;

	else if(key == 'z')	prog.key[0xA] = 0;
	else if(key == 'x')	prog.key[0x0] = 0;
	else if(key == 'c')	prog.key[0xB] = 0;
	else if(key == 'v')	prog.key[0xF] = 0;
}



int main(int argc, char **argv) {
    if(argc < 2)
	{
		printf("Usage: ./chip8 prog_name\n");
		return 1;
	}

    // set up graphics
    // set up input

    prog.init();
    prog.loadGame(argv[1]);
    glutInit(&argc, argv);

    glutInitWindowSize(SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10);
    glutInitWindowPosition(0, 200);
    glutCreateWindow("Chip-8 Emulator");

    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutReshapeFunc(reshape_window);        
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);

#ifdef DRAWWITHTEXTURE
	setupTexture();			
#endif	

	glutMainLoop(); 
    
    // std::string line;

    // while (std::getline(std::cin, line)) {
    //     prog.emulateCycle();

    //     prog.setKeys();

    // }
    return 0;
}