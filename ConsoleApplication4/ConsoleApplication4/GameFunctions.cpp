#include "GameFunctions.h"
#include <sys/stat.h>

#define PI 3.1415926535897932384626433832795

bool FileExists(const std::string& path){
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

int GetRandomInt(int min, int max){
	return min + (rand() % (int)(max - min + 1));
}

glm::vec3 ArrayToVec(double arr[3]){
	return glm::vec3(arr[0], arr[1], arr[2]);
}

void CopyVector(double *dest, double targ[3]){
	*dest = targ[0]; *(dest + 1) = targ[1]; *(dest + 2) = targ[2];
}

double GetAngleRad(glm::vec2 origin, glm::vec2 targ){
	targ -= origin;

	if (targ.x == 0){
		if (targ.y > 0){
			return PI / 2;
		}
		else{
			return 3 * PI / 2;
		}
	}

	double temp = atan(abs(targ.y / targ.x));
	if (targ.x < 0 && targ.y > 0){
		temp = PI - temp;
	}
	else if (targ.x < 0 && targ.y < 0){
		temp += PI;
	}
	else if (targ.x > 0 && targ.y < 0){
		temp = 2 * PI - temp;
	}

	return temp;
}

double GetAngleDeg(glm::vec2 origin, glm::vec2 targ){
	return GetAngleRad(origin, targ) * 180 / PI;
}

glm::vec2 GetXZ(glm::vec3 temp){
	return glm::vec2(temp.x, temp.z);
}

glm::vec3 PlaneIntersection(glm::vec3 lineDir, glm::vec3 linePoint, glm::vec3 normal, glm::vec3 planePoint){
	if (glm::dot(lineDir, normal) == 0){//If line never touches plane, give line origin
		return linePoint;
	}
	return (glm::dot((planePoint - linePoint), normal) / glm::dot(lineDir, normal))*lineDir + linePoint;
}

glm::vec3 NormalOfFace(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3){
	return glm::cross((point2 - point1), (point3 - point1));
}

bool ValIsBetween(glm::float32 check, glm::float32 v1, glm::float32 v2){
	glm::float32 big, small;
	if (v2 < v1){
		big = v1;
		small = v2;
	}
	else{
		big = v2;
		small = v1;
	}
	if (check <= big && check >= small){
		return true;
	}
	return false;
}

bool IsCoplanarPointOnFace(glm::vec3 point, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
	if ((glm::dot(v1, v2 - v1) <= glm::dot(point, v2 - v1) && glm::dot(point, v2 - v1) <= glm::dot(v2, v2 - v1)) &&
		(glm::dot(v1, v3 - v1) <= glm::dot(point, v3 - v1) && glm::dot(point, v3 - v1) <= glm::dot(v3, v3 - v1))){
		return true;
	}
	return false;
}

bool IsPointInPrism(glm::vec3 point, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4){
	glm::vec3 u, v, w;
	u = v1 - v2;
	v = v1 - v3;
	w = v1 - v4;
	glm::float32 prod[3];
	prod[0] = glm::dot(u, point);
	prod[1] = glm::dot(v, point);
	prod[2] = glm::dot(w, point);
	if (ValIsBetween(prod[0], glm::dot(u, v1), glm::dot(u, v2)) && ValIsBetween(prod[1], glm::dot(v, v1), glm::dot(v, v3)) &&
		ValIsBetween(prod[2], glm::dot(w, v1), glm::dot(w, v4))){
		return true;
	}
	return false;
}

glm::vec3 RayShot(glm::vec3 origin, glm::vec3 direction, glm::vec2 xv, glm::vec2 yv, glm::vec2 zv){
	xv[0] -= origin.x; xv[1] -= origin.x;//Make relative to origin
	yv[0] -= origin.y; yv[1] -= origin.y;
	zv[0] -= origin.z; zv[1] -= origin.z;

	glm::vec2 test;//Other 2 calculated coordinates.
	glm::vec2 val = glm::vec2(0, 0);//Values of last hit distance and it's X val.
	glm::uint temp = 0;//Temporary value of distance
	glm::vec3 result = glm::vec3(0, 0, 0);//Final result
	glm::float32 angle = (float)GetAngleRad(glm::vec2(0, 0), glm::vec2(direction.x, direction.z));
	glm::float32 test_angle;

	for (int i = 0; i <= 1; i++){
		test[0] = direction.y / direction.x * xv[i];
		test[1] = direction.z / direction.y * test[0];

		test_angle = GetAngleRad(glm::vec2(0, 0), glm::vec2(xv[i], test[1])) - angle;
		if (test_angle > PI/2 || test_angle < -PI/2){ continue; }//Skip if ray shoots backwards

		if (test[0] >= yv[0] && test[0] <= yv[1] && test[1] >= zv[0] && test[1] <= zv[1]){
			temp = sqrt(pow(xv[i], 2) + pow(test[0], 2) + pow(test[1], 2));
			if (temp < val[0] || val[0] == 0){
				val[0] = temp;
				val[1] = xv[i];
			}
		}
	}

	for (int i = 0; i <= 1; i++){
		test[0] = direction.x / direction.y * yv[i];
		test[1] = direction.z / direction.x * test[0];

		test_angle = GetAngleRad(glm::vec2(0, 0), glm::vec2(test[0], test[1])) - angle;
		if (test_angle > PI / 2 || test_angle < -PI / 2){ continue; }//Skip if ray shoots backwards

		if (test[0] >= xv[0] && test[0] <= xv[1] && test[1] >= zv[0] && test[1] <= zv[1]){
			temp = sqrt(pow(yv[i], 2) + pow(test[0], 2) + pow(test[1], 2));
			if (temp < val[0] || val[0] == 0){
				val[0] = temp;
				val[1] = test[0];
			}
		}
	}

	for (int i = 0; i <= 1; i++){
		test[0] = direction.y / direction.z * zv[i];
		test[1] = direction.x / direction.y * test[0];

		test_angle = GetAngleRad(glm::vec2(0, 0), glm::vec2(test[1], zv[i])) - angle;
		if (test_angle > PI / 2 || test_angle < -PI / 2){ continue; }//Skip if ray shoots backwards

		if (test[0] >= yv[0] && test[0] <= yv[1] && test[1] >= xv[0] && test[1] <= xv[1]){
			temp = sqrt(pow(zv[i], 2) + pow(test[0], 2) + pow(test[1], 2));
			if (temp < val[0] || val[0] == 0){
				val[0] = temp;
				val[1] = test[1];
			}
		}
	}

	if (val[0] != 0){//If ray hit, set up result vector
		result.x = val[1];
		result.y = direction.y / direction.x * val[1];
		result.z = direction.z / direction.x * val[1];
		result += origin;
	}
	
	return result;
}

glm::vec3 GetClosestPoint(glm::vec3 origin, glm::vec3 line, glm::vec3 point){
	float t = glm::dot(point - origin, line - origin) / pow(glm::length(line - origin), 2);
	if (t < 0){
		return origin;
	}
	return line * t + origin;
}