#ifndef EMPTY_SPACE_H
#define EMPTY_SPACE_H

class EmptySpace{
public : 
 double x , y , z;
 double width , height , depth;

 EmptySpace(double x_pos , double y_pos , double z_pos , 
            double w , double h  ,double d);
 
 double volume() const;
 bool canHold(double p_Width , double p_height , double p_depth) const;
 bool contains(const EmptySpace& other) const;
 bool intersects(const EmptySpaces& other) const;

 bool operator< (const EmptySpaces& other) const;
};

#endif
