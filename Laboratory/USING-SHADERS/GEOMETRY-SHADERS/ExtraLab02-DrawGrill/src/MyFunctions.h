/*
 * MyFunctions.h
 *
 *  Created on: 2 abr. 2020
 *      Author: Usuario
 */
#ifndef MYFUNCTIONS_H_
#define MYFUNCTIONS_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Point {
	glm::vec3 coord;
	glm::vec3 color;
};

class MyFunctions {

public:

	MyFunctions();
	virtual ~MyFunctions();

	static std::vector<glm::vec3> getRectanglePoints(glm::vec3 point0, float width, float height, const char *plane);
	static std::vector<Point> getRectanglePointsColor(glm::vec3 point0, float width, float height, const char *plane, glm::vec3 color);
	static std::vector<glm::vec3> getRectangle4Points(glm::vec3 point0, float width, float height, const char *plane);
	static std::vector<glm::vec3> getGrillPoints(glm::vec3 p0, float radio, int length, const char *plane);
	static std::vector<glm::vec3> getGrillQuadPoints(glm::vec3 p0, float radio, int length, const char *plane);


};

#endif /* MYFUNCTIONS_H_ */
