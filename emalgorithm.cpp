#include "emalgorithm.h"

EMAlgorithm::EMAlgorithm()
{

}

EMAlgorithm::~EMAlgorithm()
{

}

void EMAlgorithm::setMeans(Mat means)
{
    this->means = means;
}
Mat EMAlgorithm::getMeans()
{
    return this->means;
}
void EMAlgorithm::setWeights(Mat weights)
{
    this->weights = weights;
}
Mat EMAlgorithm::getWeights()
{
    return this->weights;
}
void EMAlgorithm::setProbs(Mat probs)
{
    this->probs = probs;
}
Mat EMAlgorithm::getProbs()
{
    return this->probs;
}
void EMAlgorithm::setLabels(Mat labels)
{
    this->labels = labels;
}
Mat EMAlgorithm::getLables()
{
    return this->labels;
}
void EMAlgorithm::setLogLikelihood(Mat logLikelihood)
{
    this->logLikelihood = logLikelihood;
}
Mat EMAlgorithm::getLogLikelihood()
{
    return this->logLikelihood;
}

void EMAlgorithm::applyEM(const Mat& src, int nMix)
{
    Mat image;
    src.copyTo(image);

    const int imageRows = image.rows;
    const int imageCols = image.cols;

    Mat reshapedImage = image.reshape(1, imageRows * imageCols);
    Mat samples;
    reshapedImage.convertTo(samples, CV_64FC1, 1.0 / 255.0);

    EM::Params params;
    params.covMatType = EM::COV_MAT_DEFAULT;
    params.nclusters = nMix;

    Ptr<EM> model = EM::train(samples, this->logLikelihood, this->labels, this->probs, params);
    this->model = model;
    this->means = model->getMeans();
    this->weights = model->getWeights();

    this->resultImage = observeLabelsMeans(this->labels, this->means, imageRows, imageCols);
}

Mat EMAlgorithm::observeLabelsMeans(const Mat& labels, const Mat& means, int height, int width)
{
    constexpr int dimentions = 3;
    Mat result(height, width, CV_8UC3);
    MatIterator_<Vec3b> resBegin = result.begin<Vec3b>();
    MatIterator_<Vec3b> resEnd = result.end<Vec3b>();
    MatConstIterator_<int> labelBegin = labels.begin<int>();

    Mat meansU8;
    means.convertTo(meansU8, CV_8UC1, 255.0);
    Mat meansU8C3 = meansU8.reshape(dimentions);
    while(resBegin != resEnd)
    {
        const Vec3b rgb = meansU8C3.ptr<Vec3b>(*labelBegin)[0];
        *resBegin = rgb;
        ++resBegin;
        ++labelBegin;
    }
    return result;
}

Mat vectorize(Mat& img)
{
    Mat floatImg;
    img.convertTo(floatImg, CV_32F);
    Mat samples(img.rows * img.cols, 3, CV_32FC1);
    int index = 0;
    for(int y = 0; y < img.rows; y++)
    {
        Vec3f *row = floatImg.ptr<Vec3f>(y);
        for(int x = 0; x < img.cols; x++)
        {
            samples.at<Vec3f>(index++, 0) = row[x];
        }
    }
    return samples;
}

/*
void classify(Mat& src, Mat& dst)
{
    Mat tmpImg;
    src.copyTo(tmpImg);

    map<int, Vec3b> colors = new map<int, Vec3b>();
    for(int y = 0; y < tmpImg.rows; y++)
    {
        for(int x = 0; x < tmpImg.cols; x++)
        {
            Mat sample = Mat(1, 0, CV_32FC3);
            sample.at<float>(0) = tmpImg<float>(y, x);
            int res = this.model->predict2(sample, noArray()[1]);
            tmpImg.at<float>(y, x) = colors.operator [res];
        }
    }
}
vector<Color> createColors(const Mat means)
{
    vector<Color> colors;
    for(int y = 0; y < means.rows; y++)
    {
        for(int x = 0; x < means.cols; x++)
        {
            Color tmp = means.at<Vec3b>(y, x);
            colors.push_back(tmp);
        }
    }
    return colors;
}
*/
