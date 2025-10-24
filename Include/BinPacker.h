#ifndef BIN_PACKER_H
#define BIN_PACKER_H

#include "ULD.h"
#include "Package.h"
#include "EmptySpace.h"
#include "common.h"
#include<vector>
#include<string>

class BinPacker{
public: 
 BinPacker(double uld_width , double uld_height , double uld_depth , 
           FitStrategy strategy = FitStrategy::COMBINED_FIT);

 void Pack(std::vector<Packages>& packages);
 const std::<ULD>& getULDs() const;
 void printResults() const;
 void setStrategy(FitStrategy newStrategy);

 void printSolution(const std::vector<Package>& originalPackages) const;
 double calculateTotalCost() const;
std::vector<PackagePlacement> getAllPlacements(const std::vector<Package>& originalPackages)const;

private: 
 std::vector<ULD> ulds_;
 ULD baseULD_;
 int nextULDId_;
 FitStrategy strategy_;
 std::vector<Package> unplacedPackages_;

 PlacementInfo findBestFit(const Package& pkg , bool preferPriorityULDs);
 bool tryPlacePackage(ULD& uld , const Package& pkg , const PlacementInfo& placement);
 void splitRemainingSpace(ULD& uld , const EmptySpace& usedSpace , 
                          const Package& pkg , int orentation);
 double calculateFitScore(const EmptySpace& space , const Package& pkg , 
                          int orentation)const;
 void sortPackages(std::vector<Package>& packages) const;
 ULD createNewULD();
};

#endif
 

