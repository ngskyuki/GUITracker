#ifndef EMALGORITHM_H
#define EMALGORITHM_H

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

class EMAlgorithm
{
public:
    EMAlgorithm();
    ~EMAlgorithm();
    void setMeans(Mat means);
    Mat getMeans();
    void setWeights(Mat weights);
    Mat getWeights();
    void setLabels(Mat labels);
    Mat getLables();
    void setProbs(Mat probs);
    Mat getProbs();
    void setLogLikelihood(Mat logLikelihood);
    Mat getLogLikelihood();

    Mat vectorize(Mat& image);
    Mat observeLabelsMeans(const Mat& labels, const Mat& means, int height, int width);
    void applyEM(const Mat& src, int nMix);
private:
    Mat means;
    Mat weights;
    Mat labels;
    Mat probs;
    Mat logLikelihood;
    Mat resultImage;
    Ptr<EM> model;
};

#endif // EMALGORITHM_H
