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

    constexpr int dimentions = 3;

    Mat reshapedImage = image.reshape(1, imageRows * imageCols);
    Mat samples;
    reshapedImage.convetTo(samples, CV_64FC1, 1.0 / 255.0);

    EM::Params params;
    params.covMatType = EM::COV_MAT_DEFAULT;
    params.nclusters = nMix;

    Ptr<EM> model = EM::train(samples, this->logLikelihood, this->labels, this->probs, params);
    this->model = model;
    this->means = model->getMeans();
    this->weights = model->getWeights();

    this->resultImage = observeLabelMeans(this->labels, this->means, imageRows, imageCols);
}

Mat EMAlgorithm::observeLabelsMeans(const Mat& labels, const Mat& means, int height, int width)
{
    Mat result(height, width, CV_8UC3);
    MatIterator_<Vec3b> resBegin = result.begin<Vec3b>();
    MatIterator_<Vec3> resEnd = result.end<Vec3b>();
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
