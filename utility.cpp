#include "cv.h"
#include "Segmentation.h"
#include <algorithm>

using namespace cv;

namespace utility{
  int classifyColor(const cv::Mat &image, Pixel position)
  {
    int x = position.x;
    int y = position.y;
    Vec3f value = image.at<Vec3f>(x,y);

    int index = minColorDistance(value);

    return index;
  }

  //find the min color Eclidean basic color
  int minColorDistance(const Vec3f & value)
  {
      vector<double>dists;
      double min_distance = 100000;
      int min_index = -1;

      for(int i = 0; i < number_of_color_type; i++)
      {
        double dist = norm(value - colorValues[i]);
        if(dist < min_distance){
          min_distance = dist;
          min_index = i;
        }
      }

      return min_index;
  }

}
