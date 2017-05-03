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

	class IntrinsicCalibrator
	{
	public:

		IntrinsicCalibrator() {
			intrinsic_mat = Mat(3, 3, CV_32F, cv::Scalar(0));
			distortion_coeff = vector<float>(5);
		}

		IntrinsicCalibrator(float _fx, float _fy, float _cx, float _cy) {

			intrinsic_mat = Mat(3, 3, CV_32F, cv::Scalar(0));
			distortion_coeff = vector<float>(5);

			intrinsic_mat.at<float>(0, 0) = _fx;
			intrinsic_mat.at<float>(1, 1) = _fy;
			intrinsic_mat.at<float>(0, 2) = _cx;
			intrinsic_mat.at<float>(1, 2) = _cy;
			intrinsic_mat.at<float>(2, 2) = 1;
		}

		IntrinsicCalibrator(float _fx, float _fy, float _cx, float _cy, const std::vector<float> & _distortion_coeff) {
			intrinsic_mat = Mat(3, 3, CV_32F, cv::Scalar(0));
			distortion_coeff = _distortion_coeff;

			intrinsic_mat.at<float>(0, 0) = _fx;
			intrinsic_mat.at<float>(1, 1) = _fy;
			intrinsic_mat.at<float>(0, 2) = _cx;
			intrinsic_mat.at<float>(1, 2) = _cy;
		}

		vector<float>  distortion() {
			return distortion_coeff;
		}

		void distortion(const std::vector<float> & _distortion_coeff) {
			distortion_coeff = _distortion_coeff;
		}

		void compute() {

		}

		float fx() const { return intrinsic_mat.at<float>(0, 0); };
		float fy() const { return intrinsic_mat.at<float>(1, 1); };
		float cx() const { return intrinsic_mat.at<float>(0, 2); };
		float cy() const { return intrinsic_mat.at<float>(1, 2); };
		float fx(float _fx) { intrinsic_mat.at<float>(0, 0) = _fx; };
		float fy(float _fy) { intrinsic_mat.at<float>(1, 1) = _fy; };
		float cx(float _cx) { intrinsic_mat.at<float>(0, 2) = _cx; };
		float cy(float _cy) { intrinsic_mat.at<float>(1, 2) = _cy; };

		Mat intrinsic_mat;
		std::vector<float> distortion_coeff;
	};

};
