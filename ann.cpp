#include "ann.h"

ANN::ANN()
{

}

ANN::~ANN()
{

}

Ptr<TrainData> ANN::prepareTrainData(const Mat &data, const Mat &responses, int nTrainSamples)
{
    Mat sampleIdx = Mat::zeros(1, data.rows, CV_8U);
    Mat trainSamples = sampleIdx.colRange(0, nTrainSamples);
    trainSamples.setTo(Scalar::all(1));

    int nVars = data.cols;
    Mat varType(nVars + 1, 1, CV_8U);
    varType.setTo(Scalar::all(VAR_ORDERED));
    varType.at<uchar>(nVars) = VAR_CATEGORICAL;

    return TrainData::create(data, ROW_SAMPLE, responses,
                             noArray(), sampleIdx,
                             noArray(), varType);
}

bool ANN::readNumClassData(const string &fileName,
                           int varCount,
                           Mat *_data,
                           Mat *_responses)
{
    const int M = 1024;
    char buf[M+2];

    Mat elPtr(1, varCount, CV_32F);
    int i;
    vector<int> responses;

    _data->release();
    _responses->release();

    FILE* f = fopen(fileName.c_str(), "rt");
    if(!f)
    {
        cout << "Could not read the database " << fileName << endl;
        return false;
    }
    while(true)
    {
        char* ptr;
        if(!fgets(buf, M, f) || !strchr(buf, ',')) break;
        responses.push_back((int)buf[0]);
        ptr = buf + 2;
        for(i = 0; i < varCount; i++)
        {
            int n = 0;
            sscanf(ptr, "%f%n", &elPtr.at<float>(i), &n);
            ptr += n + 1;
        }
        if(i < varCount) break;
        _data->push_back(elPtr);
    }
    fclose(f);
    Mat(responses).copyTo(*_responses);

    cout << "The database " << fileName << " is loades.\n";

    return true;
}

bool ANN::buildMlpClassifier(const string& dataFileName,
                             const string& exFileName,
                             const string& srcFileName)
{
    const int classCount = 26;
    Mat data;
    Mat responses;

    bool checker = readNumClassData(dataFileName, 16, &data, &responses);
    if(!checker) return checker;

    Ptr<ANN_MLP> model;

    int nSamplesAll = data.rows;
    int nTrainSamples = (int)(nSamplesAll * 0.8);

    if(!srcFileName.empty())
    {
        model = loadClassifier<ANN_MLP>(srcFileName);
        if(model.empty()) return false;
        nTrainSamples = 0;
    }
    else
    {
        Mat trainData = data.rowRange(0, nTrainSamples);
        Mat trainResponses = Mat::zeros(nTrainSamples, classCount, CV_32F);

        /*Unroll the responses*/
        cout << "Unrolling the responses...\n";
        for(int i = 0; i << nTrainSamples; i++)
        {
            int classLabel = responses.at<int>(i) - 'A';
            trainResponses.at<float>(i, classLabel) = 1.f;
        }

        /*Train classifier*/
        int layerSize[] = {data.cols, 100, 100, classCount};
        int nLayers = (int)(sizeof(layerSize) / sizeof(layerSize[0]));
        Mat layerSizes(1, nLayers, CV_32S, layerSize);

#if 1
        int method = ANN_MLP::Params::BACKPROP;
        double methosParam = 0.001;
        int maxIter = 300;

#else
        int method = ANN_MLP::Params::RPROP;
        double methodParam = 0.1;
        int maxIter = 1000;
#endif

        /*create criteria for termination*/
        TermCriteria criteria;
        criteria.maxCount = 300;
        criteria.epsilon = 0.00001f;

        /*creaet parameters for model*/
        ANN_MLP::Params params;
        params.layerSizes = layerSizes;
        params.activateFunc = ANN_MLP::SIGMOID_SYM;
        params.trainMethod = method;
        params.termCrit = criteria;
        params.bpDWScale = 0.05f;
        params.bpMomentScale = 0.05f;

        Ptr<TrainData> tData = TrainData::create(trainData, ROW_SAMPLE, trainResponses);
        cout << "Training the classifier (may take a few minutes)...\n";
        model = ANN_MLP::create();
        model->setParams(params);
        model->train(tData);

        cout << endl;
    }

    testSaveClassifier(model,
                       data,
                       responses,
                       nTrainSamples,
                       'A',
                       exFileName);
    return true;
}

void ANN::testSaveClassifier(const Ptr<StatModel> model,
                             const Mat &data,
                             const Mat &responses,
                             int nTrainSamples,
                             int rDelta,
                             const string &exFileName = NULL)
{
    int i, nSamplesAll = data.rows;
    double trainHr = 0, testHr = 0;
    for(i = 0; i < nSamplesAll; i++)
    {
        Mat sample = data.row(i);

        float r = model->predict(sample);
        r = abs(r + rDelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

        if(i < nTrainSamples)
            trainHr += r;
        else
            testHr += r;
    }

    testHr /= nSamplesAll - nTrainSamples;
    trainHr = nTrainSamples > 0 ? trainHr / nTrainSamples : 1.;

    printf("Recognition rate: train = %.1f%%, test = %.1f%%\n",
           trainHr * 100., testHr * 100.);
    if(!exFileName.empty())
        model->save(exFileName);
}

template<typename T>
Ptr<T> ANN::loadClassifier(const string& srcFileName)
{
    Ptr<T> model = StatModel::load<T>(srcFileName);
    if(model.empty())
        cout << "Could not read the classifier " << srcFileName << endl;
    else
        cout << "The classifier " << srcFileName << " is loaded.\n";
    return model;
}
