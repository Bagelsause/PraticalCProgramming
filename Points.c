#include <stdio.h>
#include <math.h>
typedef char *String;

typedef struct {
    double x;
    double y;
} point;

double perimeter(const point *points, size_t count) {
    double sum = 0;
    double dist = 0;
    for (int i = 0; i < count; i++) {
        if (i == count-1){
            dist = sqrt(pow((points[0].x - points[i].x), 2) + pow((points[0].y - points[i].y), 2));
            //printf("Distance at end: %f", dist);
            sum += dist;
        } else {
            dist = sqrt(pow((points[i].x - points[(i+1)].x), 2) + pow((points[i].y - points[i+1].y), 2));
            //printf("Distance before end: %f", dist);
            sum += dist;
        }
    }

    return sum;
}

double area(const point *points, size_t count) {
    double area = 0;
    for (int i = 1; i < count; i++){
        point p1, p2;
        if (i == count - 1) {
            //I tried using this code below, but it kept on saying "expected expression at the curly brace line"
            //p1 = {points[0].x, points[0].y};
            //p2 = {points[i].x, points[i].y};

            //I'm going to do some outside work related to solving this problem, but I'm using code below for actual run
            p1.x = points[0].x;
            p1.y = points[0].y;
            p2.x = points[i].x;
            p2.y = points[i].y;


        } else {
            //p1 = {points[i+1].x, points[i+1].y};
            //p2 = {points[i].x, points[i].y};

            p1.x = points[i+1].x;
            p1.y = points[i+1].y;
            p2.x = points[i].x;
            p2.y = points[i].y;
        }

        double dx = p1.x + p2.x;
        double dy = p2.y - p1.y;
        area += (dx * dy)/2;
    }
    return fabs(area);
}

int main (int argc, String argv[]) {
    printf("How many points are on your polygon? ");
    int amnt;
    scanf("%d", &amnt);
    if (amnt >= 3){
        point points[amnt];
        printf("\nEnter your points, clockwise or counterclockwise,");
        printf("\none per line, with a space between the x and y coordinates. \n");
        for (int i = 0; i < amnt; i++){
            double xe = 0;
            double ye = 0;
            printf("Coordinate #%d: \n", i+1);
            scanf("%lf %lf", &xe, &ye);
            points[i].x = xe;
            points[i].y = ye;
            //Debug:
            //printf("%f \n", points[i].x);
            //printf("%f \n", points[i].y);
        }
        printf("Perimeter: %.3f \n", perimeter(points, amnt));
        printf("Area: %.3f \n", area(points, amnt));


    } else {
        printf("No you idiot! That's not a shape!");
        return 0;
    }


    return 0;
}
