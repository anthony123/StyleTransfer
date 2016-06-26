#include "cv.h"
#include "highgui.h"
#include "RegionBasedColorTransfer.h"
#include "Segmentation.h"
#include "imconv.h"
#include "misc.h"
#include <iostream>

using namespace cv;
using namespace std;

void RegionBasedColorTransfer::process()
{
  segmentImages(sourceBuffer,targetBuffer);
  cout<<"segment image done"<<endl;
  classifyColor();
  cout<<"claasify color done\n";
  globallyTransferColor();
  cout<<"globally transfer color done\n";
  //correlateSourceAndTargetRegions();
  //colorTransferCorrespondently();
  //textureTransferCorrespondently();
}

void RegionBasedColorTransfer::globallyTransferColor()
{
  Mat labSource, labTarget;

  cvtColor(source, labSource, CV_BGR2Lab);
  cvtColor(target, labTarget, CV_BGR2Lab);

  //mean color value of each basic color
  vector<double> meanSourceLValues(number_of_color_type);
  vector<double> meanSourceAValues(number_of_color_type);
  vector<double> meanSourceBValues(number_of_color_type);

  vector<double> meanTargetLValues(number_of_color_type);
  vector<double> meanTargetAValues(number_of_color_type);
  vector<double> meanTargetBValues(number_of_color_type);

  //standard deviation of each basic color
  vector<double> SDSourceValues(number_of_color_type);
  vector<double> SDTargetValues(number_of_color_type);

  //mean value for source image
  for(int i = 0; i < number_of_color_type; i++){
    int num = 0;
    double totalL = 0, totalA = 0, totalB = 0;

    for(auto px: sourceClassification[i]){
      //Vec3f value = labSource.at<Vec3f>(px.x, px.y);
      uchar* data = labSource.data + px.x*labSource.step + px.y*labSource.elemSize();
      int value1 = *data;
      int value2 = *(data+1);
      int value3 = *(data+2);
      num++;
      totalL += value1;
      totalA += value2;
      totalB += value3;
    }
    double meanL = totalL/num;
    double meanA = totalA/num;
    double meanB = totalB/num;

    meanSourceLValues[i] = meanL;
    meanSourceAValues[i] = meanA;
    meanSourceBValues[i] = meanB;

  }

  //standard deviation of source image
  for(int i = 0; i < number_of_color_type; i++){
      int num = 0;
      double totalSquared = 0.0;
      for(auto px: sourceClassification[i]){
        //Vec3f value = labSource.at<Vec3f>(px.x, px.y);
        uchar* data = labSource.data + px.x*labSource.step + px.y*labSource.elemSize();
        int value1 = *data;
        int value2 = *(data+1);
        int value3 = *(data+2);
        num++;
        totalSquared += (value1 - meanSourceLValues[i])*(value1 - meanSourceLValues[i]);
        totalSquared += (value2 - meanSourceAValues[i])*(value2 - meanSourceAValues[i]);
        totalSquared += (value3 - meanSourceBValues[i])*(value3 - meanSourceBValues[i]);
      }
      double meanSquared = totalSquared/num;
      SDSourceValues[i] = sqrt(meanSquared);
  }

  //mean value of target image
  for(int i = 0; i < number_of_color_type; i++){
    int num = 0;
    double totalL = 0, totalA = 0, totalB = 0;

    for(auto px :targetClassification[i]){
      //Vec3f value = labTarget.at<Vec3f>(px.x, px.y);
      uchar* data = labTarget.data + px.x*labTarget.step + px.y*labTarget.elemSize();
      int value1 = *data;
      int value2 = *(data+1);
      int value3 = *(data+2);
      num++;
      totalL += value1;
      totalA += value2;
      totalB += value3;
    }
    double meanL = totalL/num;
    double meanA = totalA/num;
    double meanB = totalB/num;

    meanTargetLValues[i] = meanL;
    meanTargetAValues[i] = meanA;
    meanTargetBValues[i] = meanB;

  }


  //standard deviation of target image
  for(int i = 0; i < number_of_color_type; i++){
      int num = 0;
      double totalSquared = 0.0;
      for(auto px: targetClassification[i]){
        //Vec3f value = labTarget.at<Vec3f>(px.x, px.y);
        uchar* data = labTarget.data + px.x*labTarget.step + px.y*labTarget.elemSize();
        int value1 = *data;
        int value2 = *(data+1);
        int value3 = *(data+2);
        num++;
        totalSquared += (value1 - meanTargetLValues[i])*(value1 - meanTargetLValues[i]);
        totalSquared += (value2 - meanTargetAValues[i])*(value2 - meanTargetAValues[i]);
        totalSquared += (value3 - meanTargetBValues[i])*(value3 - meanTargetBValues[i]);
      }
      double meanSquared = totalSquared/num;
      SDTargetValues[i] = sqrt(meanSquared);
  }

  //color transfer
  for(int i = 0; i < number_of_color_type; i++){
    double SDRatio = SDSourceValues[i]/SDTargetValues[i];
    for(auto px: targetClassification[i]){
      //Vec3f value = labTarget.at<Vec3f>(px.x, px.y);
      uchar* data = labTarget.data + px.x*labTarget.step + px.y*labTarget.elemSize();
      int value1 = *data;
      int value2 = *(data+1);
      int value3 = *(data+2);
      float L = SDRatio *(value1 - meanTargetLValues[i]) + meanSourceLValues[i];
      float A = SDRatio *(value2 - meanTargetAValues[i]) + meanSourceAValues[i];
      float B = SDRatio *(value3 - meanTargetBValues[i]) + meanSourceBValues[i];
      //labTarget.at<Vec3f>(px.x, px.y) = Vec3f(L,A,B);
      *data = L;
      *(data+1) = A;
      *(data+2) = B;
    }
  }

  //convet back from lab to bgr
  cvtColor(labTarget, target, CV_Lab2BGR);
}

