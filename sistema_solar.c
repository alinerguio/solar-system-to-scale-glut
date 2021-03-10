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

#include <GL/glut.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h> 

static double year_mercury = 0, day_mercury = 0, year_venus = 0, day_venus = 0;
static double year_earth = 0, day_earth = 0, year_mars = 0, day_mars = 0;
static double year_jupiter = 0, day_jupiter = 0, year_saturn = 0, day_saturn = 0;
static double year_uranus = 0, day_uranus = 0, year_neptune = 0, day_neptune = 0;

GLfloat proportion = 1; /* proportion of sun and planets - its sizes and distances */

static bool automatic = false;
static bool light = false;

#define stripeImageWidth 12
GLubyte stripeImage[4*stripeImageWidth];

#ifdef GL_VERSION_1_1
static GLuint texName;
#endif

void makeStripeImage(void)
{
   int j;
    
   for (j = 0; j < stripeImageWidth; j++) {
      stripeImage[4*j] = (GLubyte) 255;
      stripeImage[4*j+1] = (GLubyte) ((j>4) ? 100 : 0);
      stripeImage[4*j+2] = (GLubyte) 0;
      stripeImage[4*j+3] = (GLubyte) 255;
   }
}

void timer()
{
    if (automatic)
    {
        glutPostRedisplay();
        glutTimerFunc(1000 / 60, timer, 0);
        day_mercury = fmod((day_mercury + 0.25), 360.0);
        year_mercury = fmod((year_mercury + 4.09), 360.0);
        day_venus = fmod((day_venus + 0.06), 360.0);
        year_venus = fmod((year_venus + 0.62), 360.0);
        day_earth = fmod((day_earth + 15.15), 360.0);
        year_earth = fmod((year_earth + 0.98), 360.0);
        day_mars = fmod((day_mars + 14.56), 360.0);
        year_mars = fmod((year_mars + 0.52), 360.0);
        day_jupiter = fmod((day_jupiter + 36.58), 360.0);
        year_jupiter = fmod((year_jupiter + 0.083), 360.0);
        day_saturn = fmod((day_saturn + 33.33), 360.0);
        year_saturn = fmod((year_saturn + 0.033), 360.0);
        day_uranus = fmod((day_uranus + 20.83), 360.0);
        year_uranus = fmod((year_uranus + 0.012), 360.0);
        day_neptune = fmod((day_neptune + 22.38), 360.0);
        year_neptune = fmod((year_neptune + 0.005), 360.0);
    }
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_SMOOTH);

   makeStripeImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_LIGHTING);

#ifdef GL_VERSION_1_1
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_1D, texName);
#endif
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef GL_VERSION_1_1
   glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
#else
   glTexImage1D(GL_TEXTURE_1D, 0, 4, stripeImageWidth, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
#endif

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   
   GLfloat coeff[] = {1.0, 1.0, 1.0, 0.0};

   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   glTexGenfv(GL_S, GL_OBJECT_PLANE, coeff);

   if (light)
   {

     GLfloat ambient_light[] = { 1.0, 1.0, 1.0, 1.0 };
     
     glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);  
     glEnable(GL_LIGHT1);

     GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };

     glLightfv(GL_LIGHT0, GL_POSITION, light_position);
     glEnable(GL_LIGHT0);
  
   } else {

       glDisable(GL_LIGHT1);
       glDisable(GL_LIGHT0);

   }

   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_CULL_FACE);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);
   glFrontFace(GL_CW);
   glCullFace(GL_BACK);
   glEnable(GL_DEPTH_TEST);
}

void sun()
{
   glPushMatrix();
   #ifdef GL_VERSION_1_1
   glBindTexture(GL_TEXTURE_1D, texName);
   #endif
   glEnable(GL_TEXTURE_1D);
   glutSolidSphere(1.0 * proportion, 20, 16);   /* draw sun */
   glDisable(GL_TEXTURE_1D);
   glPopMatrix();
}

