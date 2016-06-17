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

#pragma once
#include <GL/glut.h>

// Because Microsoft finds it so fucking hard to support C99
typedef int bool;
#define false 0
#define true 1

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} position_t;

typedef struct 
{
	GLubyte red;
	GLubyte green;
	GLubyte blue;
} colour_t;

typedef struct
{
	GLfloat angle;
	GLfloat x;
	GLfloat y;
	GLfloat z;
} rotation_t;

typedef struct
{
	bool stationary;
	position_t position;
	colour_t colour;
	rotation_t rotation;
	GLdouble radius;
	GLint slices;
	GLint stacks;
} sphere_t;

