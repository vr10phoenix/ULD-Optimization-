#ifndef ULD_H
#define ULD_H

#include "Package.h"
#include "EmptySpace.h"
#include <vector>
#include <string>

class ULD {
public : 
 int id;
 double width , height , depth ;
 std::vector<Package> packages;
 std::vector<EmptySpaces> emptySpaces;
 std::vector<PackagePlacement> placements; 

 ULD(int id , double w , double h , double d);

 bool hasPriorityPackages() const;
 double usedVolume() const;
 double totalVolume() const;
 double utilization() const;
 int priorityPackageCount() const;

 void addPackage ( const Package& pkg , double x , double y , double z , 
                double p_width , double p_height , double p_depth );
 void removeRedundantSpaces();

 std::vector<PackagePlacement> getPlacements() const;
 int getId() const {return id;}

private: 
 void intitalizeEmptySpaces();
};

#endif
