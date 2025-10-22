#inlcude "Package.h"
#inlcude <algorithm>
using namespace std;

 Package :: Package(int p_id , double w , double h , double d , bool priority):
  id(p_id) , width(w) , height(h) , depth(d) , isPriority(priority) {} 

 double Package :: volume() const {
   return width * height * depth;}

 void Package :: getDimensionsInOrientation(int orientation , double& out_width , 
                                           double& out_height , double out_depth)const{
   switch(orientation){
     case 0 : out_width = width; out_height = height; out_depth = depth; break;
     case 1 : out_width = width; out_height = depth; out_depth = height; break;
     case 2 : out_width = height; out_height = width; out_depth = depth; break;
     case 3 : out_width = height; out_height = depth; out_depth = width; break;
     case 4 : out_width = depth; out_height = height; out_depth = width; break;
     case 5 : out_widt = depth; out_heigth = width; out_depth = height; break;

     default : out_width = width; out_height = height; out_depth = depth; break;

   }
 }

bool Package :: operator< (const Package& other ) const {
  if(ispriority != other.isPriority){
    return isPriority > other.isPriority;
  }
  if(std :: abs(volume() - other.volume()) > 1e-9){
    return volume() > other.volume();
  }
  // then biggest Dimension 
 double max_dim = max({width , height , depth});
double other_max_dim = max({other.width , other.height , other.depth});

return max_dim > other_max_dim;
}

