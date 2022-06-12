// orbits
#include <GL/glut.h>
#include <math.h>

void drawCircle(float x, float y, float radius );
void displayMe(void);


float step = 0.01;
float delta = step;

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle(0.5 + step, 0.5, 0.25);    
    glFlush();
    step += delta; 
}

void drawCircle(float x, float y, float radius )
{
  float PI = 3.14;
  int i;
  int triangleAmount = 20; //# of triangles used to draw circle
  //GLfloat radius = 0.8f; //radius
  GLfloat twicePi = 2.0f * PI;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y); // center of circle
  for(i = 0; i <= triangleAmount;i++) { 
    glVertex2f(
	       x + (radius * cos(i *  twicePi / triangleAmount)), 
	       y + (radius * sin(i * twicePi / triangleAmount))
	       );
  }
  glEnd();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world from Badprog.com :D");
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}
