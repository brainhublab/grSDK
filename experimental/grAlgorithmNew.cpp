#include <grAlgorithmNew.h>


GRAlgorithm::GRAlgorithm()
{

}

GRAlgorithm::~GRAlgorithm()
{

}

GRAlgorithm::GRAlgorithm(const GRAlgorithm& t)
{
    this->dtw = DTW(t.dtw);
    this->trainingData = TimeSeriesClassificationData(t.trainingData);
    this->testData = TimeSeriesClassificationData(t.testData);

    this->testAccuracy = t.testAccuracy;
}

GRAlgorithm& GRAlgorithm::operator=(const GRAlgorithm& t)
{
    this->dtw = DTW(t.dtw);
    this->trainingData = TimeSeriesClassificationData(t.trainingData);
    this->testData = TimeSeriesClassificationData(t.testData);

    this->testAccuracy = t.testAccuracy;
}

bool GRAlgorithm::initialize()
{
    return true;
}

bool GRAlgorithm::loadTrainingData(string filepath)
{
    if(!this->trainingData.load(filepath))
    {
        cout << "Failed to load training data!\n";
        return false;
    }

    return true;
}

bool GRAlgorithm::loadTestData(string filepath)
{
    if(!this->testData.load(filepath))
    {
        cout << "Failed to load test data!\n";
        return false;
    }

    return true;
}

bool GRAlgorithm::setTestDataFromTraining(int size)
{
    if(this->trainingData.getNumSamples() == 0)
    {
        cout << "No trainig data to use!\n";
        return false;
    }

    if(size < 0 || size > 100)
    {
        cout << "Incorrect size!\n";
        return false;
    }

    this->testData = this->trainingData.split(100 - size);

    return true;
}

bool GRAlgorithm::train()
{
    if(!this->dtw.train(this->trainingData))
    {
        cout << "Failed to train classifier!\n";
        return false;
    }

    return true;
}

double GRAlgorithm::test()
{
    double accuracy = 0;
    for(UINT i=0; i<this->testData.getNumSamples(); i++){
        //Get the i'th test sample - this is a timeseries
        UINT classLabel = this->testData[i].getClassLabel();
        MatrixDouble timeseries = this->testData[i].getData();

        //Perform a prediction using the classifier
        if( !this->dtw.predict( timeseries ) ){
            cout << "Failed to perform prediction for test sampel: " << i <<"\n";
            return EXIT_FAILURE;
        }

        //Get the predicted class label
        UINT predictedClassLabel = this->dtw.getPredictedClassLabel();
        double maximumLikelihood = this->dtw.getMaximumLikelihood();
        VectorDouble classLikelihoods = this->dtw.getClassLikelihoods();
        VectorDouble classDistances = this->dtw.getClassDistances();

        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
            cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << endl;
    }

    this->testAccuracy = (double(accuracy) / double(testData.getNumSamples())) * 100.0;

    cout << "Test Accuracy: " << this->testAccuracy << "%" << endl;

    return this->testAccuracy;
}

double GRAlgorithm::getTestAccuracy()
{
    return this->testAccuracy;
}

bool GRAlgorithm::saveModel(string filepath)
{
    if(!this->dtw.save(filepath))
    {
        cout << "Failed to save the classifier model!\n";
        return false;
    }

    return true;
}

bool GRAlgorithm::loadModel(string filepath)
{
    if(!this->dtw.load(filepath))
    {
        cout << "Failed to load the classifier model!\n";
        return false;
    }

    return true;
}

bool GRAlgorithm::predict(MatrixDouble timeseries)
{
    if(!dtw.predict(timeseries))
    {
        cout << "Failed to perform prediction!\n";
        return false;
    }

    return true;
}

UINT GRAlgorithm::getPredictedClassLabel()
{
    return this->dtw.getPredictedClassLabel();
}

double GRAlgorithm::getMaximumLikelihood()
{
    return this->dtw.getMaximumLikelihood();
}
