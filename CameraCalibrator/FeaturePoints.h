#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\xfeatures2d\nonfree.hpp>
#include<opencv2\calib3d.hpp>
#include<opencv2\tracking.hpp>
#include<vector>
#include<iostream>


using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

namespace CC {

	class FeaturePoints
	{

	public:

		FeaturePoints()
		{
			detector = SURF::create(400);
			extractor = detector;
		}

		FeaturePoints(Ptr<FeatureDetector>  _detector,
			Ptr<DescriptorExtractor> _extractor)
		{
			detector = _detector;
			extractor = _extractor;
		}

		void compute(const Mat & _image) {
			keypoints.clear();
			if (_image.channels() == 1) {
				detector->detect(_image, keypoints);
				extractor->compute(_image, keypoints, descriptors);
			}
			else
			{
				Mat gray_image;
				if (_image.channels() == 3)
					cv::cvtColor(_image, gray_image, CV_BGR2GRAY);
				else if (_image.channels() == 4)
					cv::cvtColor(_image, gray_image, CV_BGRA2GRAY);

				detector->detect(gray_image, keypoints);
				extractor->compute(gray_image, keypoints, descriptors);

			}
		}

		const Mat & descriptor() const {
			return descriptors;
		}

		const std::vector<cv::KeyPoint> & keypoint() const {
			return keypoints;
		}
		//private:
		std::vector<cv::KeyPoint> keypoints;
		Mat descriptors;
	private:
		Ptr<FeatureDetector>     detector;
		Ptr<DescriptorExtractor> extractor;

	};

}