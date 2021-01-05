// C++
#include <iostream>
#include <time.h>
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/video/tracking.hpp>
// PnP Tutorial
#include "Mesh.h"
#include "Model.h"
#include "PnPProblem.h"
#include "ModelRegistration.h"
#include "Utils.h"

/**  GLOBAL VARIABLES  **/

using namespace cv;
using namespace std;

string yml_read_path = "Data/mybox.yml";			// 3D Keypoints + Descriptors
string ply_read_path = "Data/mybox.ply";			// Mesh

/*define*/
Scalar red(0, 0, 255);
Scalar green(0, 255, 0);
Scalar blue(255, 0, 0);
Scalar yellow(0, 255, 255);

// Intrinsic camera parameters
double f = 5;                           // focal length in mm
double sx = 6.17, sy = 4.56;             // sensor size
double width = 640, height = 480;        // image size
double params_camera[] = { width*f / sx, height*f / sy, width / 2, height / 2 };   // fx,fy,cx,cy
PnPProblem pnp_detection(params_camera);

// Model variables
Mesh mesh;                 
Model model;
vector<Point3f> list_points3d_model;
Mat descriptors_model;

// Scene variables
vector<KeyPoint> keypoints_scene;	// to obtain the 2D points of the scene
Mat descriptors_scene;

// Matcher variables
vector<vector<DMatch>> matches;
vector<DMatch>	 good_matches;
vector<Point3f> list_points3d_model_match; 
vector<Point2f> list_points2d_scene_match; 

// PnP RANSAC variables
Mat inliers_idx;
vector<Point2f> list_points2d_inliers;

// Detector parameters
int numKeyPoints = 5000;      // number of detected keypoints
Ptr<FeatureDetector> detector = ORB::create(numKeyPoints);

// Matcher parameters
Ptr<flann::IndexParams> indexParams = makePtr<flann::LshIndexParams>(6, 12, 1); // instantiate LSH index parameters
Ptr<flann::SearchParams> searchParams = makePtr<flann::SearchParams>(50);       // instantiate flann search parameters																	
Ptr<DescriptorMatcher> matcher = makePtr<FlannBasedMatcher>(indexParams, searchParams);// instantiate FlannBased matcher
int k = 4;
float ratioTest = 0.70f;      // ratio test
int good_threshold = 15; 
bool good_measurement = false;

// PnP RANSAC parameters
int pnpMethod = SOLVEPNP_ITERATIVE;
int iterationsCount = 100;      // number of Ransac iterations.
float reprojectionError = 3.0;  // maximum allowed distance to consider it an inlier.
double confidence = 0.95;        // ransac successful confidence.

/*Function header*/

int main(int argc, char *argv[])
{
/** Step 0: Load Model and Mesh **/
	mesh.load(ply_read_path);  
	model.load(yml_read_path);
	list_points3d_model = model.get_points3d();
	descriptors_model = model.get_descriptors();

	namedWindow("Object Detection", WINDOW_KEEPRATIO);
	VideoCapture cap;
	cap.open(0);
	Mat frame, frame_vis;

	while (cap.read(frame) && (char)waitKey(30) != 27)
	{
		frame_vis = frame.clone();
		matches.clear();
		good_matches.clear();

		/*Scene Keypoints Detection & Descriptors Extraction*/
		detector->detect(frame, keypoints_scene);
		detector->compute(frame, keypoints_scene, descriptors_scene);

		/* Matching descriptors between model and scene*/
		matcher->knnMatch(descriptors_scene, descriptors_model, matches, k);
		for (vector<vector<DMatch> >::iterator matchIterator = matches.begin(); matchIterator != matches.end(); ++matchIterator)
		{
			// if 2 NN has been identified
			if (matchIterator->size() > 1)
			{
				// check distance ratio
				if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratioTest)
				{
					matchIterator->clear(); // remove match
				}
			}
			else
			{ // does not have 2 neighbours
				matchIterator->clear(); // remove match
			}
		}
		for (vector<vector<DMatch>>::iterator matchIterator = matches.begin(); matchIterator != matches.end(); ++matchIterator)
		{
			if (!matchIterator->empty()) good_matches.push_back((*matchIterator)[0]);
		}

		if (good_matches.size() > 10)  /*RANSAC crash if good matches = 0*/
		{

			/**Step 2: Find out the 2D/3D correspondences**/
			list_points3d_model_match.clear();
			list_points2d_scene_match.clear();
			for (unsigned int match_index = 0; match_index < good_matches.size(); ++match_index)
			{
				Point3f point3d_model = list_points3d_model[good_matches[match_index].trainIdx];	// 3D point from model
				Point2f point2d_scene = keypoints_scene[good_matches[match_index].queryIdx].pt;		// 2D point from the scene
				list_points3d_model_match.push_back(point3d_model);         // add 3D point
				list_points2d_scene_match.push_back(point2d_scene);         // add 2D point
			}

			/** Step 3: Estimate the pose using RANSAC approach **/
			pnp_detection.estimatePoseRANSAC(list_points3d_model_match, list_points2d_scene_match, pnpMethod, inliers_idx, iterationsCount, reprojectionError, confidence);
			
			/** Step 4: Catch the inliers keypoints to draw **/
			for (int inliers_index = 0; inliers_index < inliers_idx.rows; ++inliers_index)
			{
				int n = inliers_idx.at<int>(inliers_index);         // i-inlier
				circle(frame_vis, list_points2d_scene_match[n], 3, green, 1);
			}

			/** Step 5: Check measurement**/
			/*Check measurement*/
			if (inliers_idx.rows >= good_threshold)
			{
				good_measurement = true;
				/*Get the measured translation & measured rotation*/
				Mat translation_measured(3, 1, CV_64F);
				Mat rotation_measured(3, 3, CV_64F);
				//translation_measured = pnp_detection.get_t_matrix();
				//rotation_measured = pnp_detection.get_R_matrix();
			}

			/** Step 7: Draw pose **/
			double l = 5;
			vector<Point2f> pose_points2d;
			if (good_measurement)
			{
				good_measurement = false;
				drawObjectMesh(frame_vis, &mesh, &pnp_detection, blue);  // draw pose estimated by RANSAC
				pose_points2d.push_back(pnp_detection.backproject3DPoint(Point3f(0, 0, 0)));  // axis center
				pose_points2d.push_back(pnp_detection.backproject3DPoint(Point3f(l, 0, 0)));  // axis x
				pose_points2d.push_back(pnp_detection.backproject3DPoint(Point3f(0, l, 0)));  // axis y
				pose_points2d.push_back(pnp_detection.backproject3DPoint(Point3f(0, 0, l)));  // axis z
				draw3DCoordinateAxes(frame_vis, pose_points2d);
			}

			/** Step X: show info **/
			string text = "Inliers: " + IntToString(inliers_idx.rows) + " of " + IntToString((int)good_matches.size()) + " matches" ;
			drawText(frame_vis, text, green);
			imshow("Object Detection", frame_vis);
		}
		else	imshow("Object Detection", frame);
	}

	destroyWindow("Object Detection");
	cout << "GOODBYE ..." << endl;
}


