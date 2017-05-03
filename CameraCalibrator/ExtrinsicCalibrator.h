#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\xfeatures2d\nonfree.hpp>
#include<opencv2\calib3d.hpp>
#include<opencv2\tracking.hpp>
#include<vector>
#include<iostream>

#include"IntrinsicCalibrator.h"
#include"FeaturePoints.h"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

namespace CC {

	class ExtrinsicCalibrator {
	public:
		ExtrinsicCalibrator() {
			matcher = BFMatcher::create(NORM_L2, false);
#ifdef CC_DEBUG_
			namedWindow("img_matches");
			namedWindow("img_inlier_matches");
			namedWindow("Keypoints 1");
			namedWindow("Keypoints 2");
#endif // _DEBUG_

		}

		ExtrinsicCalibrator(const Mat & _object, const IntrinsicCalibrator & _calibrator) {
			matcher = BFMatcher::create(NORM_L2, false);
			setObject(_object);
			setCalibrator(_calibrator);
#ifdef CC_DEBUG_
			namedWindow("img_matches");
			namedWindow("img_inlier_matches");
			//namedWindow("Keypoints 1");
			//namedWindow("Keypoints 2");
#endif // _DEBUG_
		}

		void setCalibrator(const IntrinsicCalibrator & _calibrator) {
			calibration = _calibrator;
		}

		void setObject(const Mat & _object) {
			object_image = _object;

			object_points2d.resize(4);
			object_points3d.resize(4);

			float w = object_image.cols;
			float h = object_image.rows;

			float maxSize = std::max(w, h);
			float unitW = w / maxSize;
			float unitH = h / maxSize;

			object_points2d[0] = cv::Point2f(0, 0);
			object_points2d[1] = cv::Point2f(w, 0);
			object_points2d[2] = cv::Point2f(w, h);
			object_points2d[3] = cv::Point2f(0, h);

			object_points3d[0] = cv::Point3f(-unitW, -unitH, 0);
			object_points3d[1] = cv::Point3f(unitW, -unitH, 0);
			object_points3d[2] = cv::Point3f(unitW, unitH, 0);
			object_points3d[3] = cv::Point3f(-unitW, unitH, 0);

			object_detector.compute(object_image);
		}

