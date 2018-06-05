#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <math.h>
#include <algorithm>
#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

double pi = 3.141592653589793238462643383279502884197169;


int signed_area2D(point2D a, point2D b, point2D c) {
  int result =  0;
  result = ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
  return result;
}
/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
  int result = 0;
  result = p.x * (q.y - r.y) + q.x * (r.y - p.y) + r.x * (p.y - q.y);
  if (result != 0){
    return 0;
  }
  else {
    return 1;
  }
}

/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2D a, point2D b, point2D c) {
  int result = 0;
  result = signed_area2D(a, b, c);
  if(result > 0){
    return 1;
  }
  else{
    return 0;
  }
}


int right_strictly(point2D a, point2D b, point2D c){
  int result = 0;
  result = signed_area2D(a, b, c);
  if(result < 0){
    return 1;
  }
  else{
    return 0;
  }
}

/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2D a, point2D b, point2D c) {
  int left = 0;
  left = left_strictly(a, b, c);
  int on = 0;
  on = collinear(a, b, c);

  if(left == 1 || on == 1){
    return 1;
  }
  else{
    return 0;
  }
}

/* **************************************** */
/*this function returns the distance between two points
/Parametrs two point 2D structs.
/return value: a double which is the distnace*/
/******************************************/
double distance(point2D a, point2DD b){
  double result = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
  return result;
}

/* **************************************** */
/*checks the between ness between three points taken from the book
 return value is whether points are between each other */
/* **************************************** */
bool Between(point2D a, point2D b, point2D c){
  if(!collinear(a, b, c)){
    return false;
  }

  if(a.x != b.x){
    return((a.x <= c.x) && (c.x <= b.x)) ||
    ((a.x >= c.x) && (c.x >= b.x));
  }
  else{
    return ((a.y <= c.y) && (c.y <= b.y)) || ((a.y >= c.y) && (c.y >= b.y));
  }
}

/* **************************************** */
/* Taken from the book dont really know what it does */
/* **************************************** */
void Assigndi(point2DD *p, point2D a){
  p->x = a.x;
  p->y = a.y;
}

/* **************************************** */
/* This functions returns whether the point of intersection is collinear
with any ot the segments in the polygon
*/
char ParallelInt(point2D a, point2D b, point2D c, point2D d, point2DD *p){
  if (!collinear(a, b, c)){
    return '0';
  }
  if(Between(a, b, c)){
    Assigndi(p, c);
    return 'e';
  }
  if(Between(a, b, d)){
    Assigndi(p, d);
    return 'e';
  }
  if(Between(c, d, a)){
    Assigndi(p, a);
    return 'e';
  }
  if(Between(c, d, b)){
    Assigndi(p, b);
    return 'e';
  }
  return '0';
}

/* **************************************** */
/*This function checks to see if there are any intersections
between segments and returns the type of intersection and the points
of intersection */
/* **************************************** */
char seg_Intersect(point2D a, point2D b, point2D c, point2D d, point2DD *p){
  double s, t;
  double num, denom;
  char code = '?';

  denom = (a.x * (double)(d.y - c.y)) + (b.x * (double)(c.y - d.y)) +
  (d.x * (double)(b.y - a.y)) + (c.x * (double)(a.y - b.y));


  if(denom == 0.0){
    return ParallelInt(a, b, c, d,p);
  }

  num = (a.x * (double)(d.y - c.y)) + (c.x * (double)(a.y - d.y)) + (d.x * (double)(c.y-a.y));
  if((num == 0.0) || (num == denom)){
    code = 'v';
  }
  s = num/denom;

  num = -((a.x * (double)(c.y - b.y)) + (b.x * (double)(a.y - c.y)) + (c.x * (double)(b.y-a.y)));
  if((num == 0.0) || (num == denom)){
    code = 'v';
  }
  t = num/denom;

  
  if((0.0 < s) && (s <1.0) && (0.0 < t) && (t <1.0)){
    code = 'l';
  }
  else if((0.0 > s) || (s > 1.0) || (0.0 > t) || (t>1.0)){
    code = '0';
  }
  p->x = a.x + (s * (b.x - a.x));
  p->y = a.y + (s * (b.y - a.y));

  return code;

}