int RegionBasedColorTransfer::classifyColor(const cv::Mat &image, Pixel position)const
{
  int x = position.x;
  int y = position.y;
  uchar *data = image.data + x*image.step + y*image.elemSize();
  float value1 = *data;
  float value2 = *(data+1);
  float value3 = *(data+2);
  //Vec3f value = image.at<Vec3f>(x,y);
  Vec3f value = Vec3f(value1, value2, value3);
  //cout<<value<<endl;
  int index = minColorDistance(value);

  return index;
}

int RegionBasedColorTransfer::minColorDistance(const Vec3f & value) const
{
  vector<double>dists;
  double min_distance = 100000;
  int min_index = -1;

  for(int i = 0; i < number_of_color_type; i++)
    {

      double dist = norm(value, colorValues[i]);
      //double dist = norm(Vec3f(value1, value2, value3), Vec3f(color1, color2, color3));
      if(dist < min_distance){
	       min_distance = dist;
	        min_index = i;
      }
    }

  //cout<<"min index = "<<min_index<<endl;

  return min_index;
}

RegionBasedColorTransfer::RegionBasedColorTransfer(char* sourceFile, char* targetFile)
{

  //init file
  source = imread(sourceFile,1);
  target = imread(targetFile,1);

  for(int i = 0; i < number_of_color_type; i++)
  {
    vector<Pixel> vec;
    sourceClassification.push_back(vec);
    targetClassification.push_back(vec);

  }

  //sourceClassification.reserve(number_of_color_type);
  //targetClassification.reserve(number_of_color_type);

  //read the nine basic color category
  colorValues.push_back(Vec3f(0,0,0));
  colorValues.push_back(Vec3f(0,0,255));
  colorValues.push_back(Vec3f(0,0,128));
  colorValues.push_back(Vec3f(0,255,255));
  colorValues.push_back(Vec3f(19, 100, 100));
  colorValues.push_back(Vec3f(30,100,100));
  colorValues.push_back(Vec3f(60,100,128));
  colorValues.push_back(Vec3f(120,255,255));
  colorValues.push_back(Vec3f(150,255,128));

}

void RegionBasedColorTransfer::showSource() const
{
  namedWindow("sourceImage", 1);
  imshow("sourceImage", source);
  waitKey();
}

void RegionBasedColorTransfer::showTarget()const
{
  namedWindow("targetImage",1);
  imshow("targetImage", target);
  waitKey();
}

void RegionBasedColorTransfer::segmentImages(vector<vector<Pixel> >&sourceBuffer, vector<vector<Pixel> >&targetBuffer ) const
{

  IplImage source1 = (IplImage)source;
  IplImage target1 = (IplImage)target;
  //vector<vector<Pixel> > sourceBuf;
  //vecctor<vector<Pixel> > targetBuf;

  segmentImage(&source1, sourceBuffer);
  segmentImage(&target1, targetBuffer);

  //sourceBuffer = sourceBuf;
  //targetBuffer = targetBuf;
}

void RegionBasedColorTransfer::classifyColor()
{

  Mat hsvSource, hsvTarget;

  cvtColor(source, hsvSource, CV_BGR2HSV);
  cvtColor(target, hsvTarget, CV_BGR2HSV);

  int source_rows = source.rows;
  int source_cols = source.cols;

  for(int i = 0; i < source_rows; i++){
    for(int j = 0; j < source_cols; j++){
      //Pixel px = Pixel(i,j);
      Pixel px = {i,j};
      //cout<<"px = ("<<i<<','<<j<<")\n";
      int index = classifyColor(hsvSource, px);
      sourceClassification[index].push_back(px);

    }
  }

  //cout<<"source color classification done\n";

  int target_rows = target.rows;
  int target_cols = target.cols;

  for(int i = 0; i < target_rows; i++){
    for(int j = 0; j < target_cols; j++){
      Pixel px = {i,j};
      int index = classifyColor(hsvTarget, px);
      targetClassification[index].push_back(px);
    }
  }

}
