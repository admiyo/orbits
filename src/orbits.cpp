// orbits
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <unistd.h>

#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>
#include <sstream>
using namespace std;
using namespace boost::gregorian;

void drawCircle(float x, float y, float radius );
void display(void);

bool do_update = false;
int direction = 1;

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

void render_string(float x, float y, void *font, const char* text, Color const& rgb)
{
  char *c;
  glColor3f(rgb.red, rgb.green, rgb.blue);
  glRasterPos2f(x, y);
  glutBitmapString(font, (unsigned char *) text);
}



class Orbitor {
  Color color;
  float delta;
  float x;
  float y;
  float radius;
  float rad = 0.0;
  float orbit_rad = 1.0;
  const char * name;

public:
  Orbitor(float radius, float rads, float orbit_rad, float period,
	  Color& color, const char * name)
  {
    this->color = color;
    this->delta = 10/period;
    this->radius = radius;
    this->orbit_rad = orbit_rad / 20;
    this->rad = rads;
    x = cos(rad) * this->orbit_rad;
    y = sin(rad) * this->orbit_rad;
    this->name = name;
  };

  void display()
  {
    glColor3f(color.red,color.green,color.blue);
    drawCircle(x, y, radius);
    render_string(x, y, GLUT_BITMAP_HELVETICA_12, this->name, this->color);
  };

  void update()
  {
    rad += delta * direction;
    x = cos(rad) * this->orbit_rad;
    y = sin(rad) * this->orbit_rad;
    glutPostRedisplay();
  };
};


void mouseClicks(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    do_update = ! do_update;
  }
  if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    direction = direction * -1;
  }
}


Orbitor orbitors[]={
  Orbitor(0.25,   -98, 35.0,    115.88,  GRAY,  "Mercury"),
  Orbitor(0.25,   157, 67.0,    224.70,  GREEN, "Venus"),
  Orbitor(0.25,     0, 93.0,    365.25,  BLUE,  "Earth"),
  Orbitor(0.25,  -165, 142.0,   779.94,  RED,   "Mars"),
  Orbitor(0.25,  -230, 275.0,  1682.00,  GRAY,  "Ceres"),
  Orbitor(0.25,      3, 280.0,  1821.00, GREEN, "Psyche"),
  Orbitor(0.45,   -100, 484.0,  4333.00, RED,   "Jupiter"),
  Orbitor(0.45,    -60, 887.0, 10759.00, BLUE,  "Saturn")
};

int o_count = sizeof(orbitors) / sizeof(Orbitor);
std::string s("2022-06-15");
date current_time(from_simple_string(s));


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
    render_string(0.75, 0.0, GLUT_BITMAP_HELVETICA_12, "Sun", YELLOW);

    std::stringstream c_date;
    c_date << "date: "<< current_time.year()
	   << "-" <<  current_time.month()
	   << "-" <<  current_time.day() ;

    render_string(-30.0, 30.0, GLUT_BITMAP_HELVETICA_12, c_date.str().c_str(), YELLOW);

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void update()
{
  if (! do_update)
    {
      return;
    }
  for (int i = 0; i < o_count; i++){
    orbitors[i].update();
  }
  if (direction > 0){
    current_time += days(1);
  }else{
    current_time -= days(1);
  }
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

int orbits(int argc, char** argv)
{
  if (argc > 1)
    {
      cout << argv[1] << endl;
      std::string s(argv[1]);
      current_time = date(from_simple_string(s));
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Solar System Orbital Positions");
    glutMouseFunc(mouseClicks);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}
