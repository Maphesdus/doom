//------------------------------------------------------------------------------
//--------------------------Code By: 3DSage-------------------------------------
//----------------Video tutorial on YouTube-3DSage------------------------------
//------------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <GL/glut.h> 

#define resolution		1               		//0=160x120 1=360x240 4=640x480
#define ScreenWidth		160 * resolution		//screen width
#define ScreenHeight	120 * resolution		//screen height
#define ScreenWidth2	(ScreenWidth / 2)		//half of screen width
#define ScreenHeight2	(ScreenHeight / 2)		//half of screen height
#define pixelScale		4 / resolution				//OpenGL pixel scale
#define GL_ScreenWidth	(ScreenWidth * pixelScale)	//OpenGL window width
#define GL_ScreenHeight	(ScreenHeight * pixelScale)	//OpenGL window height
//------------------------------------------------------------------------------
typedef struct 
{
 int frame1, frame2;	//frame 1 frame 2, to create constant frame rate
}time; time Time;

typedef struct 
{
 int w, s, a, d;           		//move up, down, left, right
 int strafe_left, strafe_right;	//strafe left, right 
 int move;						//move up, down, look up, down
}keys; keys Key;

typedef struct
{
	float cos[360];		// Save sin cos in values 0-360 degrees
	float sin[360];
} math; math Math;

typedef struct
{
	int x, y, z;		// Player position. Z is up
	int angle;			// Player angle of rotation left right
	int look;			// Variable to look up and down
} player; player Player;

//------------------------------------------------------------------------------

void pixel(int x, int y, int c)	//draw a pixel at x/y with rgb
{
	int rgb[3];
	
	if(c==0){ rgb[0]=255; rgb[1]=255; rgb[2]=  0;} //Yellow	
	if(c==1){ rgb[0]=160; rgb[1]=160; rgb[2]=  0;} //Yellow darker	
	if(c==2){ rgb[0]=  0; rgb[1]=255; rgb[2]=  0;} //Green	
	if(c==3){ rgb[0]=  0; rgb[1]=160; rgb[2]=  0;} //Green darker	
	if(c==4){ rgb[0]=  0; rgb[1]=255; rgb[2]=255;} //Cyan	
	if(c==5){ rgb[0]=  0; rgb[1]=160; rgb[2]=160;} //Cyan darker
	if(c==6){ rgb[0]=160; rgb[1]=100; rgb[2]=  0;} //brown	
	if(c==7){ rgb[0]=110; rgb[1]= 50; rgb[2]=  0;} //brown darker
	if(c==8){ rgb[0]=  0; rgb[1]= 60; rgb[2]=130;} //background
	
	glColor3ub(rgb[0], rgb[1], rgb[2]); 
	glBegin(GL_POINTS);
	glVertex2i(x * pixelScale + 2, y  *pixelScale + 2);
	glEnd();
}

void movePlayer()
{
	//move up, down, left, right
	
	// Move Left:
	if(Key.a == 1 && Key.move == 0){
		printf("left\n");
		Player.angle -= 4;
			if(Player.angle < 0){
				Player.angle += 360;
			}
	}
	
	// Move Right:
	if(Key.d == 1 && Key.move == 0){
		printf("right\n");
		Player.angle += 4;
		if(Player.angle < 359){
			Player.angle -= 360;
		}
	}
	
	int direction_x = Math.sin[Player.angle] * 10.0;
	int direction_y = Math.cos[Player.angle] * 10.0; 
	
	// Move Forward:
	if(Key.w == 1 && Key.move == 0){
		printf("up\n");
		Player.x += direction_x;
		Player.y += direction_y;
	}
	
	// Move Backward:
	if(Key.s == 1 && Key.move == 0){
		printf("down\n");
		Player.x -= direction_x;
		Player.y -= direction_y;
	}
	
	//strafe right:
	if(Key.strafe_right == 1)
	{
		printf("strafe left\n");
		Player.x += direction_y;
		Player.y -= direction_x;
	}
	
	//strafe left:
	if(Key.strafe_left == 1)
	{
		printf("strafe right\n");
		Player.x -= direction_y;
		Player.y += direction_x;
	}
	
	//move up, down, look up, look down
	
	// Look Up:
	if(Key.a == 1 && Key.move == 1)
	{
		printf("look up\n");
		Player.look -= 1;
	}
	
	// Look Down:
	if(Key.d == 1 && Key.move == 1)
	{
		printf("look down\n");
		Player.look += 1;
	}
	
	// Move Up:
	if(Key.w == 1 && Key.move == 1)
	{
		printf("move up\n");
		Player.z -= 4;
	}
	
	// Move Down:
	if(Key.s == 1 && Key.move == 1)
	{
		printf("move down\n");
		Player.z += 4;
	}
}

