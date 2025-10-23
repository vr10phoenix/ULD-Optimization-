#include "EmptySpace.h"
#include "common.h"
#include <algorithm>
using namespace std;

EmptySpace :: EmptySpace(double x_pos , double y_pos , double z_pos , 
                         double w , double h , double d)
  : x(x_pos) , y(y_pos) , z(z_pos) , width(w) , height(h) , depth(d) {}

double EmptySpace :: volume() const{
  return width * height * depth;
}

bool EmptySpace :: canHold(double p_width , double p_height , double p_depth) const {
  return (p_width <= width + EPSILON && 
          p_height <= height +EPSILON && 
          p_depth <= depth + EPSILON );
}

bool EmptySpace :: contains(const EmptySpace& other) const {
  return (other.x >= x - EPSILON && 
          other.y >= y - EPSILON && 
          other.z >= z - EPSILON && 
          other.x + other.width <= x + width + EPSILON && 
          other.y + other.height <= y + height + EPSILON && 
          other.z + other.depth <= z + depth + EPSILON );
}

bool EmptySpace::operator<(const EmptySpace& other) const{
  if(abs(volume() - other.volume()) > EPSILON){
return volume() < other.volume();
     }
  if(abs(x - other.x) > EPSILON) return x < other.x;
  if(abs(y - other.y) > EPSILON) return y < other.y;
 return z < other.z;
}

bool EmptySpace :: intersects(const EmptySpace& other) const {
  return ! (x + width < other.x || other.x + other.width <= x || 
            y + height < other.y || other.y + other.height <= y || 
            z + depth < other.z || other.z + other.depth <= z);
}



