// orbits
#include <GL/glut.h>
#include <math.h>

void drawCircle(float x, float y, float radius );
void display(void);


float rad = 0.0;
float step_x = 0.01;
float step_y = 0.01;
float delta = 0.1;



void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3f(1,1,0);   
    drawCircle(-2.5 + step_x, 0.0 + step_y, 0.25);
    glColor3f(0,0,1);    
    drawCircle(-5.5 + step_x * 3 , 0.0 + step_y * 3, 0.25);
    glColor3f(1,0,0);    
    drawCircle(0.0, 0.0, 0.25);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void update()
{
  rad += delta;
  step_x += sin(rad);
  step_y += cos(rad);
  glutPostRedisplay();
}

void reshape(int w, int h)
{
  glViewport(0,0,(GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world from Badprog.com :D");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}
