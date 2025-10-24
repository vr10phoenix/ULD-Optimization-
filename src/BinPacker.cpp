#include "BinPacker.h"
#include "common.h"
#include <iostream>
#include<algorithm>
#include <iomanip>
#include <map>

BinPacker::BinPacker(double uld_width , double uld_height , double uld_depth , FitStrategy strategy)
 : baseULD_(0 , uld_width , uld_height , uld_depth) , 
   nextULDId_(1) , strategy_(strategy){}

void BinPacker::pack(std::vector<Package>& packages) {
  sortPackages(packages);
  ulds_.clear();
  unplacedPackages_.clear();

 for(auto& pkg : packages){
   PlacementInfo bestFit = findBestFit(pkg , true);
   if(bestFit.uldIndex == -1){
     // make new ULD
     ulds_.push_back(createNewULD());
     bestFit = findBestFit(pkg , true);
   }
   if(bestFit.uldIndex != -1 && bestFit.spaceIndex != -1){
     tryPlacePackage(ulds_[bestFit.uldIndex] , pkg , bestFit);
   }
   else unplacedpackages_.push_back(pkg); // cant be placed 
 }
}


PlacementInfo BinPacker:: findBestFit(cosnt Package& pkg , bool preferPriorityULDs){
 PlacementInfo bestFit;
 bestFit.uldIndex = -1;
 bestFit.spaceIndex = -1;
 bestFit.orientation = -1;
 bestFit.score = std::numeric_limits<double>::max();
 bestFit.placed = false;

 // First pass : ULD with priority packages is preferred
  for(size_t uld_idx = 0;uld_idx < ulds_.size();uld_idx++){
    if(preferPriorityULDs && !ulds_[uld_idx].HasPriorityPackages()) continue;
    for(size_t space_idx = 0; space_idx < ulds_[uld_idx].emptySpaces.size();space_idx++){
      const auto& space = ulds_[uld_idx].emptySpaces[space_idx];
      for(int orientation = 0;orientation < NUM_ORIENTATIONS;orientation++){
        double score = calculateFitScore(space , pkg , orientation);
        if(score < bestFit.score){
          bestFit.uldIndex = uld_idx;
          bestFit.spaceIndex = space_idx;
          bestFit.orientation = orientation;
          bestFit.score = score;
          bestFit.x = space.x;
          bestFit.y = space.y;
          bestFit.z = space.z;
          bestFit.placed = true;
        }
      }
    }
  }
  if(bestFit.uldIndex == -1 && preferPriorityULDs) return findBestFit(pkg , true);
  return bestFit;
}


bool BinPacker::tryPlacePackage(ULD& uld , const Package& pkg , cosnt PlacementInfo& placement){
  if(placement.spaceIndex >= uld.emptySpaces.size()) return false;
  EmptySpaces usedSpace = uld.emptySpaces[placement.spaceIndex];
  uld.emptySpaces.erase(uld.emptySpaces.begin() + Placement.spaceIndex);

  //get pkg dimensions in given orientation
  double p_width , p_height , p_depth;
  pkg.getDimensionsInOrientation(placement.orientation , p_width , p_height , p_depth);
  uld.addPackage(pkg , placement.x , placement.y , placement.z , p_width , p_height , p_depth);
  splitRemainingSpace(uld ,usedSpace, pkg , placement.orientation);
  uld.removeRedundantSpaces();
  return true;
  
}


void BinPacker::splitRemainingSpace(ULD& uld , const EmptySpace& usedSpace , const Package& pkg , int orientation){
  double p_width , double p_height , double p_depth;
  pkg.getDimensionsInOrentation(orientation , p_width , p_height , p_depth);

  // Cerate new spaces from remaining volume , mostly space above the package
  if(p_height < usedSpace.height - EPSILON){
   uld.emptySpaces.push_back(EmptySpace(
     usedSpace.x , 
     usedSpace.y + p_height, 
     usedSpace.z , 
     usedSpace.width , 
     usedSpace.height - p_height,
     usedSpace.depth
     ));
  }
  //space in front
  if(p_depth < usedSpace.depth - EPSILON){
     uld.emptySpaces.push_back(EmptySpace(
     usedSpace.x , 
     usedSpace.y, 
     usedSpace.z + p_depth, 
     usedSpace.width , 
     usedSpace.height,
     usedSpace.depth - p_depth
       ));
  }
  //space in sides 
  if( p_width < usedSpace.width - EPSILON){
     uld.emptySpaces.push_back(EmptySpace(
     usedSpace.x + p_width, 
     usedSpace.y , 
     usedSpace.z , 
     usedSpace.width - p_width, 
     usedSpace.height,
     usedSpace.depth
       ));
  }

}


