/*Copyright (c) 2016 Seth Ballantyne <seth.ballantyne@gmail.com>
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*THE SOFTWARE.
*/

//---------------------------------------------------------------
// A heavily modified version of the electron demo
// found in the OpenGL Super Bible. The use of a linked
// list makes NO fucking sense here, but I wanted to play around.
//---------------------------------------------------------------

#include <stdlib.h>
#include "list.h"
#include "main.h"

// nucleus angle of revolution
GLfloat electron = 0.0f;

sphere_t nucleus;

// ever heard of an array? :P
sphere_t electron1;
sphere_t electron2;
sphere_t electron3;

list_t list;

void RenderScene(void)
{
	element_t *elem;
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glTranslatef(0.0f, 0.0f, -100.0f);
	elem = list.head;
	for(i = 0; i < list.size; i++)
	{
		sphere_t *temp = elem->data;

		glColor3ub(temp->colour.red, temp->colour.green, temp->colour.blue);
		
		glPushMatrix();

		glRotatef(temp->rotation.angle, temp->rotation.x, temp->rotation.y, temp->rotation.z);
		glTranslatef(temp->position.x, temp->position.y, temp->position.z);
		glutSolidSphere(temp->radius, temp->slices, temp->stacks);
	
		glPopMatrix();

		// kludge - we don't want to update the red sphere.
		if(!temp->stationary)
		{
			temp->rotation.angle += 10.0f;

			if(temp->rotation.angle > 360.0f)
				temp->rotation.angle = 0.0f;
		}

		if(NULL != elem->next)
			elem = elem->next;
	}

	glutSwapBuffers();
}

void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc, 1);
}

void ChangeSize(int width, int height)
{
	GLfloat range = 100.0f;

	// prevent div by zero errors
	if(0 == height)
		height = 1;

	// set the viewport to window dimensions
	glViewport(0, 0, width, height);

	// Reset the coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// establish clipping volume (left, right, bottom, top, near, far))
	if(width <= height)
		glOrtho(-range, range, range * height / width, -range * height / width,
				-range * 2.0f, range * 2.0f);
	else
		glOrtho(-range * width / height, range * width / height, range, -range, -range * 2.0f,
		range * 2.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv)
{
	List_Create(&list, NULL);

	// set all the members of nucleus to 0 so we don't have to do it manually
	memset(&nucleus, 0, sizeof(sphere_t));
	memset(&electron1, 0, sizeof(sphere_t));
	memset(&electron2, 0, sizeof(sphere_t));
	memset(&electron3, 0, sizeof(sphere_t));

	nucleus.colour.red = 255;
	nucleus.position.z = -100.0f;
	nucleus.radius = 10.0f;
	nucleus.slices = 30;
	nucleus.stacks = 30;
	nucleus.stationary = true;

	electron1.colour.red = electron1.colour.green = 255;
	electron1.position.x = 90.0f;
	electron1.rotation.y = 1.0f;
	electron1.position.x = 90.0f;
	electron1.radius = 6.0f;
	electron1.slices = electron1.stacks = 15;

	electron2.colour = electron1.colour;
	electron2.position.x = -70.0f;
	electron2.rotation.y = electron2.rotation.z = 1.0f;
	electron2.radius = electron1.radius;
	electron2.slices = electron1.slices;
	electron2.stacks = electron1.stacks;

	List_AddItem(&list, NULL, (sphere_t *) &nucleus);
	List_AppendItem(&list, NULL, (sphere_t *) &electron1);
	List_AppendItem(&list, NULL, (sphere_t *) &electron2);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Atom");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(500, TimerFunc, 1);
	SetupRC();
	glutMainLoop();

	List_Destroy(&list);

	return EXIT_SUCCESS;
}