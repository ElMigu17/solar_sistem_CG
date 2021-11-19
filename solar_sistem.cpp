/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  planet.c
 *  This program shows how to composite modeling transformations
 *  to draw translated and rotated models.
 *  Interaction:  pressing the d and y keys (day and year)
 *  alters the rotation of the planet around the sun.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <math.h>

static int year = 0, day = 0;
static int moon_year = 0, moon_day = 0;
static float dv = 20.0;
static int iv = 0;
static double zoom = 100.0;

void reshape (int w, int h);
double rotate_y=0;
double rotate_x=0;
double rotate_z=0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_LINE_SMOOTH); 
   glShadeModel (GL_FLAT);
      glShadeModel (GL_SMOOTH);


   
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_FRONT);
   
}

void criaPlanetas(){
   int qtd = 4;
   double cores[qtd][3];
   double distancia[qtd];
   double tamanho[qtd][3];
   double velocidade_ano[qtd];
   int dias_no_ano[qtd];
   
   // mercurio
   cores[0][0] = cores[0][1] = cores[0][2] = 0.863;
   
   distancia[0] = 11.58;
   
   tamanho[0][0] = 1.4;
   tamanho[0][1] = 10;
   tamanho[0][2] = 8;
   
   dias_no_ano[0] = 88;
   
   //venus
   cores[1][0] = 0.886;
   cores[1][1] = 0.617;
   cores[1][2] = 0.109;
   
   distancia[1] = 21.64;
   
   tamanho[1][0] = 3.5;
   tamanho[1][1] = 14;
   tamanho[1][2] = 10;
   
   dias_no_ano[1] = 225;
   
   //terra
   cores[2][0] = 0.109;
   cores[2][1] = 0.289;
   cores[2][2] = 0.613;
   
   distancia[2] = 29.92;
   
   tamanho[2][0] = 3.7;
   tamanho[2][1] = 14;
   tamanho[2][2] = 10;
   
   dias_no_ano[2] = 365;
   
   //marte
   cores[3][0] = 0.839;
   cores[3][1] = 0.316;
   cores[3][2] = 0.171;
   
   distancia[3] = 45.588;
   
   tamanho[3][0] = 2;
   tamanho[3][1] = 14;
   tamanho[3][2] = 10;
   
   dias_no_ano[3] = 687;
   
   for(int i=0; i<qtd; i++){
      GLfloat day_in_the_planet = ((GLfloat)360/(GLfloat)dias_no_ano[i]) * year;

      glPushMatrix();
      glRotatef ((GLfloat) day_in_the_planet, 0.0, 1.0, 0.0);
      glTranslatef (distancia[i] + 20, 0.0, 0.0);
      glColor3f(cores[i][0], cores[i][1], cores[i][2]);
      glRotatef ((GLfloat) day , 1.0, 0.0, 0.0);
      glutSolidSphere(tamanho[i][0], tamanho[i][1], tamanho[i][2]); 
      glPopMatrix();        
   }
}

void makeLuz(){
   float a = 0.2f;
   float luzAmbiente[] = {a, a, a, 0.0f};
   
   float d = 1.0;
   float e = 1.0;
   float luzDifusa[] = {d, d, d, 1.0f};
   float luzEspecular[] = {e, e, e, 1.0f};
   float posicaoLuz[] = {0.0f, 0.0f, 0.0f, 1.0f};

   // Ativa o uso da luz ambiente 
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
   glEnable(GL_COLOR_MATERIAL);
   // Define os parâmetros da luz de número 0
   glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);

   glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
   glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
   glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
   

   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   

}


void display(void)
{

   reshape(1000, 700);

   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glRotatef(rotate_x, 1.0, 0.0, 0.0);
   glRotatef(rotate_y, 0.0, 1.0, 0.0);
   glRotatef(rotate_z, 0.0, 0.0, 1.0);
      
   glCullFace(GL_FRONT);
   
   float a = 0.7f;
   float luzAmbiente[] = {a, a, a, 0.5f};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
   

   glPushMatrix();
   glColor3f(1.0, 0.5, 0.0);
   glutSolidSphere(20.0, 20, 16);   /* draw sun */

   makeLuz();

   criaPlanetas();
   /*
   glColor3f (1.0, 1.0, 1.0);
   glRotatef ((GLfloat) moon_year, 0.0, 1.0, 0.0);
   glTranslatef (0.5, 0.0, 0.0);
   glRotatef ((GLfloat) moon_day, 0.0, 1.0, 0.0);
   https://www.khronos.org/opengl/wiki/Viewing_and_Transformations#How_does_the_camera_work_in_OpenGL.3F
   glutSolidSphere(0.1, 16, 12);    /* draw moon */

   glPopMatrix();
   glutSwapBuffers();
}


void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(zoom, (GLfloat) w/(GLfloat) h, 1.0, 400.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'd':
         day = day + 1;
         glutPostRedisplay();
         break;
      case 'D':
         day = day - 1;
         glutPostRedisplay();
         break;
      case 'y':
         year = year + 1;
         glutPostRedisplay();
         break;
      case 'Y':
         year = year - 1;
         glutPostRedisplay();
         break;
      case 'n':
         moon_day = (moon_day + 10) % 360;
         glutPostRedisplay();
         break;
      case 'N':
         moon_day = (moon_day - 10) % 360;
         glutPostRedisplay();
         break;
      case 'm':
         moon_year = (moon_year + 5) % 360;
         glutPostRedisplay();
         break;
      case 'M':
         moon_year = (moon_year - 5) % 360;
         glutPostRedisplay();
         break;
      case 'q':
         dv = dv + 1;
         std::cout<<dv<<std::endl;
         glutPostRedisplay();
         break;
      case 'Q':
         dv = dv - 1;
         std::cout<<dv<<std::endl;
         glutPostRedisplay();
         break;
      case 'w':
         iv = iv + 1;
         glutPostRedisplay();
         break;
      case 'W':
         iv = iv - 1;
         glutPostRedisplay();
         break;
      case 'z':
         zoom = 1;
         glutPostRedisplay();
         break;
      case 'Z':
         zoom = -1;
         glutPostRedisplay();
         break;
      
      case 27:
         exit(0);
         break;
         
         
      default:
         break;
   }
      
}

void mouseWheel(int button, int dir, int x, int y)
{
   glTranslatef(0,0,0);
   if (button == 3)
   {
      zoom += 1;
   }
   else if(button == 4)
   {
      zoom -= 1;
   }
   else if(button == 1){
      rotate_z += 5%360;
   }
   else if(button == 2){
      rotate_z -= 5%360;
   }
   std::cout<<zoom<<std::endl;   
   int w = 1000;
   int h = 700;

   glutPostRedisplay();
}

void specialKeys(int key, int x, int y){

   if(key == GLUT_KEY_RIGHT){
      rotate_y += 5%360;
   }
   else if(key == GLUT_KEY_LEFT){
      rotate_y -= 5%360;
   }
   else if(key == GLUT_KEY_UP){
      rotate_x += 5%360;
   }
   else if(key == GLUT_KEY_DOWN){
      rotate_x -= 5%360;
   }

   glutPostRedisplay();
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1000, 700); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeys);
   glutMouseFunc(mouseWheel);
   glutMainLoop();
   return 0;
}