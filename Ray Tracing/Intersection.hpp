#include <iostream>
#include "Vector.hpp"

#ifndef RT_INTERSECTION_INCLUDED
#define RT_INTERSECTION_INCLUDED

namespace rt {

  class Geometry;
  class Line;

  class Intersection {
  private:
    bool _valid;
    double _t;
    Vector _vec;
	Vector _sphereCenter;
    Geometry* _geometry;
    const Line* _line;

  public:
    Intersection();
    
    Intersection(bool b, Geometry& geometry, const Line& line, double t);
    
    inline bool valid() const {
      return _valid;
    }
    
    inline double t() const {
      return _t;
    }
    
    inline const Vector& vec() const {
      return _vec;
    }

	inline const Vector& sphereCenter() const {
      return _sphereCenter;
    }

	inline void sphereCenter (Vector sphereCenter) 
	{
		_sphereCenter = sphereCenter;
	}
    
    inline Geometry& geometry() const {
      return *_geometry;
    }

    inline const Line& line() const {
      return *_line;
    }
  };
}

#endif
