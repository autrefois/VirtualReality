// Biluta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define HEAD_HEIGHT 1.2
#define HEAD_RADIUS 1.0

#define TORSO_HEIGHT 4.5
#define TORSO_RADIUS 1.0

#define UPPER_ARM_LENGTH 2.0
#define UPPER_ARM_RADIUS  0.5
#define LOWER_ARM_LENGTH 1.0
#define LOWER_ARM_RADIUS  0.5

#define UPPER_LEG_LENGTH 3.0
#define UPPER_LEG_RADIUS  0.5
#define LOWER_LEG_LENGTH 2.0
#define LOWER_LEG_RADIUS  0.5

static GLfloat body[11] = {0.0, 0.0, 0.0, // torso, head
	                       180.0,	0.0,  // left arm
						   180.0,   0.0, // right arm
						   180.0,   0.0,  // left leg
						   180.0, 0.0  // right leg
                           };  

int speed = 5;

int up = 1;
int left_leg = 1;
int max_leg_angle = 90;
float position_x = 0.0;
float position_y = 0.0;
float position_z = 0.0;
float moving_direction = 0.0;

GLUquadricObj *t, *h, *lua, *lla, *rua, *rla, *lll, *rll, *rul, *lul;

double size=1.0;

void torso()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(t, TORSO_RADIUS, TORSO_RADIUS, TORSO_HEIGHT, 100, 100);
   glPopMatrix();
}

void head()
{
   glPushMatrix();
   glTranslatef(0.0, 0.7*HEAD_HEIGHT, 0.0);
   glScalef(HEAD_RADIUS, HEAD_HEIGHT, HEAD_RADIUS);
   gluSphere(h, 1.0, 100, 100);
   glPopMatrix();
}

void left_upper_arm()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_LENGTH, 100, 100);
   glPopMatrix();
}

void left_lower_arm()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(lla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_LENGTH, 100, 100);
   glPopMatrix();
}

void right_upper_arm()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_LENGTH, 100, 100);
   glPopMatrix();
}

void right_lower_arm()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(rla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_LENGTH, 100, 100);
   glPopMatrix();
}

void left_upper_leg()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_LENGTH, 100, 100);
   glPopMatrix();
}

void left_lower_leg()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(lll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_LENGTH, 100, 100);
   glPopMatrix();
}

void right_upper_leg()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_LENGTH, 100, 100);
   glPopMatrix();
}

void right_lower_leg()
{
   glPushMatrix();
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   gluCylinder(rll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_LENGTH, 100, 100);
   glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.0, 0.0, 0.0);

	glTranslatef(position_x, position_y, position_z);
    glRotatef(body[0], 0.0, 1.0, 0.0);
    torso();
    glPushMatrix();

    glTranslatef(0.0, TORSO_HEIGHT + 0.5 * HEAD_HEIGHT, 0.0);
    glRotatef(body[1], 1.0, 0.0, 0.0);
    glRotatef(body[2], 0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.5 * HEAD_HEIGHT, 0.0);
    head();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-(TORSO_RADIUS + UPPER_ARM_RADIUS), 0.9 * TORSO_HEIGHT, 0.0);
    glRotatef(body[3], 1.0, 0.0, 0.0);
    left_upper_arm();

    glTranslatef(0.0, UPPER_ARM_LENGTH, 0.0);
    glRotatef(body[4], 1.0, 0.0, 0.0);
    left_lower_arm();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(TORSO_RADIUS + UPPER_ARM_RADIUS, 0.9 * TORSO_HEIGHT, 0.0);
    glRotatef(body[5], 1.0, 0.0, 0.0);
    right_upper_arm();

    glTranslatef(0.0, UPPER_ARM_LENGTH, 0.0);
    glRotatef(body[6], 1.0, 0.0, 0.0);
    right_lower_arm();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-(TORSO_RADIUS + UPPER_LEG_RADIUS) + 0.9, 0.1 * UPPER_LEG_LENGTH, 0.0);
    glRotatef(body[7], 1.0, 0.0, 0.0);
    left_upper_leg();

    glTranslatef(0.0, UPPER_LEG_LENGTH, 0.0);
    glRotatef(body[8], 1.0, 0.0, 0.0);
    left_lower_leg();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(TORSO_RADIUS + UPPER_LEG_RADIUS - 0.9, 0.1 * UPPER_LEG_LENGTH, 0.0);
    glRotatef(body[9], 1.0, 0.0, 0.0);
    right_upper_leg();

    glTranslatef(0.0, UPPER_LEG_LENGTH, 0.0);
    glRotatef(body[10], 1.0, 0.0, 0.0);
    right_lower_leg();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == 'w') // go up
	{
		position_y += 0.1;
		display();
	}

	if (key == 's') // go down
	{
		position_y -= 0.1;
		display();
	}

    if (key == 'a') // go my left
	{
		position_x -= 0.1;
		display();
	}

	if (key == 'd') // go my right
	{
		position_x += 0.1;
		display();
	}

}

