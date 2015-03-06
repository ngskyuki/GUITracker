#ifndef ANN_H
#define ANN_H

#include <opencv2/ml/ml.hpp>
#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::ml;

class ANN
{
public:
    ANN();
    ~ANN();

    void showPlot(Mat testData);
    bool buildMlpClassifier(const string& dataFileName,
                            const string& exFileName,
                            const string& srcFileName);
    static Ptr<TrainData> prepareTrainData(const Mat& data, const Mat& responses, int nTrainSamples);
private:
    Mat prediction;
    bool readNumClassData(const string& fileName,
                          int varCount,
                          Mat* _data,
                          Mat* _responses);

    void testSaveClassifier(const Ptr<StatModel> model,
                            const Mat& data,
                            const Mat& responses,
                            int nTrainSamples,
                            int rDelta,
                            const string& exFileName);

    template<typename T>
    Ptr<T> loadClassifier(const string& srcFileName);
};

#endif // ANN_H
