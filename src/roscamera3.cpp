#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
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
/*
	//开操作处理
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
  	int iLowH = 100;
  	int iHighH = 140;
 
  	int iLowS = 90; 
  	int iHighS = 255;
 
  	int iLowV = 90;
 	 int iHighV = 255;
 
  	//Create trackbars in "Control" window
  	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 	 cvCreateTrackbar("HighH", "Control", &iHighH, 179);
 
  	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 	 cvCreateTrackbar("HighS", "Control", &iHighS, 255);
 
  	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
  	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
 
	kernel = getStructuringElement(MORPH_RECT, Size(5, 5));

	namedWindow("input", CV_WINDOW_AUTOSIZE);
	namedWindow("output", CV_WINDOW_AUTOSIZE);
*/
	//vector<vector<Point> > contours;
	vector<Vec4i> hireachy;
	Rect rect;
	Point2f center;
	float radius=20;

	Mat mask(imgOriginal.rows, imgOriginal.cols, CV_8UC1);  // 2值掩膜  
	Mat dst(imgOriginal); // 输出图像 
       vector<vector<Point>> contours;   // 轮廓  
       vector<vector<Point>> filterContours; // 筛选后的轮廓  
       vector<Vec4i> hierarchy;    // 轮廓的结构信息  
        vector<Point> hull; // 凸包络的点集  
	
	bool movement=false;   
        int count=0;  
      
        int presentX=0;  
        int presentY=0;  
	//行也就是rows, 对应的是y，矩形的height信息就是在y方向；
        //列也就是cols, 对应的是x，矩形的width信息就是在x方向；
        int minX=320;//屏幕的一半  480 848    480 640
        int maxX=320;  
        int minY=240;  
        int maxY=240;  
  	int delay = 1;  
        char c;  
        int frameNum = -1;          // Frame counter  
        bool lastImgHasHand=false;  
  	
        int previousX=0;  
        int previousY=0;  
	bool bHandFlag = false;  
	while (capture.read(imgOriginal))
	{	
		  Mat imgHSV;
		 
   		  vector<Mat> hsvSplit;
		  resize(imgOriginal,imgOriginal,Size(640,480),0,0, INTER_LINEAR);//width  height
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
                  filterContours.clear();  
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
                        filterContours.push_back(contours[i]); 	
 		        drawContours(dst, contours, -1, Scalar(255,0,255), 3); 
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
		if((320-center.x)<20) 
                {  
                    	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<left\n");                		 
                }  
		 if((320-center.x)>20)  
               	{  
                     	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>right\n");  
                     		 
                }	  
                if((240-center.y)<20)  
                {	  
                       printf("downVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");  
                }  
                if((240-center.y)>20)  
                {  
                    	printf("upAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n");  
                }  

		//printf("rows= %d cols =%d\n",dst.rows,dst.cols);
		// 画轮廓 

		
/*
        	if(filterContours.size()>0)  
       		 {  
      
            		count++;  
            		lastImgHasHand=true;  
            		drawContours(dst, filterContours, -1, Scalar(255,0,255), 3);  
            		for (size_t j=0; j<filterContours.size(); j++)  
            		{  
                		convexHull(Mat(filterContours[j]), hull, true);  
                		int hullcount = (int)hull.size(); 				
                		for (int i=0; i<hullcount-1; i++)  
                		{	  
					//白色
                    			line(dst, hull[i+1], hull[i], Scalar(255,255,255), 2, CV_AA);            
                    			//printf("num%d:x=%d\ty=%d\t\n",i,hull[i].x,hull[i].y);  
                    			if(hull[i].x>maxX)  
                       				 maxX=hull[i].x;  
                   			if(hull[i].x<minX)  
                       				 minX=hull[i].x;  
                    			if(hull[i].y>maxY)  
                        			 maxY=hull[i].y;  
                    			if(hull[i].y<minY)  
                        			 minY=hull[i].y;  
                    			printf("miniX=%d\tminiY=%d\tmaxX=%d\tmaxY=%d\t\n",minX,minY,maxX,maxY);  
                		}  
         
                		line(dst, hull[hullcount-1], hull[0], Scalar(0,255,0), 2, CV_AA);//绿色，最后一条  
                  
                		if(count==1)//第一个轮廓的中心位置存在全局变量中，到最后一个再跟它比。  
                		{  
                    			previousX=320;  
                    			previousY=240;  
                    			//printf("previousX=%d  previousY=%d\n",previousX,previousY);  
                		}  
                		else  
                		{  
                    			presentX=(minX+maxX)/2;  
                    			presentY=(minY+maxY)/2;  
					//printf("presentX=%d presentY=%d\n",presentX,presentY);  
                		}  
            		}  
			
        	}  
       		 else  
        	{     
            		if(lastImgHasHand==true)  
            		{  
      printf("previousX=%d\tpresentX=%d\tpreviousY=%d\tpresentY=%d\n",previousX,presentX,previousY,presentY); 
                printf("(previousX-presentX)=%d (previousY-presentY)=%d\n",(previousX-presentX),(previousY-presentY));
                		
				if((previousX-presentX)<0) 
                		{  
                    			printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<left\n");
                    		 
                	        }  
                	      if((previousX-presentX)>0)  
               		        {  
                     		 	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>right\n");  
                     		 
                		}	  
                		if((previousY-presentY)<0)  
                		{	  
                    			printf("downVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");  
               			}  
                		if((previousY-presentY)>0)  
                		{  
                    			printf("upAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n");  
                		}  
  
               		 	count=0;  
                		lastImgHasHand=false;  
            		}  

	  	}
	*/
/*
    //  inRange(frameHSV, Scalar(0,30,30), Scala
		//因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
   		split(imgHSV, hsvSplit);
   		equalizeHist(hsvSplit[2],hsvSplit[2]);
   		merge(hsvSplit,imgHSV);
   		Mat imgThresholded;
      		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
		   //开操作 (去除一些噪点)
   		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
   		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
 
   		//闭操作 (连接一些连通域)
  		 morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
		//blur(frame, dst, Size(5,5));
		
		//获取边界
		findContours(imgThresholded, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0,0));
		//框选面积最大的边界
		if (contours.size() > 0)
		{
			double maxArea=0;
			for (int i = 0; i < contours.size(); i++)
			{
				double area = contourArea(contours[static_cast<int>(i)]);
				if (area > maxArea)
				{
					maxArea = area;
					rect = boundingRect(contours[static_cast<int>(i)]);
					minEnclosingCircle(contours[static_cast<int>(i)], center, radius);
				}
			}
		}
		//矩形框
		//rectangle(frame,rect, Scalar(0,255,0),2);
		//圆形框
		circle(imgOriginal, Point(center.x,center.y), (int)radius, Scalar(0,255,0), 2);
		printf("(%d,%d)\r\n",(int)center.x,(int)center.y);
		imshow("input", imgOriginal);
		imshow("output", imgThresholded);

		waitKey(100);
	}
*/
	imshow("dst", dst);	
 
	//printf("count=%d\n",count);  
	waitKey(3);	
     }
	capture.release();
	return 0;
}


