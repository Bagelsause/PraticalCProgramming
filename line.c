#include <stdio.h>
#include <math.h>

typedef struct{
	double x;
	double y;
} point;

double distance(point p1, point p2) {
	return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

typedef struct{
	point p1;
	point p2;
} line;

double length(line ln) {
	return distance(ln.p1, ln.p2);
}

double slope(line ln) {
	return ((ln.p1.x - ln.p2.x) / (ln.p1.y - ln.p2.y));
}

double y_intercept (line ln) {
	return ln.p1.y - slope(ln) * ln.p1.x;
}

int point_on_line(line ln, point p){
	return p.y == slope(ln) * p.x + y_intercept(ln);
}

int main( int argc, char *argv[] ) {
	point pont = {1, -1};
	point pint = {2, -2};
	point check = {3, -3};
	line lin = {pont, pint};
	
	printf("%d \n", point_on_line(lin, check));
	
	return 0;
}