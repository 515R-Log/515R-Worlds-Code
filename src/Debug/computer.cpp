#include "main.h"
/*-----------------------------------------------------------------------------
   _____                                 _                   _____         _                   
  / ____|                               | |                 |  __ \       | |                  
 | |      ___   _ __ ___   _ __   _   _ | |_  ___  _ __     | |  | |  ___ | |__   _   _   __ _ 
 | |     / _ \ | '_ ` _ \ | '_ \ | | | || __|/ _ \| '__|    | |  | | / _ \| '_ \ | | | | / _` |
 | |____| (_) || | | | | || |_) || |_| || |_|  __/| |       | |__| ||  __/| |_) || |_| || (_| |
  \_____|\___/ |_| |_| |_|| .__/  \__,_| \__|\___||_|       |_____/  \___||_.__/  \__,_| \__, |
                          | |                                                             __/ |
Created on 5/20/2022      |_|                                                            |___/ 
Last Updated on 3/15/2022

Prints to the terminal in a format which is readable by custom-made 
External Sensor Count: 9
Total Sensor Count: 13

-----------------------------------------------------------------------------*/

// ---------------- ROUND ---------------- //
// To
float round(double value, int decimal_place){
  int scale = std::pow(10,decimal_place);
  value=std::round(value*scale)/scale;
  return value;
}

// ---------------- PRINT TO TERMINAL ---------------- //
// Prints to terminal based on a given set of values and precision
void printToGraph(float x, std::vector<float> ys, int precision){
  // Create an effience vector that uses floats
  std::vector<float> compYs;

  // Loop through each provided double value in 'ys'
  for(int i=0; i<ys.size(); i++)
    // Round ys value at the index 'i' and add it to the efficient vector
    compYs.push_back(round(ys.at(i),precision));

  // Fill the remainder of the 'ys' if it is empty
  while(compYs.size()<5)
    compYs.push_back(0);

  // Print the vector list of 'ys' to the terminal
  printf("%f,%f,%f,%f,%f,%f\n", x, compYs.at(0), compYs.at(1), compYs.at(2), compYs.at(3), compYs.at(4));
}

// ---------------- PRINT TO GRAPH ADVANCED ---------------- //
//
void printToGraph(int time, int timeIntervals, float x, std::vector<float> ys, int precision){
  if(time%timeIntervals==0)
  printToGraph(x, ys, precision);
}
