#indef COMMON_H
#define COMMON_H

#inlcude <vector>
#include<algorithm>
#inlcude<cmath>
#inlcude<limits>
#include<string>

// requireed Constants : 
const double EPSILON = 1e-9;
const int NUM_ORIENTATIONS = 6;
const double K_COST = 1000.0; // let shipment price for priority packages be 1000.0
const double DELAY_COST  = 500.0; // delay Cost of any package 

enum class FitStrategy{
 BEST_VOLUME_FIT,
 BEST_SIDE_FIT, 
COMBINED_FIT
};

struct PlacementInfo{
int uldIndex ;
int spaceIndex;
int orientation;
double score;
double x ,y , z;
bool placed;
};

struct PackagePlacement{
int packageId;
int uldId;
double x , y , z;
double width , height , depth;
bool placed;
};

#endif