double BinPacker::CalculateFitScore(const EmptySpace& space , const Package& pkg, int orientation)const{
  double p_width , p_height , p_depth;
  pkg.getDimensionsInOrientation(orientation , p_width , p_height , p_width);
  if(!space.canHold(p_width , p_height , p_depth)) return std::numeric_limits<double>::max();
  double remainingVolume = space.volume() - (p_height * p_height * p_depth);
  double dimensionDiff = (space.width - p_width) + 
                         (space.height - p_height) + 
                         (space.depth - p_depth);
  switch(strategy_){
    case FitStrategy::BEST_VOLUME_FIT:
      return remaininVolume;
    case FitStrategy::BEST_SIDE_FIT:
      return dimensionDiff;
    case FitStrategy::COMBINED_FIT:
    default: 
     return remainingVolume * 0.7 + dimensionDiff * 0.3;
  }
}


void BinPacker::sortPackages(std::vector<Packages>& Packages)const{
std:: sort(Packages.begin() , Packages.end());
}


ULD BinPacker::createNewULD(){
 ULD newULD(nextULDId_++ , baseULD_.width , baseULD.height , baseULD_.depth);
  return newULD;
}


const std::vector<ULD>& BinPacker::getULDs() const{
  return ulds_;
}


double BinPacker::calculateTotalCost() const{
  double cost = 0.0;
  //delay cost for unloaded Packages
  cost += unplacedPackages_.size() * DELAY_COST;
  // K cost for spreading Priority Packages in Multiple ULDs
  int uldsWithPriority = 0;
  for(cosnt auto& uld : ulds_){
  if(uld.hadPriorityPackages()) uldsWithPriority++:
  }
  cost += uldsWithPriority * K_COST;
  return cost;
}


std::vector<PackagePlacement> BinPacker::getAllPlacements(const std::vector<Package>& originalPackages) const {
    std::vector<PackagePlacement> allPlacements;
    
    // First, collect all placed packages from ULDs
    for (const auto& uld : ulds_) {
        auto uldPlacements = uld.getPlacements();
        allPlacements.insert(allPlacements.end(), uldPlacements.begin(), uldPlacements.end());
    }
    
    // Now, create placements for unplaced packages (with uldId = -1)
    for (const auto& pkg : originalPackages) {
        bool placed = false;
        for (const auto& placement : allPlacements) {
            if (placement.packageId == pkg.id) {
                placed = true;
                break;
            }
        }
        
        if (!placed) {
            PackagePlacement unplaced;
            unplaced.packageId = pkg.id;
            unplaced.uldId = -1; // NONE
            unplaced.x = 0;
            unplaced.y = 0;
            unplaced.z = 0;
            unplaced.width = pkg.width;
            unplaced.height = pkg.height;
            unplaced.depth = pkg.depth;
            unplaced.placed = false;
            allPlacements.push_back(unplaced);
        }
    }
    
    return allPlacements;
}



void BinPacker::printSolution(const std::vector<Package>& originalPackages) const {
    auto allPlacements = getAllPlacements(originalPackages);
    
    std::cout << "=== PACKAGE PLACEMENT SOLUTION ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    // Print package placements
    for (const auto& placement : allPlacements) {
        std::cout << "Package " << placement.packageId << ": ";
        if (placement.uldId == -1) {
            std::cout << "NONE";
        } else {
            std::cout << "ULD" << placement.uldId << " at ("
                      << placement.x << ", " << placement.y << ", " << placement.z << ")";
        }
        std::cout << std::endl;
    }
    
    // Print cost summary
    double totalCost = calculateTotalCost();
    int unplacedCount = unplacedPackages_.size();
    int uldsWithPriority = 0;
    for (const auto& uld : ulds_) {
        if (uld.hasPriorityPackages()) {
            uldsWithPriority++;
        }
    }
    
    std::cout << "\n=== COST BREAKDOWN ===" << std::endl;
    std::cout << "Unplaced packages: " << unplacedCount << " x " << DELAY_COST 
              << " = " << (unplacedCount * DELAY_COST) << std::endl;
    std::cout << "ULDs with priority packages: " << uldsWithPriority << " x " << K_COST
              << " = " << (uldsWithPriority * K_COST) << std::endl;
    std::cout << "TOTAL COST: " << totalCost << std::endl;
    
    // Print ULD utilization summary
    std::cout << "\n=== ULD UTILIZATION ===" << std::endl;
    for (const auto& uld : ulds_) {
        std::cout << "ULD" << uld.id << ": " << (uld.utilization() * 100) 
                  << "% utilized, " << uld.packages.size() << " packages, "
                  << uld.priorityPackageCount() << " priority" << std::endl;
    }
}


void BinPacker::printResults() const {
    // Legacy method, now uses the new format
    std::vector<Package> empty;
    printSolution(empty);
}


void BinPacker::setStrategy(FitStrategy newStrategy) {
    strategy_ = newStrategy;
}


