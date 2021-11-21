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

static int year = 0;
static double zoom = 1000.0;

double rotate_y=0;
double rotate_x=0;
double rotate_z=0;
static bool animacao = false;

void reshape (int w, int h);
void criaAnel(int raioInterno, int raioExterno);
void criaAneisSaturno();

class CorpoCeleste {
   public:
      double cores[3];
      double distancia;
      double tamanho[3];
      int diasTerrestresNoAno;
      std::vector<CorpoCeleste> corpos_orbitando;

      CorpoCeleste(double cores[3], double distancia, double tamanho[3], int diasTerrestresNoAno, std::vector<CorpoCeleste> corpos_orbitando){
         for(int i = 0; i < 3; i++){
            this->cores[i] = cores[i];
            this->tamanho[i] = tamanho[i];
         
         }
         this->distancia = distancia;
         this->diasTerrestresNoAno = diasTerrestresNoAno;
         this->corpos_orbitando = corpos_orbitando;
      }
      void rederizaCorpo(){
         GLfloat diaNoCorpo = ((GLfloat)360/(GLfloat) diasTerrestresNoAno) * year;
         
         glRotatef (diaNoCorpo, 0.0, 1.0, 0.0);
         glTranslatef (distancia, 0.0, 0.0);
         glColor3f(cores[0], cores[1], cores[2]);
         glRotatef ((GLfloat) year*13, 0.0, 1.0, 0.0);
         glutSolidSphere(tamanho[0], tamanho[1], tamanho[2]);         
  
         for(int i = 0; i < corpos_orbitando.size(); ++i){
            glPushMatrix();
               corpos_orbitando[i].rederizaCorpo();
            glPopMatrix();   
         }

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
   double tamanhoSol = 40.0;

   //mercurio
   double cores[3] = {0.863, 0.863, 0.863};
   double tamanho[3] = {1.4, 10, 8};
   CorpoCeleste mercurio(cores, 11.58 + tamanhoSol, tamanho, 88, corpo_null);

   //venus
   double cores1[3] = {0.886, 0.617, 0.109};
   double tamanho1[3] = {3.5, 14, 10};
   CorpoCeleste venus(cores1, 21.64 + tamanhoSol, tamanho1, 225, corpo_null);
   
   //terra
   double cores2[3] = {0.163, 0.163, 0.163};
   double tamanho2[3] = {1, 8, 6};
   CorpoCeleste lua(cores, 1.07+3.7, tamanho2, 27, corpo_null);
   std::vector<CorpoCeleste> luas;

   luas.push_back(lua);
   double cores3[3] = {0.109, 0.289, 0.613};
   double tamanho3[3] = {3.7, 14, 10};
   CorpoCeleste terra(cores3, 29.92 + tamanhoSol, tamanho3, 365, luas);
         
   //marte   
   double cores4[3] = {0.839, 0.316, 0.171};
   double tamanho4[3] = {2, 14, 10};
   CorpoCeleste marte(cores4, 45.588 + tamanhoSol, tamanho4, 687, corpo_null);
   
   //Jupiter   
   double cores5[3] = {0.785, 0.562, 0.222};
   double tamanho5[3] = {17, 18, 16};
   CorpoCeleste jupiter(cores5, 77.833 + tamanhoSol, tamanho5, 4330, corpo_null);

   //Saturno   
   double cores6[3] = {0.687, 0.558, 0.210};
   double tamanho6[3] = {15, 16, 14};
   CorpoCeleste saturno(cores6, 142.94 + tamanhoSol, tamanho6, 10752, corpo_null);

   //Urano  
   double cores7[3] = {0.332, 0.5, 0.664};
   double tamanho7[3] = {7.35, 14, 12};
   CorpoCeleste urano(cores7, 191.39 + tamanhoSol, tamanho7, 31762, corpo_null);

   //Netuno   
   double cores8[3] = {0.210, 0.406, 0.585};
   double tamanho8[3] = {7.1, 14, 12};
   CorpoCeleste netuno(cores8, 225.218 + tamanhoSol, tamanho8, 60140, corpo_null);


   planetas.push_back(mercurio);
   planetas.push_back(venus);
   planetas.push_back(terra);
   planetas.push_back(marte);
   planetas.push_back(jupiter);
   planetas.push_back(saturno);
   planetas.push_back(urano);
   planetas.push_back(netuno);


   for(int i = 0; i < planetas.size(); ++i){
      glPushMatrix();
      planetas[i].rederizaCorpo();

      if( i == 5 ){ //saturno
         criaAneisSaturno();
      }
      glPopMatrix();
      
   }
}

void criaAneisSaturno(){
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glColor3f(0.487, 0.358, 0.110);
   criaAnel(20, 23);
   glColor3f(0.387, 0.258, 0.000);
   criaAnel(24, 26);
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
}

void criaAnel(int raioInterno, int raioExterno){
   int totalIteracoes = 30; 
   GLfloat doisPi = 2.0f * 3.14159;
   
   glBegin(GL_TRIANGLE_STRIP);
      for(int i = 0; i <= totalIteracoes;i++) { 
         glVertex3f(
            0 + (raioInterno * cos(i *  doisPi / totalIteracoes)),
            0.0, 
            0 + (raioInterno * sin(i * doisPi / totalIteracoes))
         );
         glVertex3f(
            0 + (raioExterno * cos(i *  doisPi / totalIteracoes)),
            0.0, 
            0 + (raioExterno * sin(i * doisPi / totalIteracoes))
         );
      }
      
   glEnd();
}

void fazLuz(){
   float luzAmbiente[] = {0.2, 0.2, 0.2, 0.0f};
   float luzDifusa[] = {1.0, 1.0, 1.0, 1.0f};
   float luzEspecular[] = {1.0, 1.0, 1.0, 1.0f};
   float posicaoLuz[] = {0.0f, 0.0f, 0.0f, 1.0f};

   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

   glEnable(GL_COLOR_MATERIAL);

   glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
   glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
   glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
   
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

}

void criaSol(){
   float luzAmbiente[] = {0.7, 0.7, 0.7, 0.5f};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
   glColor3f(1.0, 0.5, 0.0);
   glutSolidSphere(40.0, 20, 16); 
}

void rotacionaSistema(){
   glRotatef(rotate_x, 1.0, 0.0, 0.0);
   glRotatef(rotate_y, 0.0, 1.0, 0.0);
   glRotatef(rotate_z, 0.0, 0.0, 1.0);
}

void display(void)
{

   reshape(1000, 700);

   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);   
   glCullFace(GL_FRONT);
   

   
   glPushMatrix();

      rotacionaSistema();

      criaSol();
      fazLuz();
      criaPlanetas();

   glPopMatrix();
   glutSwapBuffers();
}

void rodaAnimacao(int a)
{
    if (animacao)
    {
        glutPostRedisplay();
        glutTimerFunc(1000 / 60, rodaAnimacao, a);
        year += 1;
    }
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
      case 'a':
      case 'A':
         animacao = !animacao;
         rodaAnimacao(1);
      case 'y':
         year = year + 1;
         glutPostRedisplay();
         break;
      case 'Y':
         year = year - 1;
         glutPostRedisplay();
         break;
      case 'u':
         year = year + 10;
         glutPostRedisplay();
         break;
      case 'U':
         year = year - 10;
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