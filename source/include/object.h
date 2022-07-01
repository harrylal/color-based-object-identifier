#include <opencv2/opencv.hpp>

class object
{
  int minHSV[3];//minimum h,s and v values
  int maxHSV[3];//maximum h,s and v vales
  char label[7];//tag
  cv::Scalar boundRectColor;//bounding rectangle colour
public:
  object(int[],int[],int[],char[]);//minhsv,maxhsv,bounfing rectangle colour,label
  cv::Mat covtBinary(cv::Mat);//function to covert to binary
  cv::Mat equalize(cv::Mat);//equalizes the image ;
  void find(cv::Mat,cv::Mat);//draws the contours on passed matrix and draws the boundingRectangle
  void find(cv::Mat,cv::Mat,cv::Mat);//finds object after subtracting 2nd argument from first and draws the boundingRect
};