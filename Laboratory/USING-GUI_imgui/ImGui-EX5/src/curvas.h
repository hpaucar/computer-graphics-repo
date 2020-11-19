#ifndef CURVAS_H
#define CURVAS_H

#include <glm\glm.hpp>
#include <vector>
#include <cmath>

/*
* Curva cuadratica de Bezier
* P = (P0)(1-t)^2 + (P1)2t(1-t) + (P2)t^2 , t: [0,1]
*/
void addPointsCurveG2(std::vector<double> &vert, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, int n_points);

/*
* Curva cuadratica de Bezier - Con modificaciones:
* Para dirigir luego de cada dos puntos un punto en direccion a la normal de la curva en
* ese punto, generando asi que los puntos de la curva se formen en forma de cerros sobre la superficie
* de la curva (como el pelo de Bart).
*/
void addPointsPelo(std::vector<double> &vert, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, int n_points, double tam_pelo);

/*
* Curva cubica de Bezier
* P = (P0)(1-t)^3 + (P1)3t(1-t)^2 + (P2)3t^2(1-t) + (P3)t^3 , t: [0,1]
*/
void addPointsCurveG3(std::vector<double> &vert, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, int n_points);

/*
* Curva grado 4 de Bezier
* P = (P0)(1-t)^4 + (P1)3t(1-t)^3 + (P2)6t^2(1-t)^2 + (P3)4t^3(1-t) + (P4)t^4 , t: [0,1]
*/
void addPointsCurveG4(std::vector<double> &vert, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, int n_points);

/*
* Circunferencia
* P = (r * sin(t), r * cos(t)) , t: [-1,1]
*/
void addPointCircle(std::vector<double> &vert, double r, glm::vec2 center, int n_points);
#endif