#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <stdio.h>
 
int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  cv::Mat image = cv::imread("/home/weitter/Pictures/1.jpg", CV_LOAD_IMAGE_COLOR);
  if(image.empty())
  {
    printf("open error\n");
  }
  cv::imshow("",image);
  cv::waitKey(3000);
  cv::destroyWindow("");
  ros::spin();
}