void move_trunc()
{
	if (moving_direction == 0 || fmod(abs(moving_direction), 18) == 0) // 0, 180 -> x does not change
	{
		position_z = position_z + 0.001 * (moving_direction);
	}
	else if (fmod(abs(moving_direction), 9) == 0 || fmod(abs(moving_direction), 27) == 0) // 90, 270 -> z does not change
	{
		position_x = position_x + 0.001 * (moving_direction);
	}
	else
	{
		position_x = position_x + 0.001 * (moving_direction);
		position_z = position_z + 0.001 * (moving_direction);
	}

	printf("x : %2.3f | z : %2.3f  .. moving : %2.3f\n", position_x, position_z, moving_direction);
}

void processSpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{

		if (up == 1) // leg goes up
		{
			if (left_leg == 1) // now moving the left leg
			{
			//	printf("%3.2f ", body[10] - 360);

				body[10] += speed ;
				if( body[10] < 360.0 ) body[10] += 360.0;
				body[9] -= speed;
				if( body[9] < 360.0 ) body[9] += 360.0;

				body[4] -= ceil((double)speed/2) ;
				if( body[4] < 360.0 ) body[4] += 360.0;

				body[3] -= ceil((double)speed/2);
				if( body[3] < 360.0 ) body[3] += 360.0;

				if (body[6] != 0)
				{
					body[6] += ceil((double)speed/2)/2 ;
					if( body[6] < 360.0 ) body[6] += 360.0;

					body[5] += ceil((double)speed/2)/2;
					if( body[5] < 360.0 ) body[5] += 360.0;
				}

				if (body[10] - 360 < max_leg_angle)
				{
					up = 1;
				}
				else up = 0;

				// move the trunc

				move_trunc();

				display();
			}
			else // moving the right leg
			{
			//	printf("%3.2f ", body[8] - 360);

				body[8] += speed ;
				if( body[8] < 360.0 ) body[8] += 360.0;
				body[7] -= speed;
				if( body[7] < 360.0 ) body[7] += 360.0;

				body[6] -= ceil((double)speed/2) ;
				if( body[6] < 360.0 ) body[6] += 360.0;

				body[5] -= ceil((double)speed/2);
				if( body[5] < 360.0 ) body[5] += 360.0;

				if (body[4] != 0)
				{
					body[4] += ceil((double)speed/2)/2 ;
					if( body[6] < 360.0 ) body[4] += 360.0;

					body[3] += ceil((double)speed/2)/2;
					if( body[3] < 360.0 ) body[3] += 360.0;
				}

				if (body[8] - 360 < max_leg_angle)
				{
					up = 1;
				}
				else up = 0;

				// move the trunc

				move_trunc();

				display();
			}
		}

		else if (up == 0) // leg goes down
		{
			if (left_leg == 1) // now moving the left leg
			{
			//	printf("%3.2f (%d) + %3.2f (%d) // ", body[10] - 360, 10, body[9] - 360, 9);

				if (body[7] - 360 > 180)
				{
					body[7] -= speed/4;
				}

				body[10] -= speed ;
				body[9] += speed;
				if( body[9] < 360.0 ) body[9] += 360.0;

				body[4] += ceil((double)speed/2)/2 ;
				if( body[4] < 360.0 ) body[4] += 360.0;

				body[3] += ceil((double)speed/2)/2;
				if( body[3] < 360.0 ) body[3] += 360.0;

				if (body[10] - 360 == 0)
				{
					if (body[9] - 360 < 190)
					{
						body[9] += speed;
					}
					up = 1;
					left_leg = 0; // leg got down, switch leg
				}
				else up = 0;

				// move the trunc 

				move_trunc();

				display();
			}
			else // moving the right leg
			{
			//	printf("%3.2f (%d) ", body[7] - 360, 7);

				if (body[9] - 360 > 180)
				{
					body[9] -= speed/4;
				}

				body[8] -= speed ;
				body[7] += speed;
				if( body[7] < 360.0 ) body[7] += 360.0;

				body[6] += ceil((double)speed/2)/2 ;
				if( body[6] < 360.0 ) body[6] += 360.0;

				body[5] += ceil((double)speed/2)/2;

				if (body[8] - 360 == 0)
				{
					if (body[7] - 360 < 190)
					{
						body[7] += speed;
					}

					up = 1;
					left_leg = 1; // leg got down, switch leg
				}
				else up = 0;

				// move the trunc
				move_trunc();

				display();
			}
		}
	}

	if (key == GLUT_KEY_DOWN)
	{
		//body[10] -= 5 ;
		//if( body[10] < 360.0 ) body[10] += 360.0;
		//body[9] += 5;
		//if( body[9] < 360.0 ) body[9] += 360.0;

		//position += 0.5;

		display();
	}

	if (key == GLUT_KEY_LEFT)
	{
		body[0] += 5 ;
		moving_direction += 0.5;
	 //   if( moving_direction > 36.0 ) moving_direction -= 36.0;
		display();

	}

	if (key == GLUT_KEY_RIGHT)
	{
		body[0] -= 5 ;
		moving_direction -= 0.5;
		if( body[0] < 360.0 ) body[0] += 360.0;
	//  if( moving_direction < 36.0 ) moving_direction += 36.0;
		display();
	}
	
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-10.0, 10.0, -10.0 * (GLfloat) h / (GLfloat) w,
            10.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-10.0 * (GLfloat) w / (GLfloat) h,
            10.0 * (GLfloat) w / (GLfloat) h, 0.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init()
{
        GLfloat mat_specular[]={0.0, 0.0, 0.0, 0.0};
        GLfloat mat_diffuse[]={0.5, 0.4, 0.0, 1.0};
        GLfloat mat_ambient[]={0.5, 0.5, 0.0, 1.0};
		GLfloat mat_shininess={100.0};

        GLfloat light_ambient[]={0.0, 0.0, 0.0, 1.0};
        GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
        GLfloat light_position[]={10.0, 10.0, 10.0, 0.0};

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING); 
        glEnable(GL_LIGHT0);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST); 

        glClearColor(0.8, 0.0, 0.5, 1.0);
        glColor3f(0.0, 0.0, 0.0);

        h=gluNewQuadric();
        gluQuadricDrawStyle(h, GLU_FILL);

        t=gluNewQuadric();
        gluQuadricDrawStyle(t, GLU_FILL);

        lua=gluNewQuadric();
        gluQuadricDrawStyle(lua, GLU_FILL);

        lla=gluNewQuadric();
        gluQuadricDrawStyle(lla, GLU_FILL);

        rua=gluNewQuadric();
        gluQuadricDrawStyle(rua, GLU_FILL);

        rla=gluNewQuadric();
        gluQuadricDrawStyle(rla, GLU_FILL);

        lul=gluNewQuadric();
        gluQuadricDrawStyle(lul, GLU_FILL);

        lll=gluNewQuadric();
        gluQuadricDrawStyle(lll, GLU_FILL);

        rul=gluNewQuadric();
        gluQuadricDrawStyle(rul, GLU_FILL);

        rll=gluNewQuadric();
        gluQuadricDrawStyle(rll, GLU_FILL);
}

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Biluta");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(processSpecialKeys);

    glutMainLoop();
}


