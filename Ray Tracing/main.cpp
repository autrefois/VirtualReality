#include <cmath>
#include <iostream>
#include <string>

#include "Vector.hpp"
#include "Line.hpp"
#include "Geometry.hpp"
#include "Sphere.hpp"
#include "Image.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Material.hpp"

#include "Scene.hpp"

using namespace std;
using namespace rt;

double imageToViewPlane(int n, int imgSize, double viewPlaneSize) {
	double u = (double)n*viewPlaneSize/(double)imgSize;
	u -= viewPlaneSize/2;
	return u;
}

const Intersection findFirstIntersection(const Line& ray, double minDist, double maxDist) 
{
	Intersection intersection;

	for(int i=0; i<geometryCount; i++) {
		Intersection in = scene[i]->getIntersection(ray, minDist, maxDist);
		if(in.valid()) {
			if(!intersection.valid()) {
				intersection = in;
			}
			else if(in.t() < intersection.t()) {
				intersection = in;
			}
		}
	}

	return intersection;
}

void drawSphere(Sphere* sphere, Image image) // not used 
{	
	/* A point is in the circle <=> x^2 + y^2 <= r^2 */
	for(double y=-sphere->radius(); y<=sphere->radius(); y++)
		for(double x=-sphere->radius(); x<=sphere->radius(); x++)
			//	for(double z=-sphere->radius(); z<=sphere->radius(); z++)
				if(x*x+y*y <= sphere->radius()*sphere->radius()&& 
					sphere->center().x() + x >= 0 && 
					sphere->center().y() + y >= 0 &&
					//			sphere->center().z() + z >= 0 &&
					sphere->center().x() + x <= image.width() &&
					sphere->center().y() + y <= image.height())
					image.setPixel(sphere->center().x() + x, sphere->center().y() + y, sphere->color());


};

int main() {
	Vector viewPoint(0, 0, 0);
	Vector viewDirection(0, 0, 1);
	Vector viewUp(0, -1, 0);

	double frontPlaneDist = 2;
	double backPlaneDist = 1000;

	double viewPlaneDist = 100;
	double viewPlaneWidth = 200;
	double viewPlaneHeight = 100;

	int imageWidth = 1000;
	int imageHeight = 500;

	int geometryCount = 1;

	Vector viewParallel = viewUp^viewDirection;

	viewDirection.normalize();
	viewUp.normalize();
	viewParallel.normalize();

	Line *ray = new Line(viewPoint, viewDirection, true);

	/*
	Sphere *greenSphere = new Sphere(140, 80, 20, 30, 0, 112, 0);
	Sphere *whiteSphere = new Sphere(140, 118, 20, 10, 255, 255, 255);
	Sphere *blueSphere = new Sphere(36, 25, 20, 110, 0, 0, 112);
	Sphere *redSphere = new Sphere(180, 100, 20, 25, 112, 0, 0);
	Sphere *yellowSphere = new Sphere(155, 85, 20, 15, 255, 215, 0);
	Sphere *pinkSphere = new Sphere(250, 80, 20, 20, 255, 0, 255);
	Sphere *cyanSphere = new Sphere(60, 80, 20, 40, 0, 255, 255); */

	Image image(imageWidth, imageHeight);

	Color lightColor(1.0,1.0,1.0);
	// ambient light, diffuse light, specular light, shininess, opacity, reflectivity
	Material lightMaterial(lightColor, lightColor, lightColor, 0, 0, 0);

	for (int y = 0; y < imageHeight; ++y) {
		for (int x = 0; x < imageWidth; ++x) {
			// direction vector 
			double viewPlaneY = imageToViewPlane(y, imageHeight, viewPlaneHeight);
			double viewPlaneX = imageToViewPlane(x, imageWidth, viewPlaneWidth);

			Vector rayDirection(0, 0, 0);
			rayDirection = rayDirection + viewPoint + viewDirection * viewPlaneDist + viewUp * viewPlaneY + viewParallel * viewPlaneX;

			Line ray(viewPoint, rayDirection, false);
			Intersection firstIntersection = findFirstIntersection(ray, frontPlaneDist, backPlaneDist);
			if (firstIntersection.valid())
			{
				//	Color color = firstIntersection.geometry().color();
				//	image.setPixel(x, y, color);

				Vector l(50, 0, 50); // light position vector
				Vector c = viewPoint; // camera position vector
				Vector v = firstIntersection.vec(); // intersection position vector
				Vector e = c - v; //ray.dx(); // vector from the intersection to the camera 
				e.normalize();
				Vector n = v - ((Sphere *)(&firstIntersection.geometry()))->center(); //v - firstIntersection.sphereCenter(); // normal to the surface at the intersection point -> sphere center to intersection point
				n.normalize();
				Vector t = l - v; // vector from the intersection point to the light
				t.normalize();
				Vector r = n * (n * t) * 2 - t;
				r.normalize();

			//	cout << n.x() << " " << n.y() << " " << n.z() << "           " << t.x() << " " << t.y() << " " << t.z() << endl;

				Color materialAmbient = firstIntersection.geometry().material().ambient();
				Color lightAmbient = lightMaterial.ambient();
				Color materialDiffuse = firstIntersection.geometry().material().diffuse();
				Color lightDiffuse = lightMaterial.diffuse();
				Color materialSpecular = firstIntersection.geometry().material().specular();
				Color lightSpecular = lightMaterial.specular();
				double materialShininess = firstIntersection.geometry().material().shininess();

				// Ambient Color
				Color color(materialAmbient * lightAmbient);

				// Diffuse Color
				if (n*t > 0) {
					color += materialDiffuse*lightDiffuse*(n*t);
				}

				// Specular Color
				if (e*r > 0)
				{
					color += materialSpecular * lightSpecular * pow(e*r, materialShininess);
				}
				
				 color *= 1; //lightMaterial.intensity;

				image.setPixel(x, y, color);
			}
		}
	}

	/*
	drawSphere(whiteSphere, image);
	drawSphere(greenSphere, image);
	drawSphere(blueSphere, image);
	drawSphere(redSphere, image);
	drawSphere(yellowSphere, image);
	drawSphere(cyanSphere, image);
	drawSphere(pinkSphere, image); */

	image.store("bigScene.ppm");

	for(int i=0; i<geometryCount; i++) {
		delete scene[i];
	}

	return 0;
}
