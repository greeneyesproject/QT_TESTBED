#ifndef INVERSE_BRISK_H_
#define INVERSE_BRISK_H_

#include <string>
#include "VisualFeatureExtraction.h"
#include <dirent.h>

#define MASK_BACKGROUND 1
#define MAX_ITERATION 50

using namespace std;
using namespace cv;

inline uchar clampValue (float val) {
	if (val > 255) return 255;
	else if (val < 0) return 0;
	else return (uchar)val;
}


class inverse_BRISK{

private:

	Mat                        db_descriptors;
	vector<KeyPoint>           db_keypoints;
	vector<Mat>                db_patches;
    VisualFeatureExtraction    *extractor;
	DescriptorMatcher          *matcher;
	Mat                        erosionKernel;

	int stitch_patch(Mat inPatch, Mat &outPatch, int numIter);

    int imW, imH;

public:

    inverse_BRISK(VisualFeatureExtraction *ext, int imWidth = 640, int imHeight = 480);
	int build_database(string path);
    int save_database();
	int load_database();
    int get_features(string inputImg, Mat &descriptors, vector<KeyPoint> &keypoints);
    Mat invert_BRISK(Mat descriptors, vector<KeyPoint> keypoints, int dist_threshold = 200);	
    Mat get_patch(int id);

	int scale_rot_patch(Mat inPatch, Mat &outPatch, Mat &mask, Mat &contour, float theta, float scale);
	void poisson_stitching(IplImage *srcImage, IplImage *destImage, IplImage *maskImage);
	void poisson_stitch(Mat srcPatch, Mat srcMask, Mat dst, Mat dstMask);

	int getDir (string dir, vector<string> &files)
	{
	    DIR *dp;
	    struct dirent *dirp;
	    if((dp  = opendir(dir.c_str())) == NULL) {
	        cout << "Error opening " << dir << endl;
	        return -1;
	    }

	    int fcount = 0;
	    while ((dirp = readdir(dp)) != NULL) {
	    	if(!(strcmp(dirp->d_name,".")==0 | strcmp(dirp->d_name,"..")==0) )
	    		files.push_back(string(dirp->d_name));
	    	fcount++;
	    }
	    closedir(dp);
	    return 0;
	}

};


struct KeyPoint_idx{
	KeyPoint keypoint;
	int      index;

	KeyPoint_idx(KeyPoint kp, int idx){
		keypoint = kp;
		index = idx;
	}
};

#endif /* INVERSE_BRISK_H_ */
