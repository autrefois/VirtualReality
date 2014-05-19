#include "Vector.hpp"
#include "Line.hpp"
#include "Intersection.hpp"
#include "Geometry.hpp"
#include "Color.hpp"

#ifndef RT_SPHERE_INCLUDED
#define RT_SPHERE_INCLUDED

namespace rt {

  class Sphere : public Geometry {
  private:
    Vector _center;
    double _radius;

  public:
    Sphere(const Vector& center, double radius, const Color& color, Material& material) 
    : Geometry(color, material) {
      _center = Vector(center);
      _radius = radius;
    }

    Sphere(double x, double y, double z, double radius,
           double r, double g, double b,
		   double ra, double ga, double ba,
		   double rd, double gd, double bd,
		   double rs, double gs, double bs,
		   double shininess, double opacity, double reflectivity) 
		   : Geometry (Color(r, g, b), Material(Color(ra, ga, ba), Color(rd, gd, bd), Color(rs, gs, bs), shininess, opacity, reflectivity)) {
      _center = Vector(x, y, z);
      _radius = radius;
    }

    virtual Intersection getIntersection(const Line& line, 
                                         double minDist,
                                         double maxDist);

    inline double& radius() {
      return _radius;
    }

    inline Vector& center() {
      return _center;
    }
  };
}

#endif
