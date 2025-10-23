#include "ULD.h"
#include "common.h"
#include <algorithm>

ULD::ULD(int uld_id, double w, double h, double d)
    : id(uld_id), width(w), height(h), depth(d) {
    initializeEmptySpaces();
}

void ULD::initializeEmptySpaces() {
    emptySpaces.clear();
    emptySpaces.push_back(EmptySpace(0, 0, 0, width, height, depth));
}

bool ULD::hasPriorityPackages() const {
    for (const auto& pkg : packages) {
        if (pkg.isPriority) return true;
    }
    return false;
}

double ULD::usedVolume() const {
    double total = 0.0;
    for (const auto& pkg : packages) {
        total += pkg.volume();
    }
    return total;
}

double ULD::totalVolume() const {
    return width * height * depth;
}

double ULD::utilization() const {
    return usedVolume() / totalVolume();
}

int ULD::priorityPackageCount() const {
    int count = 0;
    for (const auto& pkg : packages) {
        if (pkg.isPriority) count++;
    }
    return count;
}

void ULD::addPackage(const Package& pkg, double x, double y, double z,
                    double p_width, double p_height, double p_depth) {
    packages.push_back(pkg);
    
    PackagePlacement placement;
    placement.packageId = pkg.id;
    placement.uldId = id;
    placement.x = x;
    placement.y = y;
    placement.z = z;
    placement.width = p_width;
    placement.height = p_height;
    placement.depth = p_depth;
    placement.placed = true;
    
    placements.push_back(placement);
}

void ULD::removeRedundantSpaces(){
  for(auto itr1 = emptySpaces.begin();itr1!=emptySpaces.end();){
    bool redundant = false;
    for(auto itr2 = emptySpaces.begin();itr2 != emptySpaces.end();itr2++){
      if(itr1 != itr2 && itr2->conatins(*itr1)){
        redundant = true;
        break;
      }
    }
    if(redundant){
     itr1 = emptySpaces.erase(itr1);
    }else{
        itr1++;
    }
  }
}

std::vector<PackagePlacement>ULD::getPlacements()const{
return placements;
}
