// orbits
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <unistd.h>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include <sstream>
using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

void drawCircle(float x, float y, float radius );
void display(void);
void distances_table();

bool do_update = true;
int direction = 1;
const int SCALING = 10;
std::string s("2022-06-15");
date base_time(from_simple_string(s));
date current_time;
int day_count = 0;
int time_delta = 24;  


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
  float orbit_radius = 1.0;
  float start_rad;
  float period;

public:
  const char * name;
  
public:
  Orbitor(float radius, float rads, float orbit_radius, float period,
	  Color& color, const char * name)
  {
    this->color = color;
    this->period = period;
    this->delta = 10/period;
    this->radius = radius;
    this->orbit_radius = orbit_radius / SCALING;
    this->name = name;
    calculate_position();
  };

  void calculate_position(){
    int d = (current_time - base_time).days();

    double delta = d / period;
    if (delta == 0.0){
      cout <<"delta too low for " << this->name << endl;
    }
    double rads = start_rad + delta;
    x = cos(rads) * this->orbit_radius;
    y = sin(rads) * this->orbit_radius;
  }
  
  void display()
  {
    glColor3f(color.red,color.green,color.blue);
    drawCircle(x, y, radius);
    render_string(x, y, GLUT_BITMAP_HELVETICA_12, this->name, this->color);
  };

  void set_date(date& d){
    calculate_position();
  }
  
  void update()
  {
    calculate_position();
    glutPostRedisplay();
  };

  float distance(Orbitor& o)
  {
    return sqrt(pow((o.x - x ),  2)  + pow((o.y - y ), 2)) * SCALING;
  };
};

void reverse(){
   direction = direction * -1;
}

void start_stop(){
    do_update = ! do_update;
}

void speed_up(){
  time_delta = time_delta + 1;
  if (time_delta > 240 ){
    time_delta = 240 ;
  }
}

void slow_down(){
  time_delta = time_delta - 1;
  if (time_delta <= 0.0 ){
    time_delta = 1;
  }
}

void mouseClicks(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    start_stop();
  }
  if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    reverse();
  }
  if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    distances_table();
  }
}

void keyPressed (unsigned char key, int x, int y){

  if (key == 't') {
    distances_table();
  }
  if (key == 'r'){
    reverse();
  }
  if (key == 's'){
    start_stop();
  }
  if (key == '+'){
    speed_up();
  }
  if (key == '-'){
    slow_down();
  }
  if (key == 'q'){
    exit(0);
  }  
}

std::vector<Orbitor> orbitors = {
  Orbitor(0.25,   -98, 35.0,    115.88,  GRAY,  "Mercury"),
  Orbitor(0.25,   157, 67.0,    224.70,  GREEN, "Venus"),
  Orbitor(0.25,     0, 93.0,    365.25,  BLUE,  "Earth"),
  Orbitor(0.25,  -165, 142.0,   779.94,  RED,   "Mars"),
  Orbitor(0.25,  -230, 275.0,  1682.00,  GRAY,  "Ceres"),
  Orbitor(0.25,      3, 280.0,  1821.00, GREEN, "Psyche"),
  Orbitor(0.45,   -100, 484.0,  4333.00, RED,   "Jupiter")//,
  //  Orbitor(0.45,    -60, 887.0, 10759.00, BLUE,  "Saturn")
};


void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT); 
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    for (Orbitor o : orbitors){
      o.display();
    }
   
    glColor3f(YELLOW.red,YELLOW.green,YELLOW.blue);
    drawCircle(0.0, 0.0, 0.75);
    render_string(0.75, 0.0, GLUT_BITMAP_HELVETICA_12, "Sun", YELLOW);

    std::stringstream c_date;
    c_date << "time_delta:  "<< time_delta <<"hours " << endl
	   << "date: "<< current_time.year()
	   << "-" <<  current_time.month()
	   << "-" <<  current_time.day();

    render_string(-30.0, 30.0, GLUT_BITMAP_HELVETICA_12, c_date.str().c_str(), YELLOW);

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void update_time(){
  ptime t(current_time);
  if (direction > 0){
    t += hours(time_delta);
  }else{
    t -= hours(time_delta);
  }
  current_time = t.date();
}

void update()
{
  if (! do_update)
    {
      return;
    }
  for (std::vector<Orbitor>::iterator it = orbitors.begin() ;
       it != orbitors.end();
       ++it)
    {
      it->update();
    }
  update_time();
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
  int triangleAmount = 20;
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

void distances_table(){
  int w = 7;
  cout << setw(w) << " " << " | " ;
  for (Orbitor o : orbitors){
    cout << setw(w) << o.name << " | " ;
  }
  cout << endl;  
  cout << "--------|" ;
  for (Orbitor o : orbitors){
    cout << "---------|" ;
  }
  cout << endl;  

  for (Orbitor o : orbitors){
    cout << setw(w) << o.name << " | " ;
      for (Orbitor t : orbitors){
	cout << setw(w) << o.distance(t) << " | ";
      }
      cout << endl;  
  }
  cout << "distances in millions of miles" << endl;  
  cout << endl;

}


int orbits(int argc, char** argv)
{
  
  if (argc > 1)
    {
      cout << argv[1] << endl;
      s = std::string(argv[1]);
      current_time = date(from_simple_string(s));
      for (Orbitor o : orbitors){
	o.set_date(current_time);
      }
    }
  
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Solar System Orbital Positions");
    glutMouseFunc(mouseClicks);
    glutKeyboardFunc(keyPressed);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}