void mercury()
{
   glPushMatrix();
   glColor3f (1.0, 0.0, 0.0);
   glRotatef ((GLfloat) year_mercury, 0.0, 0.0, 1.0);
   glTranslatef ((0.069 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_mercury, 0.0, 0.0, 1.0);
   glutSolidSphere(0.0035 * proportion, 20, 16);    /* draw mercury */
   glPopMatrix();
}

void venus()
{
   glPushMatrix();
   glColor3f (0.90, 0.90, 0.90);
   glRotatef ((GLfloat) year_venus, 0.0, 0.0, 1.0);
   glTranslatef ((0.128 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_venus, 0.0, 0.0, 1.0);
   glutSolidSphere(0.0084 * proportion, 20, 16);    /* draw venus */
   glPopMatrix();
}

void earth()
{
   glPushMatrix();
   glColor3f (0.18, 0.41, 0.59);
   glRotatef ((GLfloat) year_earth, 0.0, 0.0, 1.0);
   glTranslatef ((0.177 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_earth, 0.0, 0.0, 1.0);
   glutSolidSphere(0.009 * proportion, 20, 16);    /* draw earth */
   glPopMatrix(); 
}

void mars()
{
   glPushMatrix();
   glColor3f (0.6, 0.24, 0.0);
   glRotatef ((GLfloat) year_mars, 0.0, 0.0, 1.0);
   glTranslatef ((0.27 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_mars, 0.0, 0.0, 1.0);
   glutSolidSphere(0.0048 * proportion, 20, 16);    /* draw mars */
   glPopMatrix();
}

void jupiter()
{
   glPushMatrix();
   glColor3f (0.69, 0.5, 0.21);
   glRotatef ((GLfloat) year_jupiter, 0.0, 0.0, 1.0);
   glTranslatef ((0.923 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_jupiter, 0.0, 0.0, 1.0);
   glutSolidSphere(0.1029 * proportion, 20, 16);    /* draw jupiter */
   glPopMatrix();

}

void saturn()
{
   glPushMatrix();
   glRotatef ((GLfloat) year_saturn, 0.0, 0.0, 1.0);
   glTranslatef ((1.693 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_saturn, 0.0, 0.0, 1.0);
   glColor3f (0.85, 0.56, 0.21);
   glutSolidTorus ((0.005) * proportion, (0.0847 + 0.05) * proportion, 15, 15);
   glColor3f (0.69, 0.56, 0.21);
   glutSolidSphere(0.0847 * proportion, 20, 16);    /* draw saturn */
   glPopMatrix();
}

void uranus()
{
  glPushMatrix();
   glColor3f (0.33, 0.50, 0.67);
   glRotatef ((GLfloat) year_uranus, 0.0, 0.0, 1.0);
   glTranslatef ((3.404 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_uranus, 0.0, 0.0, 1.0);
   glutSolidSphere(0.0363 * proportion, 20, 16);    /* draw uranus */
   glPopMatrix();
}

void neptune()
{
   glPushMatrix();
   glColor3f (0.21, 0.40, 0.58);
   glRotatef ((GLfloat) year_neptune, 0.0, 0.0, 1.0);
   glTranslatef ((5.333 + 1) * proportion, 0.0, 0.0);
   glRotatef ((GLfloat) day_neptune, 0.0, 0.0, 1.0);
   glutSolidSphere(0.0363 * proportion, 20, 16);    /* draw neptune */
   glPopMatrix();
} 

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   glPushMatrix();
     sun();
     mercury();
     venus();
     earth();
     mars();
     jupiter();
     saturn();
     uranus();
     neptune();
   glPopMatrix();
   
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(75.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 7.5 , 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) { // 20x times the proportional translation
      case 'A': 
      case 'a': // automatic movement
         automatic = !automatic;
         timer();
         break;
      case 'm': // mercury
         year_mercury = fmod((year_mercury + 81.8), 360.0);
         glutPostRedisplay();
         break;
      case 'M': // mercury
         year_mercury = fmod((year_mercury - 81.8), 360.0);
         glutPostRedisplay();
         break;
      case 'v': // venus
         year_venus = fmod((year_venus + 12.4), 360.0);
         glutPostRedisplay();
         break;
      case 'V': // venus
         year_venus = fmod((year_venus - 12.4), 360.0);
         glutPostRedisplay();
         break;
      case 'e': // earth
         year_earth = fmod((year_earth + 19.6), 360.0);
         glutPostRedisplay();
         break;
      case 'E': // earth
         year_earth = fmod((year_earth - 19.6), 360.0);
         glutPostRedisplay();
         break;
      case 'r': // mars
         year_mars = fmod((year_mars + 10.4), 360.0);
         glutPostRedisplay();
         break;
      case 'R': // mars
         year_mars = fmod((year_mars - 10.4), 360.0);
         glutPostRedisplay();
         break;
      case 'j': // jupiter
         year_jupiter = fmod((year_jupiter + 1.66), 360.0);
         glutPostRedisplay();
         break;
      case 'J': // jupiter
         year_jupiter = fmod((year_jupiter - 1.66), 360.0);
         glutPostRedisplay();
         break;
      case 's': // saturn
         year_saturn = fmod((year_saturn + 0.66), 360.0);
         glutPostRedisplay();
         break;
      case 'S': // saturn
         year_saturn = fmod((year_saturn - 0.66), 360.0);
         glutPostRedisplay();
         break;
      case 'u': // uranus
         year_uranus = fmod((year_uranus + 0.24), 360.0);
         glutPostRedisplay();
         break;
      case 'U': // uranus
         year_uranus = fmod((year_uranus - 0.24), 360.0);
         glutPostRedisplay();
         break;
      case 'n': // neptune
         year_neptune = fmod((year_neptune + 0.1), 360.0);
         glutPostRedisplay();
         break;
      case 'N': // neptune
         year_neptune = fmod((year_neptune - 0.1), 360.0);
         glutPostRedisplay();
         break;
      case 'L': 
      case 'l': // turn off light
         light = !light;
         init ();
         glutPostRedisplay();
         break;
      case 27: // exit
         exit(0);
         break;
      default:
         break;
   }
}

void mouse(int button, int state, int x, int y)  
{
    if (button == GLUT_LEFT_BUTTON){ // zoom in
      if (state == GLUT_UP) {
        if (proportion < 2.55){
          proportion += 0.05; 
        }
        display(); 
      }  
    }

    if (button == GLUT_RIGHT_BUTTON){ // zoom out
      if (state == GLUT_UP) {
        if(proportion >= 1.0){
          proportion -= 0.05; 
        }
        display();
      }  
    }
         
    glutPostRedisplay();
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
   glutInitWindowSize (1000, 1000); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}