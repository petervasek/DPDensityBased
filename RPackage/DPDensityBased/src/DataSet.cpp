#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <memory>
#include <omp.h>
#include <regex>
#include "DataSet.h"
#include "Cluster.h"
#include "Cell.h"
#include "Level.h"

#include <Rcpp.h>
using namespace Rcpp;

DataSet::DataSet() = default;;

DataSet::~DataSet(){
  inputPoints.clear();
  delete[] coordBordersMax;
  delete[] coordBordersMin;
};

void DataSet::loadInputPoints(std::string fileName) {

  std::ifstream file;
  file.open(fileName);

  if (file.is_open()) {
    std::string firstLine;
    getline(file, firstLine);
    if(!datasetHasHeader(firstLine)){
      file.seekg(0);
    }
    getline(file, firstLine);
    setNumOfDimensions(findOutNumOfDimensions(firstLine));
    file.seekg(0);
    getline(file, firstLine);
    if(!datasetHasHeader(firstLine)){
      file.seekg(0);
    }

#ifdef DELETE_INPUTTED
    coordBordersMax = new double[this->getNumOfDimensions()];
    coordBordersMin = new double[this->getNumOfDimensions()];
#endif

#ifndef LOAD_CLUSTERING_RESULT
    coordBordersMax = new double[this->getNumOfDimensions()];
    coordBordersMin = new double[this->getNumOfDimensions()];

    //we need limit values in every dimension for future calculations
    for (int i = 0; i < this->getNumOfDimensions(); i++) {
      coordBordersMax[i] = std::numeric_limits<double>::min();
      coordBordersMin[i] = std::numeric_limits<double>::max();
    }
#endif
    while (!file.eof()) {
      std::shared_ptr<Point> p = std::make_shared<Point>(getNumOfDimensions());

      int i;
      for (i = 0; i < this->getNumOfDimensions(); i++) {
        file >> p->coord[i];
      }
      //#ifdef OPENMP
      //    #pragma omp parallel for schedule(static) private (i) shared(inputPoints, pointIndex, bordersMax, bordersMin)
      //#endif*/
      for (i = 0; i < this->getNumOfDimensions(); i++) {
        double coordValue = p->coord[i];
        if (coordValue > coordBordersMax[i])
          coordBordersMax[i] = coordValue;
        if (coordValue < coordBordersMin[i])
          coordBordersMin[i] = coordValue;
      }
      p->st = Point::unvisited;
      p->gr = Point::undecided;
      p->clusterIndex = -1;
      p->corePoint = false;

      inputPoints.emplace_back(p);
    }
  }
  file.close();
}

void DataSet::printAllClusters(std::vector<Cluster *> clusters) {
  for (int i = 0; i < clusters.size(); i++) {
    clusters.at(i)->printCluster(getNumOfDimensions());
  }
}

void DataSet::printNoise(std::vector<Point> &input) {

  int count = 0;

  std::cout << "\nNoise:\n";
  for (int i = 0; i < (int) input.size(); i++) {
    if (input[i].gr == Point::noise) {
      count++;
      for (int j = 0; j < this->getNumOfDimensions(); j++)
        std::cout << input[i].coord[j] << " ";
      std::cout << "\n";
    }
  }
  if (count == 0)
    std::cout << "no noise\n";
}

bool DataSet::datasetHasHeader(std::string firstLine) {
  std::regex unwantedChars("[\"a-zA-Z]");
  return std::regex_search(firstLine, unwantedChars);
}

int DataSet::findOutNumOfDimensions(std::string firstLine) {

  //reduce spaces if needed
  std::regex re("\\s{2,}");
  firstLine = regex_replace(firstLine, re, " ");

  //trim
  re = ("^[ ]*");
  firstLine = regex_replace(firstLine, re, "");
  re = ("[ ]*$");
  firstLine = regex_replace(firstLine, re, "");

  std::istringstream line(firstLine);
  std::string s;
  int counter = 0;

  while(line) {
    std::getline(line, s, ' ');
    counter++;
  }

  return counter-1;
}

std::string DataSet::getOutputName(std::string purpose, std::string inputFile, double eps, int minPts){
  std::string outputName = inputFile;
  outputName = outputName.substr(0,outputName.length()-4);
  outputName += "_" + purpose + "_";
  outputName += std::to_string(minPts);
  outputName += "_";
  outputName += std::to_string(eps);
  outputName += ".txt";

  std::regex e ("Sources");
  outputName = std::regex_replace (outputName,e,"Results");

  return outputName;
}

int DataSet::getNumOfDimensions() {
  return numOfDimensions;
}

void DataSet::setNumOfDimensions(int num) {
  numOfDimensions = num;
}

std::vector<std::shared_ptr<Point>> &DataSet::getInputPoints() {
  return inputPoints;
}

double* DataSet::getCoordBordersMax() {
  return coordBordersMax;
}

double* DataSet::getCoordBordersMin() {
  return coordBordersMin;
}
