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

#include "orbits.h"

const float PI = 3.14;

Color WHITE =  {1.0, 1.0, 1.0};
Color RED =    {1.0, 0.0, 0.0};
Color YELLOW = {1.0, 1.0, 0.0};
Color GREEN =  {0.0, 1.0, 0.0};
Color BLUE =   {0.0, 0.0, 1.0};
Color GRAY =   {0.5, 0.5, 0.5};

void draw_disk(float x, float y, float radius );
void draw_circle(float x, float y, float radius );
void display(void);
void distances_table();

bool do_update = false;
int direction = 1;
const int SCALING = 10;
std::string s("2022-06-15");
date base_time(from_simple_string(s));
date current_date;
int day_count = 0;
int time_delta = 24;



void render_string(float x, float y, void *font, const char* text, Color const& rgb)
{
  char *c;
  glColor3f(rgb.red, rgb.green, rgb.blue);
  glRasterPos2f(x, y);
  glutBitmapString(font, (unsigned char *) text);
}


Orbitor::Orbitor(float radius, float rads, float orbit_radius, float period,
	  Color& color, const char * name, double mass)
  {
    this->color = color;
    this->period = period;
    this->radius = radius;
    this->orbit_radius = orbit_radius / SCALING;
    this->name = name;
    this->mass = mass;
    this->start_rad = rads;
    calculate_position();
  };

  void Orbitor::calculate_position(){

    if (0.0 == period){
      x = 0;
      y = 0;
      return;
    }
    
    int d = (current_date - base_time).days();
    double delta =  d / period;
    double rads = start_rad + delta;

    float old_x = x;
    float old_y = y;
    x = cos(rads * 2 * PI) * this->orbit_radius;
    y = sin(rads * 2 * PI) * this->orbit_radius;
    this->dv_x = x - old_x;
    this->dv_y = y - old_y;
  }
  
  void Orbitor::display()
  {
    glColor3f(color.red,color.green,color.blue);
    draw_disk(this->x, this->y, radius);
    if (orbit_radius > 0.0){
      glColor3f(WHITE.red,WHITE.green,WHITE.blue);
      draw_circle(0, 0, orbit_radius);
    }
    render_string(this->x, this->y, GLUT_BITMAP_HELVETICA_12, this->name, this->color);
  };

  
  void Orbitor::update()
  {
    calculate_position();
    glutPostRedisplay();
  };

  float Orbitor::distance(Orbitor& o)
  {
    return sqrt(pow((o.x - x ),  2)  + pow((o.y - y ), 2)) * SCALING;
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
  Orbitor(0.50,     0,  0.1,      0.0,  YELLOW,"Sun",
	  19890000000000000000000000000000.0),
  Orbitor(0.25,   -98, 35.0,    115.88,  GRAY,  "Mercury", 1.0),
  Orbitor(0.25,   157, 67.0,    224.70,  GREEN, "Venus", 1.0),
  Orbitor(0.25,     0, 93.0,    365.25,  BLUE,  "Earth",
              59720000000000000000000000.0),
  Orbitor(0.25,  -165, 142.0,   779.94,  RED,   "Mars",
	       6391000000000000000000000.0),
  Orbitor(0.25,  -230, 275.0,  1682.00,  GRAY,  "Ceres",
                   910000000000000000000.0),
  Orbitor(0.25,      3, 280.0,  1821.00, GREEN, "Psyche", 1.0),
  Orbitor(0.45,   -100, 484.0,  4333.00, RED,   "Jupiter", 1.0),
  Orbitor(0.45,    -60, 887.0, 10759.00, BLUE,  "Saturn",
	  568300000000000000000000000.0)
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

    std::stringstream c_date;
    c_date << "time_delta:  "<< time_delta <<"hours " << endl
	   << "date: "<< current_date.year()
	   << "-" <<  current_date.month()
	   << "-" <<  current_date.day();

    render_string(-30.0, 30.0, GLUT_BITMAP_HELVETICA_12, c_date.str().c_str(), YELLOW);

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void update_time(){
  ptime t(current_date);
  if (direction > 0){
    t += hours(time_delta);
  }else{
    t -= hours(time_delta);
  }
  if (current_date == t.date()){
    cout << "whoops" <<endl;
  }

  
  current_date = t.date();
}

void update()
{
  if (! do_update)
    {
      return;
    }
  update_time();
  for (std::vector<Orbitor>::iterator it = orbitors.begin() ;
       it != orbitors.end();
       ++it)
    {
      it->update();
    }
}

void reshape(int w, int h)
{
  glViewport(0,0,(GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void draw_disk(float x, float y, float radius )
{
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

void draw_circle(float x, float y, float radius )
{
  int i;
  int triangleAmount = radius * 20;
  GLfloat twicePi = 2.0f * PI;
  glBegin(GL_LINE_LOOP);
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
  std::string s("2022-06-15");
  base_time = date(from_simple_string(s));

  cout << "base time " << base_time << endl;
  
  if (argc > 1)
    {
      cout << argv[1] << endl;
      s = std::string(argv[1]);
      current_date = date(from_simple_string(s));
    }else{
      current_date = base_time;
    }

  cout << "current time " << current_date << endl;

  cout<< "days ="  << (current_date - base_time).days() << endl;
  
  
  for (std::vector<Orbitor>::iterator it = orbitors.begin() ;
       it != orbitors.end();
       ++it)
    {
      it->calculate_position();
    }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800, 800);
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
