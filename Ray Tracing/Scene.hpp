#include "Geometry.hpp"
#include "Sphere.hpp"

using namespace rt;

int geometryCount = 7;



Geometry* scene[] = {

// red
  new Sphere(-50, -25, 175,
              30, 
			  1, 0, 0, 
			  0.2, 0, 0, // ambient
			  0.6, 0, 0, // diffuse 
			  0.7, 0.5, 0.5, // specular
			  50, 10, 20) // shininess, opacity, reflectivity
  ,

  // yellow
  new Sphere(-10, 0, 100,
              10, 
			  1, 1, 0,
			  0.2, 0.2, 0,  // ambient
			  0.6, 0.6, 0, // diffuse
			  0.7, 0.7, 0.5, // specular
			  50, 10, 20) // shininess, opacity, reflectivity			  
  ,
  // green
  new Sphere(  0, 0, 200,
               40, 
			   0, 1, 0,
			   0, 0.2, 0, // ambient
			   0, 0.6, 0, // diffuse 
			   0.5, 0.7, 0.5, // specular
			   50, 10, 120) // shininess, opacity, reflectivity
  ,
  // white
  new Sphere(  0, -50, 200,
               10, 
			   1, 1, 1,
		       0.2, 0.2, 0.2, // ambient
			   0.6, 0.6, 0.6, // diffuse 
			   0.7, 0.7, 0.7, // specular
			   50, 10, 120) // shininess, opacity, reflectivity
  ,
  // turquoise 
  new Sphere( 10,   0,  20,
               5, 
			   0, 1, 1,
			   0, 0.2, 0.2, // ambient
			   0, 0.6, 0.6, // diffuse 
			   0.5, 0.7, 0.7, // specular
			   50, 5, 100) // shininess, opacity, reflectivity
  ,
  // purple
  new Sphere(-70, 0, 100,
              10, 
			   1, 0, 1,
			   0.2, 0, 0.2, // ambient
			   0.6, 0, 0.6, // diffuse 
			   0.7, 0.5, 0.7, // specular
			   50, 10, 20) // shininess, opacity, reflectivity
  ,
  // blue
  new Sphere( 50, 25, 75, 
               50, 
			   0, 0, 1,
			   0, 0, 0.2, // ambient
			   0, 0, 0.6, // diffuse 
			   0.5, 0.5, 0.7, // specular
			   50, 10, 20) // shininess, opacity, reflectivity
};
