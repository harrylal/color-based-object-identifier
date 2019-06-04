#include <opencv2/opencv.hpp>
#include <stdio.h>


/*the main function is on the bottom of this file*/

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



/////////////////////////////////////////////////////////////////////////////////////////main code//////////////////////////////////////////////////////////////////////////


#define CAMERAPORT 0

int redMinHSV[]={113,119,76};
int redMaxHSV[]={163,214,235};
int redBondColor[]={0,0,255};
char redLabel[]={"BOX"};
int yellowMinHSV[]={54,74,125};
int yellowMaxHSV[]={117,247,255};
int yellowBondColor[]={0,255,255};
char yellowLabel[]={"DISC"};
int blueMinHSV[]={0,96,81 };
int blueMaxHSV[]={20,255,231};
int blueBondColor[]={255,0,0};
char blueLabel[]={"BOTTLE"};


int main(int argc ,char** argv)
{
  cv::Mat raw;
  cv::Mat output;

  object red(redMinHSV,redMaxHSV,redBondColor,redLabel);
  object yellow(yellowMinHSV,yellowMaxHSV,yellowBondColor,yellowLabel);
  object blue(blueMinHSV,blueMaxHSV,blueBondColor,blueLabel);

  cv::namedWindow("LIVE",cv::WINDOW_AUTOSIZE);
  cv::VideoCapture live(0);

  while(1)
  {
    live >> output;
    output.copyTo(raw);
    red.find(raw.clone(),output);

    yellow.find(raw.clone(),red.covtBinary(raw.clone()),output);
    blue.find(raw.clone(),red.covtBinary(raw.clone()),output);
    cv::imshow("LIVE",output);
    cv::waitKey(40);
  }
}
