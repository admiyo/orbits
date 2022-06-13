// orbits
#include <GL/glut.h>
#include <math.h>

void drawCircle(float x, float y, float radius );
void display(void);



class Orbitor {
  float delta;
  float x;
  float y;
  float radius;
  GLfloat red = 0;
  GLfloat green = 0;
  GLfloat blue = 0;
  float rad = 0.0;
  float orbit_rad = 1.0;
  
public:
  Orbitor(float radius, float orbit_rad, float period){ 
    this->delta = 1/period;
    this->radius = radius;
    this->red = 1.0;
    this->orbit_rad = orbit_rad;
    x = cos(rad) * orbit_rad;
    y = sin(rad) * orbit_rad;

  };
  
  void display(){
    glColor3f(red,green,blue);    
    drawCircle(x, y, radius);
  };


  void update()
  {
    rad += delta;
    x = cos(rad) * orbit_rad;
    y = sin(rad) * orbit_rad;
    glutPostRedisplay();
  }
};

Orbitor orbitors[]={
  Orbitor(0.25, 12.0, 100),
  Orbitor(0.25, 16.0, 200),
  Orbitor(0.25, 19.0, 300),
  Orbitor(0.25, 22.0, 400),
  Orbitor(0.25, 24.0, 500)

};

int o_count = sizeof(orbitors) / sizeof(Orbitor);


void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    for (int i = 0; i < o_count; i++){
      orbitors[i].display();
    }
    
    glColor3f(1,1,0);    
    drawCircle(0.0, 0.0, 1.0);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void update()
{

  for (int i = 0; i < o_count; i++){
    orbitors[i].update();
  }
  
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
