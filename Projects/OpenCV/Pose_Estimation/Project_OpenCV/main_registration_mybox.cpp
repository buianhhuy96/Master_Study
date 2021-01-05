// C++
#include <iostream>
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
// PnP Tutorial
#include "Mesh.h"
#include "Model.h"
#include "PnPProblem.h"
#include "RobustMatcher.h"
#include "ModelRegistration.h"
#include "Utils.h"

using namespace cv;
using namespace std;

/**  GLOBAL VARIABLES  **/
string img_path = "Data/resize_mybox.jpg";
string ply_read_path = "Data/mybox.ply";
string write_path = "Data/mybox.yml";
bool end_registration = false;

// Intrinsic camera parameters
double f = 5;                           // focal length in mm
double sx = 6.17, sy = 4.56;             // sensor size
double width = 640, height = 360;        // image size
double params_camera[] =
{ width*f / sx,    // fx
height*f / sy,    // fy
width / 2,      // cx
height / 2 };   // cy
        // Setup the points to register in the image
        // In the order of the *.ply file and starting at 1
int n = 8;
int pts[] = { 1, 2, 3, 4, 5, 6, 7, 8 }; // 3 -> 4
                    // Some basic colors
Scalar red(0, 0, 255);
Scalar green(0, 255, 0);
Scalar blue(255, 0, 0);
Scalar yellow(0, 255, 255);

/*
* CREATE MODEL REGISTRATION OBJECT
* CREATE OBJECT MESH
* CREATE OBJECT MODEL
* CREATE PNP OBJECT
*/
ModelRegistration registration;
Model model;
Mesh mesh;
PnPProblem pnp_registration(params_camera);

// Mouse events for model registration
static void onMouseModelRegistration(int event, int x, int y, int, void*)
{
  if (event == EVENT_LBUTTONUP)
  {
    int n_regist = registration.getNumRegist();
    int n_vertex = pts[n_regist];

    Point2f point_2d = Point2f((float)x, (float)y);
    Point3f point_3d = mesh.getVertex(n_vertex - 1);

    bool is_registrable = registration.is_registrable();
    if (is_registrable)
    {
      registration.registerPoint(point_2d, point_3d);
      if (registration.getNumRegist() == registration.getNumMax()) end_registration = true;
    }
  }
}

/**  Main program  **/
int main()
{
  /** Step 1: Load Mesh and setup Robust Matcher **/

  /*Load a mesh given the *.ply file path*/
  mesh.load(ply_read_path);

  /*setup Robust Matcher: detector, extractor, matcher*/
  RobustMatcher rmatcher;
  int numKeyPoints = 5000;
  Ptr<FeatureDetector> detector = ORB::create(numKeyPoints);
  rmatcher.setFeatureDetector(detector);
  rmatcher.setDescriptorExtractor(detector);

  /** Step 2: Point registration **/
  /*setup*/
  namedWindow("MODEL REGISTRATION", WINDOW_KEEPRATIO);
  setMouseCallback("MODEL REGISTRATION", onMouseModelRegistration, 0); /*Set up the mouse events*/
  Mat img_in = imread(img_path, IMREAD_COLOR);
  Mat img_vis = img_in.clone();
  if (!img_in.data) {
    cout << "Could not open or find the image" << endl;
    return -1;
  }
  cout << "Click the box corners ..." << endl;
  cout << "Waiting ..." << endl;

  /*Point registration*/
  int num_registrations = n; /*Set the number of points to register*/
  registration.setNumMax(num_registrations);
  while (waitKey(30) < 0)
  {
    img_vis = img_in.clone(); /*Refresh debug image*/

                  // Current registered points
    vector<Point2f> list_points2d = registration.get_points2d();
    vector<Point3f> list_points3d = registration.get_points3d();

    // Draw current registered points
    drawPoints(img_vis, list_points2d, list_points3d, red);

    // If the registration is not finished, draw which 3D point we have to register.
    // If the registration is finished, breaks the loop.
    if (!end_registration)
    {
      /*draw debug text*/
      int n_regist = registration.getNumRegist();
      int n_vertex = pts[n_regist];
      Point3f current_poin3d = mesh.getVertex(n_vertex - 1);
      drawQuestion(img_vis, current_poin3d, green);
      drawCounter(img_vis, registration.getNumRegist(), registration.getNumMax(), red);
    }
    else
    {
      /*draw debug text*/
      drawText(img_vis, "END REGISTRATION", green);
      drawCounter(img_vis, registration.getNumRegist(), registration.getNumMax(), green);
      break;
    }

    // Show the image
    imshow("MODEL REGISTRATION", img_vis);
  }

  /** Step 3: Pose Estimate **/
  /*setup*/
  cout << "COMPUTING POSE ..." << endl;
  vector<Point2f> list_points2d = registration.get_points2d();
  vector<Point3f> list_points3d = registration.get_points3d();

  /*Estimate pose given the registered points*/
  bool is_correspondence = pnp_registration.estimatePose(list_points3d, list_points2d, SOLVEPNP_ITERATIVE);
  if (is_correspondence)
  {
    cout << "Correspondence found" << endl;
    // Compute all the 2D points of the mesh to verify the algorithm and draw it
    vector<Point2f> list_points2d_mesh = pnp_registration.verify_points(&mesh);
    draw2DPoints(img_vis, list_points2d_mesh, green);
    drawObjectMesh(img_vis, &mesh, &pnp_registration, blue);
  }
  else {
    cout << "Correspondence not found" << endl << endl;
  }

  /*show the image*/
  imshow("MODEL REGISTRATION", img_vis);
  waitKey(0);

  /** Step 4: Keypoint detectoion & Descriptors extraction, Inlier Outlier check **/

  /*setup*/
  vector<KeyPoint> keypoints_model;
  Mat descriptors;

  /*Keypoint detectoion & Descriptors extraction*/
  rmatcher.computeKeyPoints(img_in, keypoints_model);
  rmatcher.computeDescriptors(img_in, keypoints_model, descriptors);

  /*Inlier Outlier check*/
  for (unsigned int i = 0; i < keypoints_model.size(); ++i) {
    Point2f point2d(keypoints_model[i].pt);
    Point3f point3d;
    bool on_surface = pnp_registration.backproject2DPoint(&mesh, point2d, point3d);
    if (on_surface)
    {
      model.add_correspondence(point2d, point3d);
      model.add_descriptor(descriptors.row(i));
      model.add_keypoint(keypoints_model[i]);
    }
    else
    {
      model.add_outlier(point2d);
    }
  }

  /** Step 5: Save model, show info **/

  /*Save model*/
  model.save(write_path);

  /*show info*/
  img_vis = img_in.clone();
  vector<Point2f> list_points_in = model.get_points2d_in();
  vector<Point2f> list_points_out = model.get_points2d_out();
  string num = IntToString((int)list_points_in.size());
  string text = "There are " + num + " inliers";
  drawText(img_vis, text, green);
  num = IntToString((int)list_points_out.size());
  text = "There are " + num + " outliers";
  drawText2(img_vis, text, red);
  drawObjectMesh(img_vis, &mesh, &pnp_registration, blue);
  draw2DPoints(img_vis, list_points_in, green);
  draw2DPoints(img_vis, list_points_out, red);
  imshow("MODEL REGISTRATION", img_vis);
  waitKey(0);

  // Close and Destroy Window
  destroyWindow("MODEL REGISTRATION");
  cout << "GOODBYE" << endl;
}

