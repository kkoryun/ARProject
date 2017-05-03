#include "opencv2\core.hpp"
#include <opencv2\highgui.hpp>
#include<opencv2\xfeatures2d\nonfree.hpp>


//#define CC_DEBUG_
#include"CameraCalibrator.h"
int main() {
	cv::namedWindow("1");
	CC::IntrinsicCalibrator icalib(526.58037684199849f, 524.65577209994706f, 318.41744018680112f, 202.96659047014398f);
	cv::Mat object;
	object = cv::imread("obj.bmp");
	CC::ExtrinsicCalibrator ecalib(object, icalib);

	
    cv::VideoCapture vid_cap("sce.avi");
	if (!vid_cap.isOpened()) {
		printf("Fail");
	cv::waitKey();
	return 0;
	}
	cv::Mat scene;
    vid_cap >> scene;
	

	while (!scene.empty())
	{
		ecalib.compute(scene);
		cv::line(scene, ecalib.scene_points2d[0], ecalib.scene_points2d[1], 0,3);
		cv::line(scene, ecalib.scene_points2d[1], ecalib.scene_points2d[2], 0,3);
		cv::line(scene, ecalib.scene_points2d[2], ecalib.scene_points2d[3], 0,3);
		cv::line(scene, ecalib.scene_points2d[3], ecalib.scene_points2d[0], 0,3);
		cv::imshow("1", scene);
		cv::waitKey(33);
		vid_cap >> scene;

	}

	cv::waitKey();
	return 0;
}