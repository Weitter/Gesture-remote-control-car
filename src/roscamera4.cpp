#include<opencv2/opencv.hpp>
#include"ros/ros.h"
#include"std_msgs/String.h"
#include"rosOpenCV/roscamera.h"
#include<sstream>
#include<string.h>

using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	VideoCapture capture;
	capture.open(0);
	if(!capture.isOpened())
	{
		printf("can not open video file   \n");
		return -1;
	}
	Mat imgOriginal;
	Mat kernel;
	vector<Vec4i> hireachy;
	Rect rect;
	Point2f center;
	float radius=20;
	
	ros::init(argc, argv, "talker");  // 节点名称
	ros::NodeHandle n;

	// 话题topic的名称
	ros::Publisher roscamera_pub = n.advertise<rosOpenCV::roscamera>("roscamera_msg", 1000);
	ros::Rate loop_rate(100);


	Mat mask(imgOriginal.rows, imgOriginal.cols, CV_8UC1);  // 2值掩膜  
	Mat dst(imgOriginal); // 输出图像 
       vector<vector<Point>> contours;   // 轮廓  
        vector<Vec4i> hierarchy;    // 轮廓的结构信息  
        int presentX=0;  
        int presentY=0;  
	//行也就是rows, 对应的是y，矩形的height信息就是在y方向；
        //列也就是cols, 对应的是x，矩形的width信息就是在x方向； 	
        int WIDTH=640;  
        int HEIGHT=480;  


	while (capture.read(imgOriginal) && ros::ok() )
	{	
		  Mat imgHSV;
		 rosOpenCV::roscamera msg;
   		  vector<Mat> hsvSplit;
		  resize(imgOriginal,imgOriginal,Size(WIDTH,HEIGHT),0,0, INTER_LINEAR);//width  height
		  medianBlur(imgOriginal,imgOriginal, 5);//中值滤波 椒盐噪声  
   		  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	          Mat dstTemp1(imgOriginal.rows, imgOriginal.cols, CV_8UC1);  
                  Mat dstTemp2(imgOriginal.rows, imgOriginal.cols, CV_8UC1);  
                 // 对HSV空间进行量化，得到2值图像，亮的部分为手的形状  
                  inRange(imgHSV, Scalar(0,30,30), Scalar(40,170,256), dstTemp1);  
                   inRange(imgHSV, Scalar(156,30,30), Scalar(180,170,256), dstTemp2);  
                  bitwise_or(dstTemp1, dstTemp2, mask);  
                  //imshow("output1", dstTemp1);
		  //imshow("output2", dstTemp2);
	 	  // 形态学操作，去除噪声，并使手的边界更加清晰  
                  Mat element = getStructuringElement(MORPH_RECT, Size(3,3));  
                  erode(mask, mask, element);  
                  morphologyEx(mask, mask, MORPH_OPEN, element);  
                  dilate(mask, mask, element);  
                  morphologyEx(mask, mask, MORPH_CLOSE, element);  
                  imgOriginal.copyTo(dst, mask);  
                  contours.clear();  
                  hierarchy.clear();  
		//imshow("output", mask);
		  // 得到轮廓  
                  findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		  int n=0;
		 for (size_t i = 0; i < contours.size(); i++)  
                 {             
		   n = fabs(contourArea(Mat(contours[i])));
		   //printf("contourArea=%d\n",n);   
                    if (n> 20000) //判断手进入区域的阈值  
                    {  
                     
 		        drawContours(dst, contours, -1, Scalar(255,255,255), 3); 
		        double maxArea=0;
		        double area = contourArea(contours[static_cast<int>(i)]);
		       if (area > maxArea)
		      {
			   maxArea = area;
			   rect = boundingRect(contours[static_cast<int>(i)]);
			   minEnclosingCircle(contours[static_cast<int>(i)], center, radius);
		      }
                    } 

                  }
		line(dst, Point(center.x-2,center.y), Point(center.x+2,center.y), Scalar(0,255,0), 1, CV_AA);
		line(dst, Point(center.x,center.y-2), Point(center.x,center.y+2), Scalar(0,255,0), 1, CV_AA);
		//circle(dst, Point(center.x,center.y), (int)radius, Scalar(0,255,0), 2);
		if((WIDTH/2-center.x)<-20) 
                {  
                    	printf("left\n");
			msg.left=1;                		 
                }
		else
			msg.left=0;  
		 if((WIDTH/2-center.x)>20)  
               	{  
                     	printf("right\n");  
                     	msg.right=1;  	 
                }
		else
			msg.right=0;	  
                if((HEIGHT/2-center.y)<-20)  
                {	  
                       printf("down\n"); 
		       msg.down=1;   
                } 
		else
			msg.down=0;    
                if((HEIGHT/2-center.y)>20)  
                {  
                    	printf("up\n");  
			msg.up=1;    
                }
		else
			msg.up=0;     
	      imshow("dst", dst);
	      roscamera_pub.publish(msg);//发布消息
	      ros::spinOnce();
	      loop_rate.sleep();	
	      waitKey(3);	
     }
     capture.release();
     return 0;
}

