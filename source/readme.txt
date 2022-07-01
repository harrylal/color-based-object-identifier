info:-
The hsv range can and the object label can be changed from the bottom of the code to suite your needs

--------------------------------------------------

Required dependencies:-
1.opencv2/opencv3

--------------------------------------------------
To Declare an object:
object <object name>(minHSV_array[3],maxHSV_array[3],label_array[7]);


Functions:
cv::Mat covtBinary(cv::Mat);//returns the binary imgae after thresholding
void find(cv::Mat,cv::Mat);//draws the contours on passed image and draws the boundingRectangle

---------------------------------------------------