		void compute(const Mat & scene)
		{

			scene_detector.compute(scene);
			match_point();

#ifdef CC_DEBUG_
			drawMatches(scene, scene_detector.keypoints, object_image, object_detector.keypoints,
				matches, img_matches, Scalar::all(-1), Scalar::all(-1),
				std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			imshow("img_matches", img_matches);
			waitKey(25);
			drawKeypoints(object_image, object_detector.keypoints, img_keypoints_object, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
			drawKeypoints(scene, scene_detector.keypoints, img_keypoints_scene, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
			//	imshow("Keypoints 1", img_keypoints_object);
			waitKey(20);
			//imshow("Keypoints 2", img_keypoints_scene);
			waitKey(20);
#endif // _DEBUG_

			compute_homography();
			//refine_matches_fundamental();
			//refine_matches_estimate();
			//refine_matches_homography();
			//compute_homography();
#ifdef CC_DEBUG_
			drawMatches(scene, scene_detector.keypoints, object_image, object_detector.keypoints,
				matches, img_inlier_matches, Scalar::all(-1), Scalar::all(-1),
				std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			imshow("img_inlier_matches", img_inlier_matches);
			waitKey(25);
#endif // _DEBUG_
			compute_camera_position();
		}

		Mat viewMat() const {
			return extrinsic_mat;
		}

		//private:

		void match_point()
		{
			if (!object_detector.descriptors.empty() && !scene_detector.descriptors.empty())
			{
				std::vector< DMatch > all_matches;

				matcher->match(scene_detector.descriptors, object_detector.descriptors, all_matches);

				float min_dist = 100, max_dist = 0;
				for (int i = 0; i < all_matches.size(); i++)
				{
					float dist = all_matches[i].distance;
					if (dist < min_dist) min_dist = dist;
					if (dist > max_dist) max_dist = dist;
				}

				matches.clear();
				for (int i = 0; i < all_matches.size(); i++)
				{
					if (all_matches[i].distance < 3 * min_dist)
					{
						matches.push_back(all_matches[i]);
					}
				}
			}
		}

		void refine_matches_estimate() {
			std::vector<cv::Point2f> object_points(matches.size());
			std::vector<cv::Point2f> scene_points(matches.size());

			for (size_t i = 0; i < matches.size(); i++)
			{
				object_points[i] = object_detector.keypoints[matches[i].trainIdx].pt;
				scene_points[i] = object_detector.keypoints[matches[i].queryIdx].pt;
			}
			cv::Mat M = cv::estimateRigidTransform(object_points, object_points, false);
			float * m = (float*)M.ptr();
			float threshold = 1;
			std::vector<cv::DMatch> inlier_matches;
			float dx, dy, delta;
			for (int i = 0; i < object_points.size(); i++) {
				dx = m[0] * object_points[i].x + m[1] * object_points[i].y + m[2] - scene_points[i].x;
				dy = m[3] * object_points[i].x + m[4] * object_points[i].y + m[5] - scene_points[i].y;
				delta = dx*dx + dy*dy;
				if (delta < threshold) {
					inlier_matches.push_back(matches[i]);
				}
			}
		}

		void refine_matches_homography()
		{
			if (homography.empty())
			{
				compute_homography();
			}
			std::vector<cv::DMatch> inlier_matches;
			for (size_t i = 0; i < inlier_mask.size(); i++)
			{
				if (inlier_mask[i])
					inlier_matches.push_back(matches[i]);
			}
			//if (inlier_matches.size() > 8)
			matches.swap(inlier_matches);
		}

		void refine_matches_fundamental()
		{
			std::vector<cv::Point2f> object_points(matches.size());
			std::vector<cv::Point2f> scene_point(matches.size());

			for (size_t i = 0; i < matches.size(); i++)
			{
				object_points[i] = object_detector.keypoints[matches[i].trainIdx].pt;
				scene_point[i] = object_detector.keypoints[matches[i].queryIdx].pt;
			}
			Mat Fundamental = findFundamentalMat(object_points, scene_point, CV_RANSAC);
			std::vector<Point3f> line1, line2;
			computeCorrespondEpilines(object_points, 1, Fundamental, line1);
			computeCorrespondEpilines(scene_point, 2, Fundamental, line2);
			float threshold = 5;
			std::vector<cv::DMatch> inlier_matches;
			for (int i = 0; i < matches.size(); i++)
			{
				float d1 = fabs(object_points[i].x* line2[i].x + object_points[i].y*line2[i].y + line2[i].z);
				float d2 = fabs(scene_point[i].x* line1[i].x + scene_point[i].y*line1[i].y + line1[i].z);
				if (d1 <= threshold && d2 <= threshold)
				{
					inlier_matches.push_back(matches[i]);
				}
			}
			matches.swap(inlier_matches);
		}

		void compute_homography()
		{
			if (!matches.empty())
			{
				std::vector<cv::Point2f> object_points(matches.size());
				std::vector<cv::Point2f> scene_points(matches.size());

				for (size_t i = 0; i < matches.size(); i++)
				{
					object_points[i] = object_detector.keypoints[matches[i].trainIdx].pt;
					scene_points[i] = scene_detector.keypoints[matches[i].queryIdx].pt;
				}
				inlier_mask.clear();
				homography = cv::findHomography(object_points, scene_points, inlier_mask, CV_RANSAC);
			}
		}

		void compute_camera_position() {
			cv::Mat Rvec;
			cv::Mat_<float> Tvec;
			cv::Mat raux, taux;
			perspectiveTransform(object_points2d, scene_points2d, homography);
			cv::solvePnP(object_points3d, scene_points2d, calibration.intrinsic_mat,
				calibration.distortion_coeff, raux, taux);
			raux.convertTo(Rvec, CV_32F);
			taux.convertTo(Tvec, CV_32F);

			cv::Mat_<float> rotMat(3, 3);
			cv::Rodrigues(Rvec, rotMat);

			extrinsic_mat = Mat(4, 4, CV_32F, cv::Scalar(0));

			//extrinsic_mat.at<float>();
		}

		std::vector<cv::Point2f>  object_points2d;
		std::vector<cv::Point3f>  object_points3d;

		std::vector<cv::Point2f>  scene_points2d;

		Mat object_image;

		Mat extrinsic_mat;

		Mat homography;

		Ptr<DescriptorMatcher> matcher;
		std::vector<DMatch> matches;
		std::vector<unsigned char> inlier_mask;

		FeaturePoints object_detector;
		FeaturePoints scene_detector;

		IntrinsicCalibrator calibration;

#ifdef CC_DEBUG_
		Mat img_matches;
		Mat img_inlier_matches;
		Mat img_keypoints_object;
		Mat img_keypoints_scene;
#endif // _DEBUG_

	};

};