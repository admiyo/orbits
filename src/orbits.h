#pragma once

#ifdef _WIN32
  #define orbits_EXPORT __declspec(dllexport)
#else
  #define orbits_EXPORT
#endif

orbits_EXPORT int orbits(int argc, char** argv)
;
