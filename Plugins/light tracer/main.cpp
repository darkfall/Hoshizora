#include <iostream>

#include <cmath>

#include "sphere.h"
#include "perspectiveCamera.h"
#include "color.h"
#include "checkerMaterial.h"
#include "phongMaterial.h"
#include "planeMaterial.h"
#include "camera.h"
#include "geometrySet.h"
#include "plane.h"
#include "lightSet.h"
#include "directionLight.h"
#include "pointLight.h"
#include "spotLight.h"

#include <map>
#include <vector>
#include <algorithm>

#include "glfw/gl/glfw.h"

#define W 480
#define H 480

#define MAX_DEPTH 500.f

using namespace rfMath;

#include <cmath>

perspectiveCamera pcam(vector3(0, 10, 20), vector3(0, 0, -1), vector3(0, 1, 0), 90);
geometrySet geoSet;
lightSet ligSet;

#include <sys/timeb.h>

timeb currtime;
timeb oldtime;
float dt;

#define SPEED 15

float mousex = 0.f, mousey = 0.f;

void init() {
	geoSet.add(new sphere(vector3(-10, 10, -10), 10, new phongMaterial(RED_COLOR, WHITE_COLOR, 16, 0.3f)));
	geoSet.add(new sphere(vector3(10, 10, -10), 10, new phongMaterial(BLUE_COLOR, WHITE_COLOR, 16, 0.3f)));
	geoSet.add(new plane(vector3(0, 1, 0), 0, new checkerMaterial(0.1f, 0.5f)));
	
	mousex = 0;
	mousey = W/2;
	ftime(&oldtime);
}

void initDirectionLightGeometry() {
	geoSet.add(new plane(vector3(0, 1, 0), 0, new checkerMaterial(0.1f, 0.5f)));
	geoSet.add(new plane(vector3(0, 0, 1), -50, new planeMaterial(0.8f)));
	geoSet.add(new plane(vector3(1, 0, 0), -20, new planeMaterial(0.8f)));
	geoSet.add(new sphere(vector3(0, 10, -10), 10, new phongMaterial(RED_COLOR, WHITE_COLOR, 16, 0.5f)));
}

void initPointLight() {
/*	for(int i=10; i<=40; i+=10) 
		for(int j=20; j<=50; j+=10)
			ligSet.add(new pointLight(WHITE_COLOR*10, vector3(i, 50, j)));
	ligSet.add(new directionLight(RED_COLOR*0.25, vector3(1.5, 1, 0.5)));*/
	ligSet.add(new spotLight(WHITE_COLOR*2000, vector3(30, 40, 20), vector3(-1, -1, -1), 20, 30, 0.5));
	//ligSet.add(new pointLight(WHITE_COLOR*2000, vector3(30, 40, 20)));
	//ligSet.add(new pointLight(BLUE_COLOR*20000, vector3(W/2, W/4, W/6)));
	//ligSet.add(new directionLight(WHITE_COLOR, vector3(-1.75, -2, -1.5)));
}

color rayTraceRecursive(geometry* scene, ray& r, int maxReflect) {
	intersectResult result = scene->intersect(r);
	if(result.geo) {
		float reflectiveness = result.geo->mat->reflectiveness;
		color c = result.geo->mat->sample(r, result.position, result.normal);
		c = c*(1.f-reflectiveness);
		
		if(reflectiveness > 0.f && maxReflect > 0) {
			vector3 dir = result.normal * (-2 * result.normal.dot(r.direction)) + r.direction;
			ray cray(result.position, dir);
			color reflectedColor = rayTraceRecursive(scene, cray, maxReflect-1);
			c = c + reflectedColor * reflectiveness;
		}
		return c;
	}
	return BLACK_COLOR;
}

void rayTrace(geometry* scene, camera* cam, int maxReflect) {
	for(int y=0; y<H; ++y) {
		float sy = 1.f - (float)y/H;
		for(int x=0; x<W; ++x) {
			float sx = (float)x/W;
			ray cray = cam->generateRay(sx, sy);
			color c = rayTraceRecursive(scene, cray, maxReflect);
			glColor3f(c.r, c.g, c.b);
			glVertex2f(x, y);
		}
	}
}

