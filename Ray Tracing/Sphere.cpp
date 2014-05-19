#include "Sphere.hpp"
#include <ostream>;

using namespace rt;
using namespace std;

Intersection Sphere::getIntersection(const Line& line, 
                                     double minDist,
                                     double maxDist) {
Intersection in;

  double a = line.dx().x()*line.dx().x() + line.dx().y()*line.dx().y() + line.dx().z()*line.dx().z();
  double b = 2*line.dx().x()*line.x0().x() + 2*line.dx().y()*line.x0().y() + 2*line.dx().z()*line.x0().z() 
			 - 2*line.dx().x()*_center.x() - 2*line.dx().y()*_center.y() - 2*line.dx().z()*_center.z();
  double c = line.x0().x()*line.x0().x() + line.x0().y()*line.x0().y() + line.x0().z()*line.x0().z()
			 - 2*line.x0().x()*_center.x() - 2*line.x0().y()*_center.y() - 2*line.x0().z()*_center.z()
			 + _center.x()*_center.x() + _center.y()*_center.y() + _center.z()*_center.z() - _radius*_radius;

  double delta = b*b-4*a*c;
  double t;
  in.sphereCenter(_center);
  if (delta > 0) {
	double x1 = -b+sqrt(delta) / 2*a;
	double x2 = -b-sqrt(delta) / 2*a;
	if (x1 > x2)	{
		t = x2;
	} else {
		t = x1;
	}
  } else if (delta==0) {
	t = -b / 2*a;
  } else {
	return in;
  }

  Intersection inter(true, *this, line, t);
  inter.sphereCenter(_center);

  double length = (inter.vec() - inter.line().x0()).length();
  if (  length > maxDist  || length < minDist ) {
	  return in;
  }

  return inter;
}

