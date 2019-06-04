info:-
The hsv range can and the object label can be changed from the bottom of the code to suite your needs

--------------------------------------------------

Required dependencies:-
1.opencv2/opencv3

--------------------------------------------------
To Declare an object:
object <object name>(minHSV_array[3],maxHSV_array[3],label_array[7]);


Functions:
cv::Mat covtBinary(cv::Mat);//returns the binary Matrix after thresholding
void find(cv::Mat,cv::Mat);//draws the contours on passed matrix and draws the boundingRectangle
void find(cv::Mat,cv::Mat,cv::Mat);//finds object after subtracting 2nd argument(binary matrix) from first matrix(BGR) and draws the boundingRect

---------------------------------------------------

