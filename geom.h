#ifndef __geom_h
#define __geom_h

#include <vector>

using namespace std;

//change to doubles
typedef struct _point2D {
  int x,y;
  double angle;
  int distance;
  char type;
  int visible;
  int original;
} point2D;

typedef struct _point2DD {
  double x;
  double y;
  double angle;
  int distance;
  char type;
} point2DD; 

typedef struct _Ray{
  point2D start;
  point2D end;
} Ray;

typedef struct _Segment{
  point2D start;
  point2D end;
  vector<point2D> *ray;
  int svisible;
  int evisible;
}Segment;


/* returns 2 times the signed area of triangle abc. The area is
   positive if c is to the left of ab, 0 if a,b,c are collinear and
   negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c);


/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r);


/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly (point2D a, point2D b, point2D c);


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2D a, point2D b, point2D c);

double distance(point2D a, point2DD b);
bool angleCompare(point2D a, point2D b);
vector<Segment> findVisible(vector<Segment> segments, point2D querry);

#endif
