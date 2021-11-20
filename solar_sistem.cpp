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
#include <vector>

static int year = 0, day = 0;
static int moon_year = 0, moon_day = 0;
static float dv = 20.0;
static int iv = 0;
static double zoom = 100.0;

void reshape (int w, int h);
double rotate_y=0;
double rotate_x=0;
double rotate_z=0;

class CorpoCeleste {
   public:
      double cores[3];
      double distancia;
      double tamanho[3];
      int dias_terrestre_no_ano;
      std::vector<CorpoCeleste> corpos_orbitando;

      CorpoCeleste(double cores[3], double distancia, double tamanho[3], int dias_terrestre_no_ano, std::vector<CorpoCeleste> corpos_orbitando){
         for(int i = 0; i < 3; i++){
            this->cores[i] = cores[i];
            this->tamanho[i] = tamanho[i];
         
         }
         this->distancia = distancia;
         this->dias_terrestre_no_ano = dias_terrestre_no_ano;
         this->corpos_orbitando = corpos_orbitando;
      }

};

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
   std::vector<CorpoCeleste> planetas;
   std::vector<CorpoCeleste> corpo_null;
   
   double cores[3] = {0.863, 0.863, 0.863};
   double tamanho[3] = {1.4, 10, 8};
   CorpoCeleste mercurio(cores, 11.58, tamanho, 88, corpo_null);

   double cores1[3] = {0.886, 0.617, 0.109};
   double tamanho1[3] = {3.5, 14, 10};
   CorpoCeleste venus(cores1, 21.64, tamanho1, 225, corpo_null);
   
   //terra
   double cores2[3] = {0.163, 0.163, 0.163};
   double tamanho2[3] = {1, 8, 6};
   CorpoCeleste lua(cores, 1.07, tamanho2, 27, corpo_null);
   std::vector<CorpoCeleste> luas;

   luas.push_back(lua);
   double cores3[3] = {0.109, 0.289, 0.613};
   double tamanho3[3] = {3.7, 14, 10};
   CorpoCeleste terra(cores3, 29.92, tamanho3, 365, luas);
         

 

   //marte   
   double cores4[3] = {0.839, 0.316, 0.171};
   double tamanho4[3] = {2, 14, 10};
   CorpoCeleste marte(cores4, 45.588, tamanho4, 687, corpo_null);
   
   planetas.push_back(mercurio);
   planetas.push_back(venus);
   planetas.push_back(terra);
   planetas.push_back(marte);
   std::cout<<"Dia: "<<day<<std::endl;
   for(int i = 0; i < planetas.size(); ++i){
      GLfloat day_in_the_planet = ((GLfloat)360/(GLfloat) planetas[i].dias_terrestre_no_ano) * year;

      glPushMatrix();
      
      glRotatef (day_in_the_planet, 0.0, 1.0, 0.0);
      glTranslatef (planetas[i].distancia + 20, 0.0, 0.0);
      glColor3f(planetas[i].cores[0], planetas[i].cores[1], planetas[i].cores[2]);
      glRotatef ((GLfloat) year*13, 0.0, 1.0, 0.0);
      glutSolidSphere(planetas[i].tamanho[0], planetas[i].tamanho[1], planetas[i].tamanho[2]); 

      if( planetas[i].corpos_orbitando.size() != 0 ){
         for(int j = 0; j < planetas[i].corpos_orbitando.size(); ++j){
            CorpoCeleste satelite =  planetas[i].corpos_orbitando[j];

            GLfloat day_in_the_moon = ((GLfloat)360/(GLfloat)satelite.dias_terrestre_no_ano) * year;
            glPushMatrix();
      
            glRotatef (day_in_the_planet, 0.0, 1.0, 0.0);
            glTranslatef (satelite.distancia + planetas[i].tamanho[0], 0.0, 0.0);
            glColor3f(satelite.cores[0], satelite.cores[1], satelite.cores[2]);
            glutSolidSphere(satelite.tamanho[0], satelite.tamanho[1], satelite.tamanho[2]);  

            glPopMatrix();   
         }
      }
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

      
   glCullFace(GL_FRONT);
   
   float a = 0.7f;
   float luzAmbiente[] = {a, a, a, 0.5f};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
   

   glPushMatrix();
   glRotatef(rotate_x, 1.0, 0.0, 0.0);
   glRotatef(rotate_y, 0.0, 1.0, 0.0);
   glRotatef(rotate_z, 0.0, 0.0, 1.0);
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
         day = day + 5;
         glutPostRedisplay();
         break;
      case 'D':
         day = day - 5;
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
   if (button == 4)
   {
      zoom += 1;
   }
   else if(button == 3)
   {
      zoom -= 1;
   }
   else if(button == 2){
      rotate_z += 2%360;
   }
   else if(button == 0){
      rotate_z -= 2%360;
   }
   std::cout<<"button: "<<button<<std::endl;   
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