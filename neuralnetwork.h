#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

class NeuralNetwork
{
public:
    NeuralNetwork();
    ~NeuralNetwork();
    Ptr<ANN_MLP> train();

private:
    Mat trainData;
    Mat testData;
    Mat trainClasses;
    Mat testClasses;
    Mat layers;
    Ptr<ANN_MLP> mlp;
    TermCriteria criteria;
    ANN_MLP::Params params;
    ANN_MLP::Params *paramsPtr;

    int f(float x, float y, int equation);
    Mat labelData(Mat points, int equation);
};

#endif // NEURALNETWORK_H
