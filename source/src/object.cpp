#include <opencv2/opencv.hpp>
#include "object.h"

cv::Mat object::covtBinary(cv::Mat rawFile) //function takes a matrix and covtBinarys the given component
{
  cv::Mat unitone ;//output matrix
  cv::Mat kernel(3,3,CV_32F,1);
  cv::cvtColor(rawFile,unitone,cv::COLOR_RGB2HSV);//conerting to hsv
  unitone = equalize(unitone);//performs histogram equalisation
  cv::blur(unitone,unitone,cv::Size(5,5));//blurs the  image
  cv::inRange(unitone,cv::Scalar(minHSV[0],minHSV[1],minHSV[2]),cv::Scalar(maxHSV[0],maxHSV[1],maxHSV[2]),unitone);
  cv::morphologyEx(unitone,unitone,cv::MORPH_CLOSE,kernel);
  return unitone ;
}

cv::Mat object::equalize(cv::Mat input)//equalises the given hsv frame and returns the equalised frame
{
  cv::Mat equalized;
  std::vector<cv::Mat> channels;
  cv::split(input,channels);//splits into h,s,v
  cv::equalizeHist(channels[2],channels[2]);//performs histogram equalisation on v val
  cv::merge(channels,equalized);//merges all the channels together

  return equalized;
}

void object::find(cv::Mat raw,cv::Mat outputMat)//finds the object from raw and draws the enclosing box on outputMat
{
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Point> largest ;
  cv::Mat output;
  output = covtBinary(raw);
  cv::findContours(output,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
  if(contours.empty()!=1)
  {
    for(int i=0 ;i < contours.size() ; i++)// sets Default largest contour
        if(fabs(cv::contourArea(contours[i])) > 1000)
          {
          largest = contours[i];
          break;
          }
    for(int i=0 ;i < contours.size()&&largest.size()!=0; i++)//finds largest contour
    {
      if(fabs(cv::contourArea(contours[i])) > fabs(cv::contourArea(largest)))
        largest = contours[i];
    }
    if(largest.empty()!=1)
    {
      cv::Rect enclosingRect = cv::boundingRect(largest);//create a bounding Rect
      cv::rectangle(outputMat,enclosingRect.tl(),enclosingRect.br(),boundRectColor,3);//draws the rectangle
      cv::putText(outputMat,label,enclosingRect.tl(),1,1,cv::Scalar(0,0,0),2);//puts the text
    }
  }

}

void object::find(cv::Mat raw,cv::Mat subtractFrom,cv::Mat outputMat)//findcontuors after subtration of raw from subtractFrom
{
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Point> largest ;
  cv::Mat output;
  output = covtBinary(raw);
  cv::subtract(output,subtractFrom,output);//subtracts the matrix
  cv::findContours(output,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
  if(contours.empty()!=1)
  {
    for(int i=0 ;i < contours.size() ; i++)
        if(fabs(cv::contourArea(contours[i])) > 1000)
          {
          largest = contours[i];
          break;
          }
    for(int i=0 ;i < contours.size()&&largest.size()!=0; i++)
    {
      if(fabs(cv::contourArea(contours[i])) > fabs(cv::contourArea(largest)))
        largest = contours[i];
    }
    if(largest.empty()!=1)
    {
      cv::Rect enclosingRect = cv::boundingRect(largest);
      cv::rectangle(outputMat,enclosingRect.tl(),enclosingRect.br(),boundRectColor,3);
      cv::putText(outputMat,label,enclosingRect.tl(),1,1,cv::Scalar(0,0,0),2);
    }
  }
}


object::object(int min[3],int max[3],int bRectColor[3],char tag[7])//constructor
{
    for(int i=0 ; i<3;i++)
    {
      minHSV[i] = min[i];
      maxHSV[i] = max[i];
    }
    strcpy(label,tag);
    boundRectColor = cv::Scalar(bRectColor[0],bRectColor[1],bRectColor[2]);

}





