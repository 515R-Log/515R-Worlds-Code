#pragma once
#include <vector>
/** @file Debug/computer.hpp
  * @brief Export to Terminal Display
  * 
  * Sends input information to a data file
  * Data file is later processed by python programs that plots data on a graph/field
  * 
  * Added features printing in time intervals and rounding values
  * 
  * @author Logan Daugherty
*/

/** @brief Basic Print to Graph
  * 
  * Combines input information and printing data to terminal
  * 
  * @param x
  *        X Axis of Graph
  * @param ys
  *        Y Axis for each line of the graph
  * @param precision
  *        Rounds the graph numbers to a certain degree
  * 
*/
void printToGraph(float x, std::vector<float> ys, int precision);

/** @brief Alt. Print to Graph
  * 
  * Combines input information and printing data to terminal
  * This terminal information is later 
  * 
*/
void printToGraph(int time, int timeIntervals, float x, std::vector<float> ys, int precision=1);