void lightRender(geometry* scene, camera* cam, lightSet* lights) {
	for(int y=0; y<H; ++y) {
		float sy = 1.f - (float)y/H;
		for(int x=0; x<W; ++x) {
			float sx = (float)x/W;
			ray cray = cam->generateRay(sx, sy);
			intersectResult result = scene->intersect(cray);
			color c = BLACK_COLOR;
			if(result.geo != NULL) { 
				c = lights->sample(scene, &result);
			} 
			glColor3f(c.r, c.g, c.b);
			glVertex2f(x, y);

		}
	}
}

void mix(geometry* scene, camera* cam, lightSet* lights, int maxReflect, float weight=0.5f) {
	for(int y=0; y<W; ++y) {
		float sy = 1.f - (float)y/W;
		for(int x=0; x<H; ++x) {
			float sx = (float)x/H;
			ray cray = cam->generateRay(sx, sy);
			color c1 = rayTraceRecursive(scene, cray, maxReflect);
			
			intersectResult result = scene->intersect(cray);
			color c2 = BLACK_COLOR;
			if(result.geo != NULL) { 
				c2 = lights->sample(scene, &result);
			} 
			glColor3f(c1.r*weight+c2.r*(1.f-weight), 
					  c1.g*weight+c2.g*(1.f-weight), 
					  c1.b*weight+c2.b*(1.f-weight));
			//glColor3f(c2.r, c2.g, c2.b);
			glVertex2f(x, y);
		}
	}
}



float prevMouseWheelPos = 0.f;
void mouseCallback(int key, int action) {
	if(key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		int tmx, tmy;
		glfwGetMousePos(&tmx, &tmy);
		tmx -= W/2;
		tmy -= H/2;
	}
}

void updateKey() {
	float rx = 0.f, ry = 0.f, rz = 0.f;
	
	if(glfwGetKey('W') == GLFW_PRESS)
		rz -= SPEED;
	if(glfwGetKey('S') == GLFW_PRESS)
		rz += SPEED;
	if(glfwGetKey('A') == GLFW_PRESS)
		rx -= SPEED;
	if(glfwGetKey('D') == GLFW_PRESS)
		rx += SPEED;
	if(glfwGetKey('Q') == GLFW_PRESS)
		ry -= SPEED;
	if(glfwGetKey('E') == GLFW_PRESS)
		ry += SPEED;
	if(glfwGetKey('0') == GLFW_PRESS) {
		pcam.setFront(vector3(0, 0, -1));
		pcam.setEye(vector3(0, 10, 10));
	}
	
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		int tmx, tmy;
		glfwGetMousePos(&tmx, &tmy);
		tmx -= W/2;
		
		mousex = tmx;
		mousey = H/2 + (H/2-tmy);
		
		vector3 front(mousex/(W/2), mousey/(H/2), -1);
		
		pcam.setFront(front.normalize());
	} else {
		
	}
	pcam.rotate(rx, ry, rz);
	
	float curMouseWheelPos;
	if(curMouseWheelPos = glfwGetMouseWheel()) {
		pcam.rotate(0.f, 0.f, prevMouseWheelPos-curMouseWheelPos);
		prevMouseWheelPos = curMouseWheelPos;
	}
	
}


void render() {
	updateKey();
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	
	glBegin(GL_POINTS);
	rayTrace(&geoSet, &pcam, 3);
	//lightRender(&geoSet, &pcam, &ligSet); 
	//mix(&geoSet, &pcam, &ligSet, 3, 0.5f);
	
	glEnd();
	
	glFlush();
	
	glfwSwapBuffers();
	
	ftime(&currtime);
	dt = (float)((currtime.millitm - oldtime.millitm) + 1000*(currtime.time-oldtime.time)) / 1000.f;
	oldtime = currtime;
	printf("fps:%f\n", 1.f/dt);
}

int main (int argc, char * const argv[]) {
	glfwInit();
	glfwOpenWindow(W, H, 0, 0, 0, 0, 16, 0, GLFW_WINDOW);
	glfwSetWindowTitle("cg test");
	glfwSetWindowPos(200, 200);
	glfwSetMouseButtonCallback(mouseCallback);
	//glfwSetWindowRefreshCallback(render);
	
	glClearColor(1.f, 1.f, 1.f, 1.f);

	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, W, H, 0);
	//glOrtho(0, 1, 0, 1, -1, 1);
	
	init();
	//initDirectionLightGeometry();
	//initPointLight();
	while(true) {
		render();
	}
	return 0;
}