/* **************************************** */
/*goo through all edges of polygon if the point of intersection is a porper
intersection it return that the function isnt visible if ther are no proper intersections
return true*/
bool isVisible(point2D guard, point2D p1, vector<Segment> segments){
  vector<Segment> edges;
  char type;
  point2DD *point = new point2DD;
  int i = 0;
  //check all segments
  while(i < segments.size()){
    type = seg_Intersect(guard, p1, segments[i].start , segments[i].end, point);
    if(type == 'l'){
      return false;
    }
    i++;
  }
  return true;
}

/* **************************************** */
//This function takes two points and calculates the
//slope between two points. It then calculates a 
//extended point which is used with the original point to extend a line to the edge of 
//the polygon
/* **************************************** */
point2D slope(int x1,int y1, int x2,int y2){
   float slope;
   float dx, dy;
   dy = y2- y1;
   dx = x2 - x1;
   point2D extraPoint;

   slope=(dy/dx);
   //s=atan(tanx);
   //s=(180/pi)*s;
   float intercept = y1 - (slope * x1);
   if(x1 < x2){
    extraPoint.y = (slope * 1000) + intercept;
    extraPoint.x = 1000;
   }
   else if(x1 > x2){
    extraPoint.y = (slope * -1000) + intercept;
    extraPoint.x = -1000;
   }
   else if(x1 == x2){
    if(y1 > y2){
      extraPoint.x = (-1000 - intercept)/slope;
      extraPoint.y = -1000;
    }
    if(y1 < y2){
      extraPoint.x = (1000 - intercept)/slope;
      extraPoint.y = 1000;
    }
   }
   return extraPoint;

}

//Is a boolean to see if I have encountered a point to which I need to extend
// a ray that will go to the end of the polygon.
bool needToExtend(point2D guard, point2D curr, point2D next, point2D prev){
  //if point after and before are left of
  if(left_strictly(guard, curr, next) && left_strictly(guard, curr, prev)){
    return true;
  }
  //if point after and before are rightof.
  if(right_strictly(guard, curr, next) && right_strictly(guard, curr, prev)){
    return true;
  }
  return false;
}

//Purpose: Adds the intersects that the ray encounters to the vector of intersects
//located in the segment struct:
//parameters, guard, the point you are inspecting and the array of segments.
vector<Segment> returnIntersects(point2D guard, point2D p1, vector<Segment> segments){
  char type;
  //point of intersection
  point2DD *point = new point2DD;
  int size = segments.size();
  //point to be added to vector
  point2D *intersect = new point2D;
  for(int i = 0; i < segments.size(); i ++){
    type = seg_Intersect(guard, p1, segments[i].start, segments[i].end, point);
    //if proper intersection add point to array.
    if(type == 'l'){
      intersect->x = point->x;
      intersect->y = point->y;
      segments[i].ray->push_back(*intersect);
    }
  }
  //return modified array
  return segments;
}


/*************************************/
// This function uses the is visible function to go through all the points in the 
//polygon and check to see if that point is visible from the guard.
//change to insert at correct index.
vector<Segment> findVisible(vector<Segment> segments, point2D guard){
  point2D extended;
  int size = segments.size();
  //for all segments of the polygon
  for(int i = 0; i < segments.size(); i++){
    //if the start of the segment is visible
    if(isVisible(guard, segments[i].start, segments)){
      //if you are at the start of the array you have to check the last point and the next point
      if(i == 0){
        //check to see if you need to extend
        if(needToExtend(guard, segments[i].start, segments[i].end, segments[size-1].start)){
          //create extended point
          extended = slope(guard.x, guard.y, segments[i].start.x, segments[i].start.y);
          //modify segment struct
          returnIntersects(guard, extended, segments);
        }
      }
      else{
        //check to see if you need to extend
        if(needToExtend(guard, segments[i].start, segments[i].end, segments[i -1].start)){
          //create extended point
          extended = slope(guard.x, guard.y, segments[i].start.x, segments[i].start.y);
          //modify segment struct
          returnIntersects(guard, extended, segments);
        }
      }
      //check to see if end of segment is visible
      if(isVisible(guard, segments[i].end, segments)){
        //whole segment is visible
        segments[i].evisible = 1;
        segments[i].svisible = 1;
      } 
      //if segment start is not visible continue
      else{
        segments[i].svisible = 1;
      }
    }
  }
  return segments;
}


