// orbits
#include <GL/glut.h>
#include <math.h>

void drawCircle(float x, float y, float radius );
void display(void);

struct Color {
  float red;
  float green;
  float blue;
};

Color RED =    {1.0, 0.0, 0.0};
Color YELLOW = {1.0, 1.0, 0.0};
Color GREEN =  {0.0, 1.0, 0.0};
Color BLUE =   {0.0, 0.0, 1.0};
Color GRAY =   {0.5, 0.5, 0.5};


class Orbitor {
  Color color;
  float delta;
  float x;
  float y;
  float radius;
  float rad = 0.0;
  float orbit_rad = 1.0;

public:
  Orbitor(float radius, float rads, float orbit_rad, float period,
	  Color& color)
  {
    this->color = color;
    this->delta = 10/period;
    this->radius = radius;
    this->orbit_rad = orbit_rad / 20;
    this->rad = rads;
    x = cos(rad) * this->orbit_rad;
    y = sin(rad) * this->orbit_rad;
  };

  void display()
  {
    glColor3f(color.red,color.green,color.blue);
    drawCircle(x, y, radius);
  };


  void update()
  {
    rad += delta;
    x = cos(rad) * this->orbit_rad;
    y = sin(rad) * this->orbit_rad;
    glutPostRedisplay();
  }
};

Orbitor orbitors[]={
  Orbitor(0.25,   -98, 35.0,    115.88, GRAY),  //mercury
  Orbitor(0.25,   157, 67.0,    224.70, GREEN), //venus
  Orbitor(0.25,     0, 93.0,    365.25, BLUE),  //earth
  Orbitor(0.25,  -165, 142.0,   779.94, RED),   //mars
  Orbitor(0.25,  -230, 275.0,  1682.00, GRAY),  //ceres
  Orbitor(0.25,      3, 280.0,  1821.00, GREEN), //psyche
  Orbitor(0.45,   -100, 484.0,  4333.00, RED),   //jupiter
  Orbitor(0.45,    -60, 887.0, 10759.00, BLUE)   //Saturn
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

    glColor3f(YELLOW.red,YELLOW.green,YELLOW.blue);
    drawCircle(0.0, 0.0, 0.75);
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
