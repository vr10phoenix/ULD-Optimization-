#ifndef PACKAGE_H
#define PACKAGE_H
class Package{
public : 
  int id;
  double width , height , depth; 
  bool isPriority;

 Package(int p_id , double w , double h , double d , bool priority = false);

 double volume() const;
 void getDimensionsInOrientation( int orientation , double& out_width , 
                                  double out_height , double& out_depth) const;

 bool operator< (const Package& other) const;
};

#endif
