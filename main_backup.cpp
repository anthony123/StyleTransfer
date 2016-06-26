#include <cv.h>
#include <highgui.h>
#include <vector>
#include "Segmentation.h"
#include "imconv.h"
#include "misc.h"

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{

  // vector<CvScalar>rgbs;
  // rgbs.push_back(cvScalar(128, 0, 0));
  // rgbs.push_back(cvScalar(153,51,0));
  // rgbs.push_back(cvScalar(255,153,0));
  // rgbs.push_back(cvScalar(255,204,153));
  // rgbs.push_back(cvScalar(51,51,0));
  // rgbs.push_back(cvScalar(153,204,0));
  // rgbs.push_back(cvScalar(0,51,0));
  // rgbs.push_back(cvScalar(204,255,204));
  // rgbs.push_back(cvScalar(51,204,204));
  // rgbs.push_back(cvScalar(204,255,255));
  // rgbs.push_back(cvScalar(0,204,255));
  // rgbs.push_back(cvScalar(153,153,153));
  // rgbs.push_back(cvScalar(72, 209, 204));
  // rgbs.push_back(cvScalar(210, 180, 140));
  // rgbs.push_back(cvScalar(255, 250, 205));

  //const CvScalar rgbs[] = [cvScalar(128, 0, 0),, , , ),, , , , ,, cvScalar(128,0),];


  // int i = 0;
  // IplImage * img = cvLoadImage(argv[1],1);
  // int width = img->width;
  // int height = img->height;
  // int channel = img->nChannels;
  // IplImage* result = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U,channel);
  // vector<vector<Pixel> > resultBuffer;
  // //cout<<"width = "<<width<<", height = "<<height<<'\n';
  // cout<<"about to segment\n";
  // segmentImage(img, resultBuffer);
  // cout<<"segment done\n";
  // cout<<"there are "<<resultBuffer.size()<<" components\n";
  // //481 321
  // for(auto buffers : resultBuffer){
  //   auto color = rgbs[i++];
  //     //CvScalar color = cvScalar(255,255,255);
  //   for (auto buffer:buffers){
  //     cvSet2D(result, buffer.y, buffer.x, color);
  //     //cout<<"x = "<<buffer.x<<",y = "<<buffer.y<<'\n';
  //     //result.at(buffer.x, buffer.y)[0] = color.b;
  //     //result.at(buffer.x, buffer.y)[1] = color.g;
  //     //result.at(buffer.x, buffer.y)[2] = color.r;
  //   }
  // }
  //
  // cvNamedWindow("segmented images", 1);
  // cvShowImage("segmented images", result);
  // cvWaitKey();
  //
  // cvDestroyWindow("segmented images");
  // cvReleaseImage(&img);
  // cvReleaseImage(&result);

  Mat source = imread("source.jpeg", 1);
  Mat target = imread("target.jpeg",1);
  vector<vector<Pixel> > sourceBuffer;
  vector<vector<Pixel> > targetBuffer;

  //segment source and target images
  segmentImage(source, sourceBuffer);
  SegmentImage(target, targetBuffer);

  //HSV color space
  Mat hsvSource, hsvTarget;
  Mat labSource, labTarget;

  cvtColor(source, hsvSource, CV_BGR2HSV);
  cvtColor(target, hsvTarget, CV_BGR2HSV);

  cvtColor(source, labSource, CV_BGR2Lab);
  cvtColor(target, hsvTarget, CV_BGR2Lab);



  // //divide pixel colors into balck white gray red orange yellow green blue purple
  // vector<vector<vector<Pixel> > >classifiedSourceBuffer, classifiedTargetBuffer;
  //
  // //generate source pixels
  // for (auto pixels: sourceBuffer){
  //   vector<vector<Pixel> >buffer(utility::number_of_color_type);
  //   for(auto pixel: pixels){
  //     int index = utility::classifyColor(pixel);
  //     buffer[index].push_back(pixel);
  //   }
  //   classifiedSourceBuffer.push_back(buffer);
  // }
  //
  // //generate target pixels
  // for (auto pixels: targetBuffer){
  //   vector<vector<Pixel> >buffer(utility::number_of_color_type);
  //   for(auto pixel: pixels){
  //     int index = utility::classifyColor(pixel);
  //     buffer[index].push_back(pixel);
  //   }
  //   classifiedTargetBuffer.push_back(buffer);
  // }

  //color transfer globally

  //source image pixel classified on basic colors
  vector<vector<Pixel> > sourceClassification(utility::number_of_color_type);
  int source_rows = source.rows;
  int target_cols = source.cols;

  for(int i = 0; i < source_rows; i++){
    for(int j = 0; j < source_cols; j++){
      Pixel px = Pixel(i,j);
      int index = utility:classifyColor(hsvSource, px);
      sourceClassification[i].push_back(px);
    }
  }

  vector<vector<Pixel> > targetClassification(utility::number_of_color_type);
  int target_rows = target.rows;
  int target_cols = target.cols;

  for(int i = 0; i < target_rows; i++){
    for(int j = 0; j < target_cols; j++){
      Pixel px = Pixel(i,j);
      int index = utility::classifyColor(hsvTarget, px);
      targetClassification[i].push_back(px);
    }
  }

  //mean color value of each basic color
  vector<Double> meanSourceLValues(utility::number_of_color_type);
  vector<Double> meanSourceAValues(utility::number_of_color_type);
  vector<Double> meanSourceBValues(utility::number_of_color_type);

  vector<Double> meanTargetLValues(utility::number_of_color_type);
  vector<Double> meanTargetAValues(utility::number_of_color_type);
  vector<Double> meanTargetBValues(utility::number_of_color_type);

  //standard deviation of each basic color
  vector<Double> SDSourceValues(utility::number_of_color_type);
  vector<Double> SDTargetValues(utility::number_of_color_type);


  //mean value for source image
  for(int i = 0; i < utility::number_of_color_type; i++){
    int num = 0;
    double totalL = 0, totalA = 0, totalB = 0;

    for(auto px: sourceClassification){
      Vec3f value = labSource.at<Vec3f>(px.x, px.y);
      num++;
      totalL += value[0];
      totalA += value[1];
      totalB += value[2];
    }
    double meanL = totalL/num;
    double meanA = totalA/num;
    double meanB = totalB/num;

    meanSourceHValues[i] = meanL;
    meanSourceSValues[i] = meanA;
    meanSourceVValues[i] = meanB;

  }

  //standard deviation of source image
  for(int i = 0; i < utility::number_of_color_type; i++){
      int num = 0;
      double totalSquared = 0.0;
      for(auto px: sourceClassification){
        Vec3f value = labSource.at<Vec3f>(px.x, px.y);
        num++;
        totalSquared += (value[0] - meanSourceLValues[i])*(value[0] - meanSourceLValues[i]);
        totalSquared += (value[1] - meanSourceAValues[i])*(value[1] - meanSourceAValues[i]);
        totalSquared += (value[2] - meanSourceBValues[i])*(value[2] - meanSourceBValues[i]);
      }
      double meanSquared = totalSquared/num;
      SDSourceValues[i] = sqrt(meanSquared);
  }

  //mean value of target image
  for(int i = 0; i < utility::number_of_color_type; i++){
    int num = 0;
    double totalL = 0, totalA = 0, totalB = 0;

    for(auto px in targetClassification){
      Vec3f value = labTarget.at<Vec3f>(px.x, px.y);
      num++;
      totalL += value[0];
      totalA += value[1];
      totalB += value[2];
    }
    double meanL = totalL/num;
    double meanA = totalA/num;
    double meanB = totalB/num;

    meanTargetLValues[i] = meanH;
    meanTargetSAalues[i] = meanS;
    meanTargetBValues[i] = meanV;

  }


  //standard deviation of target image
  for(int i = 0; i < utility::number_of_color_type; i++){
      int num = 0;
      double totalSquared = 0.0;
      for(auto px: targetClassification){
        Vec3f value = labTarget.at<Vec3f>(px.x, px.y);
        num++;
        totalSquared += (value[0] - meanTargetLValues[i])*(value[0] - meanTargetLValues[i]);
        totalSquared += (value[1] - meanTargetAValues[i])*(value[1] - meanTargetAValues[i]);
        totalSquared += (value[2] - meanTargetBValues[i])*(value[2] - meanTargetBValues[i]);
      }
      double meanSquared = totalSquared/num;
      SDTargetValues[i] = sqrt(meanSquared);
  }

  //color transfer
  for(int i = 0; i < utility::number_of_color_type; i++){
    double SDRatio = SDSourceValues[i]/SDTargetValues[i];
    for(auto px: targetClassification){
      Vec3f value = labTarget.at<Vec3f>(px.x, px.y);
      float L = SDRatio *(value[0] - meanTargetLValues[i]) + meanSourceLValues[i];
      float A = SDRatio *(value[1] - meanTargetAValues[i]) + meanSourceAValues[i];
      float B = SDRatio *(value[2] - meanTargetBValues[i]) + meanSourceBValues[i];
      labTarget.at<Vec3f>(px.x, px.y) = Vec3f(L,A,B);
    }
  }

  //convet back from lab to bgr
  cvtColor(labTarget, target, CV_Lab2BGR);
  


  //color transfer from target image to source image within each basic color
  for(int i = 0; i < utility::number_of_color_type; i++){

  }


  //target image pixel classified on basic colors
  vector<Pixel> targetClassification(utility::number_of_color_type);


  //color transfer for each color classification of each region
  colorTransfer(source, );

  return 0;
}
