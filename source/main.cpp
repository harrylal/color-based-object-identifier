#include "object.h"
#include <opencv2/opencv.hpp>

#define CAMERAPORT 0  

int redMinHSV[]={113,119,76};
int redMaxHSV[]={163,214,235};
int redBondColor[]={0,0,255};
char redLabel[]={"RED"};
int yellowMinHSV[]={54,74,125};
int yellowMaxHSV[]={117,247,255};
int yellowBondColor[]={0,255,255};
char yellowLabel[]={"YELLOW"};
int blueMinHSV[]={0,96,81 };
int blueMaxHSV[]={20,255,231};
int blueBondColor[]={255,0,0};
char blueLabel[]={"BLUE"};


int main(int argc ,char** argv)
{
  cv::Mat raw;
  cv::Mat output;
  object red(redMinHSV,redMaxHSV,redBondColor,redLabel);
  object yellow(yellowMinHSV,yellowMaxHSV,yellowBondColor,yellowLabel);
  object blue(blueMinHSV,blueMaxHSV,blueBondColor,blueLabel);

  cv::namedWindow("LIVE",cv::WINDOW_AUTOSIZE);
  cv::VideoCapture live(CAMERAPORT);

  while(1)
  {
    live >> output;
    output.copyTo(raw);
    red.find(raw.clone(),output);

    yellow.find(raw.clone(),red.covtBinary(raw.clone()),output);
    blue.find(raw.clone(),red.covtBinary(raw.clone()),output);
    cv::imshow("LIVE",output);
    cv::waitKey(1);
  }
}

