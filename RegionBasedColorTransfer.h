
#ifndef REGION_BASED_COLOR_TRANSFER_H
#define REGION_BASED_COLOR_TRANSFER_H

#include "cv.h"
#include "highgui.h"
#include "Segmentation.h"
#include <vector>

using namespace std;
using namespace cv;


class RegionBasedColorTransfer
{
private:

  //constant values
  const int number_of_color_type = 9;
  vector<Vec3f> colorValues;

  Mat source, target;
  vector<vector<Pixel> > sourceBuffer;
  vector<vector<Pixel> > targetBuffer;
  // vector<vector<Pixel> > sourceClassification(number_of_color_type);
  //vector<vector<Pixel> > targetClassification(number_of_color_type);
  vector<vector<Pixel> > sourceClassification;
  vector<vector<Pixel> > targetClassification;


  //helper function

  //classify color into different category
  int classifyColor(const Mat &image, Pixel position) const;

  //find the min color Eclidean basic color
  int minColorDistance(const Vec3f & value) const;



public:
  RegionBasedColorTransfer(char* sourceFile, char* targetFile);


  void segmentImages(vector<vector<Pixel> >&sourceBuffer, vector<vector<Pixel> >&targetBuffer)const;
  void classifyColor();
  void globallyTransferColor();
  //void correlateSourceAndTargetRegions();
  //void colorTransferCorrespondently();
  //void textureTransferCorrespondently();
  void process();
  void showTarget() const;
  void showSource() const;
};

#endif
