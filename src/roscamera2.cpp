#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
 
int main(int argc, char** argv)
{
  ros::init(argc, argv, "img_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image", 1);
 
  cv::VideoCapture cap;
  cv::Mat frame;
 cv::Mat img_out;
  int deviceID=0;
  if(argc>1)
	deviceID=argv[1][0]-'0';
  int apiID=cv::CAP_ANY;
  cap.open(deviceID+apiID);
  if(!cap.isOpened()){
	std::cerr<<"ERROR! Unable to open camera"<<std::endl;
	return -1;
  }
 
  ros::Rate loop_rate(30);
  while (nh.ok()) {
	cap.read(frame);
	if(!frame.empty()){
		sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
		pub.publish(msg);
	}
       cv::cvtColor(frame, img_out, CV_RGB2GRAY);  //转换成灰度图象
       cv::imshow("img_out", img_out);
        cv::imshow("camera", frame);
        cv::waitKey(3); // opencv刷新图像 3ms
    	ros::spinOnce();
    	loop_rate.sleep();
  }
  return 0;
}