void clearBackground() 
{
	int x, y;
	for(y = 0; y < ScreenHeight; y++)
	{ 
	 	//clear background color
		for(x = 0; x < ScreenWidth; x++)
		{
			pixel(x, y, 8);
		}
	}	
}


void draw3D()
{
	int wall_x[4], wall_y[4], wall_z[4];
	float COSIN = Math.cos[Player.angle];
	float SIN = Math.sin[Player.angle];
	
	// Offset bottom 2 points by player
	int x1 = 40 - Player.x;
	int y1 = 10 - Player.y;
	int x2 = 40 - Player.x;
	int y2 = 290 - Player.y;
	
	// World X position (width)
	wall_x[0] = x1 * COSIN - y1 * SIN;
	wall_x[1] = x2 * COSIN - y2 * SIN;
	
	// World Y position (depth)
	wall_x[0] = y1 * COSIN + x1 * SIN;
	wall_x[1] = y2 * COSIN + x2 * SIN;
	
	// World Z position (height)
	wall_z[0] = 0 - Player.z + ((Player.look * wall_y[0]) / 32.0);
	wall_z[1] = 0 - Player.z + ((Player.look * wall_y[1]) / 32.0);
	
	// screen x position
	wall_x[0] = wall_x[0] * 200 / wall_y[0] + ScreenWidth2;
	wall_y[0] = wall_z[0] * 200 / wall_y[0] + ScreenHeight2;
	
	// screen y position
	wall_x[1] = wall_x[1] * 200 / wall_y[1] + ScreenWidth2;
	wall_y[1] = wall_z[1] * 200 / wall_y[1] + ScreenHeight2;
	
	//pixel(wall_x[0], wall_y[0], 0);
	//pixel(wall_x[1], wall_y[1], 0);
	
	// draw point 1
	if(wall_x[0] > 0 && wall_x[0] < ScreenWidth && wall_y[0] > 0 && wall_y[0] < ScreenHeight)
	{
		pixel(wall_x[0], wall_y[0], 0);
	}
	
	// draw point 2
	if(wall_x[1] > 0 && wall_x[1] < ScreenWidth && wall_y[1] > 0 && wall_y[1] < ScreenHeight)
	{
		pixel(wall_x[1], wall_y[1], 0);
	}
}

void display() 
{
	int x,y;
	
	if(Time.frame1 - Time.frame2 >= 50)		//only draw 20 frames/second
	{ 
		clearBackground();
		movePlayer();
		draw3D(); 
		
		Time.frame2 = Time.frame1;   
		glutSwapBuffers(); 
		glutReshapeWindow(GL_ScreenWidth, GL_ScreenHeight);	//prevent window scaling
	}

	Time.frame1 = glutGet(GLUT_ELAPSED_TIME);	//1000 Milliseconds per second
	glutPostRedisplay();
} 

void KeysDown(unsigned char key,int x,int y)   
{ 
 if(key=='w'==1){ Key.w = 1;} 
 if(key=='s'==1){ Key.s = 1;} 
 if(key=='a'==1){ Key.a = 1;} 
 if(key=='d'==1){ Key.d = 1;} 
 if(key=='m'==1){ Key.move = 1;} 
 if(key==','==1){ Key.strafe_right = 1;} 
 if(key=='.'==1){ Key.strafe_left = 1;} 
}

void KeysUp(unsigned char key,int x,int y)
{ 
 if(key=='w'==1){ Key.w = 0;}
 if(key=='s'==1){ Key.s = 0;}
 if(key=='a'==1){ Key.a = 0;}
 if(key=='d'==1){ Key.d = 0;}
 if(key=='m'==1){ Key.move = 0;}
 if(key==','==1){ Key.strafe_right = 0;} 
 if(key=='.'==1){ Key.strafe_left = 0;}
}

void init()
{
	int x;
	
	//store sin/cos in degrees 
	for (x = 0; x < 360; x++){
		Math.cos[x] = cos(x / 180.0 * M_PI);
		Math.sin[x] = sin(x / 180.0 * M_PI);
	}
	
	//init player variables
	Player.x = 70;
	Player.y = -110;
	Player.z = 20;
	Player.angle = 0;
	Player.look = 0;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(GL_ScreenWidth / 2, GL_ScreenHeight / 2);
	glutInitWindowSize(GL_ScreenWidth, GL_ScreenHeight);
	glutCreateWindow(""); 
	glPointSize(pixelScale);                        	//pixel size
	gluOrtho2D(0, GL_ScreenWidth, 0, GL_ScreenHeight);	//origin bottom left
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(KeysDown);
	glutKeyboardUpFunc(KeysUp);
	glutMainLoop();
	return 0;
}
