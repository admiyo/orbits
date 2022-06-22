#pragma once
#include <boost/date_time/gregorian/gregorian.hpp>

#ifdef _WIN32
  #define orbits_EXPORT __declspec(dllexport)
#else
  #define orbits_EXPORT
#endif

struct Color {
  float red;
  float green;
  float blue;
};


class Orbitor {
  friend class Ship;
  Color color;
  float x;
  float y;
  float radius;
  float orbit_radius = 1.0;
  float start_rad;
  float period;
  double mass;

public:
  const char * name;
  
public:
  Orbitor(float radius, float rads, float orbit_radius, float period,
	  Color& color, const char * name, double mass);
  void calculate_position();
  void display() ;
  void set_date(boost::gregorian::date& d);
  void update();
  float distance(Orbitor& o);
};


orbits_EXPORT int orbits(int argc, char** argv)
;
