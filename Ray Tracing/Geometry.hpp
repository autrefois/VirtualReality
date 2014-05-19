#include "Material.hpp"
#include "Color.hpp"
#include "Vector.hpp"
#include "Line.hpp"
#include "Intersection.hpp"

#ifndef RT_GEOMETRY_INCLUDED
#define RT_GEOMETRY_INCLUDED

namespace rt {

  class Geometry {
  private:
    Color _color;
	Material _material;

  public:
	  Geometry(const Color& color, Material& material) {
      _color = Color(color);
	  _material = Material(material);
    }

    Geometry(Geometry& geometry) {
      _color = Color(geometry.color());
	  _material = Material(geometry.material());
    }

    virtual Intersection getIntersection(const Line& line, 
                                         double minDist,
                                         double maxDist) {
      Intersection in(false, *this, line, 0);
      return in;
    };

    inline Color color() const {
      return _color;
    }

	inline Material material() {
		return _material;
	}

  };
}

#endif
