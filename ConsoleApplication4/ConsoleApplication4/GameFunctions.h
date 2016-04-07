#ifndef GAMEFUNC_H
#define GAMEFUNC_H

#include "glm\glm.hpp"
#include <cmath>
#include <math.h>
#include <iostream>

int GetRandomInt(int min, int max);
bool FileExists(const std::string& path);
glm::vec3 ArrayToVec(double arr[3]);
void CopyVector(double *dest, double targ[3]);

glm::vec3 RayShot(glm::vec3 origin, glm::vec3 direction, glm::vec2 xv, glm::vec2 yv, glm::vec2 zv);
glm::vec3 GetClosestPoint(glm::vec3 origin, glm::vec3 line, glm::vec3 point);
glm::vec3 PlaneIntersection(glm::vec3 lineDir, glm::vec3 linePoint, glm::vec3 normal, glm::vec3 planePoint);
glm::vec3 NormalOfFace(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
bool IsCoplanarPointOnFace(glm::vec3 point, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
bool IsPointInPrism(glm::vec3 point, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);

double GetAngleRad(glm::vec2 origin, glm::vec2 targ);
double GetAngleDeg(glm::vec2 origin, glm::vec2 targ);

glm::vec2 GetXZ(glm::vec3 temp);

#endif