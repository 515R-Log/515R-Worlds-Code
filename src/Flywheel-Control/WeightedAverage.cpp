#include "Flywheel-Control/WeightedAverage.hpp"
/*-----------------------------------------------------------------------------
 __          __  _       _     _           _
 \ \        / / (_)     | |   | |         | |        /\
  \ \  /\  / /__ _  __ _| |__ | |_ ___  __| |       /  \__   _____ _ __ __ _  __ _  ___
   \ \/  \/ / _ \ |/ _` | '_ \| __/ _ \/ _` |      / /\ \ \ / / _ \ '__/ _` |/ _` |/ _ \
    \  /\  /  __/ | (_| | | | | ||  __/ (_| |     / ____ \ V /  __/ | | (_| | (_| |  __/
     \/  \/ \___|_|\__, |_| |_|\__\___|\__,_|    /_/    \_\_/ \___|_|  \__,_|\__, |\___|
                    __/ |                                                     __/ |
                   |___/                                                     |___/

Created on 2/20/2022
Last Updated on 2/26/2023

Source file for neat data manipulation and moving average shenanigans.

-----------------------------------------------------------------------------*/

// ---------------- WEIGHTED AVERAGE ---------------- //
// Base initialization for the class
// Defines the size of sampling for weighted average
WeightedAverage::WeightedAverage(const int size){
  setSize(size);
}

// ---------------- SET SIZE ---------------- //
// Sets the sampling size of the class
// Removes consideration of any value below 1
void WeightedAverage::setSize(int size){
  // Set a minimum size of 1
  if(size<1)
    size=1;

  list.resize(size); // Resize set
}

int WeightedAverage::getSize(){
  return list.size();
}

// ---------------- ADD VALUE ---------------- //
// Adds a value to the end of the set and removes the first
// Maintains the same size of the set
void WeightedAverage::addValue(const double value){
  // list.insert(list.begin(), value);
  // list.pop_back();
  list.push_back(value); // Add value to that back
  list.erase(list.begin());
}
 //dfs
//Test

// ---------------- GET WEIGHTED AVERAGE ---------------- //
// Uses the Weighted average formula to find the best average
// Newer values have a greater effect on the end value than older values
double WeightedAverage::getWeightedAverage(){
  double result=0; // Delcare resulting value

  // Summate all of the values 
  for(int i=0; i<list.size(); i++)
    result+=list.at(i)*(i+1);

  // Summate all of the indexes
  int miniSum = 0;
  for(int i=0; i<list.size(); i++)
    miniSum+=i+1;

  result/=miniSum; // Devided the values by the indexes

  return result; // Return the resulting value
}
