#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork()
{
    this->criteria.maxCount = 100;
    this->criteria.epsilon = 0.00001f;
    this->criteria.type = TermCriteria::COUNT | TermCriteria::EPS;

    this->params.trainMethod = ANN_MLP::Params::RPROP;
    this->params.termCrit = criteria;
    this->params.rpDW0 = 0.1;
    this->params.rpDWMax = 50;
    this->params.rpDWMin = FLT_EPSILON;
    this->params.rpDWMinus = 0.5;
    this->params.rpDWPlus = 1.2;
    this->params.activateFunc = ANN_MLP::SIGMOID_SYM;
    this->params.fparam1 = 0;
    this->params.fparam2 = 0;
    this->paramsPtr = &this->params;
    this->layers = Mat(4, 1, CV_32SC1);
    this->layers.row(0) = Scalar(2);
    this->layers.row(1) = Scalar(10);
    this->layers.row(2) = Scalar(15);
    this->layers.row(3) = Scalar(1);

    this->mlp->create(this->params);
}

NeuralNetwork::~NeuralNetwork()
{

}

Ptr<ANN_MLP> NeuralNetwork::train()
{
    //return this->mlp->train(this->trainData, ROW_SAMPLE, this->trainClasses, this->paramsPtr, ANN_MLP::Params::RPROP);
    //return this->mlp->train(this->trainData, this->trainClasses, NULL, NULL, this->paramsPtr);
    Ptr<ANN_MLP> bk;
    return bk;
}

int NeuralNetwork::f(float x, float y, int equation)
{
    switch(equation)
    {
    case 0:
        return y > sin(x * 10) ? -1 : 1;
        break;
    case 1:
        return y > cos(x * 10) ?  -1 : 1;
        break;
    case 2:
        return y > 2 * x ? -1 : 1;
        break;
    case 3:
        return y > tan(x * 10) ? -1 : 1;
        break;
    default:
        return y > cos(x * 10) ? -1 : 1;
    }
}
