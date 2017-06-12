#include <grGrt.h>

GRGrt::GRGrt()
{

}

GRGrt::~GRGrt()
{

}

GRGrt::GRGrt(const GRGrt &t)
{
	this->dtw = GRT::DTW(t.dtw);
    this->trainingData = GRT::TimeSeriesClassificationData(t.trainingData);
    this->testData = GRT::TimeSeriesClassificationData(t.testData);

    this->testAccuracy = t.testAccuracy;
}

GRGrt& GRGrt::operator=(const GRGrt& t)
{
	this->dtw = GRT::DTW(t.dtw);
    this->trainingData = GRT::TimeSeriesClassificationData(t.trainingData);
    this->testData = GRT::TimeSeriesClassificationData(t.testData);

    this->testAccuracy = t.testAccuracy;

}
bool GRGrt::loadTrainingData(std::string filepath)
{
    if(!this->trainingData.load(filepath))
    {
        std::cout << "Failed to load training data!\n";
        return false;
    }

    return true;
}

bool GRGrt::loadTestData(std::string filepath)
{
    if(!this->testData.load(filepath))
    {
        std::cout << "Failed to load test data!\n";
        return false;
    }

    return true;
}

bool GRGrt::setTestDataFromTraining(int size)
{
    if(this->trainingData.getNumSamples() == 0)
    {
        std::cout << "No trainig data to use!\n";
        return false;
    }

    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!\n";
        return false;
    }

    this->testData = this->trainingData.split(100 - size);

    return true;
}

bool GRGrt::train()
{
    if(!this->dtw.train(this->trainingData))
    {
        std::cout << "Failed to train classifier!\n";
        return false;
    }

    return true;
}

double GRGrt::test()
{
    double accuracy = 0;
    for(GRT::UINT i=0; i<this->testData.getNumSamples(); i++){
        //Get the i'th test sample - this is a timeseries
        GRT::UINT classLabel = this->testData[i].getClassLabel();
        GRT::MatrixDouble timeseries = this->testData[i].getData();

        //Perform a prediction using the classifier
        if( !this->dtw.predict( timeseries ) ){
            std::cout << "Failed to perform prediction for test sampel: " << i <<"\n";
            return EXIT_FAILURE;
        }

        //Get the predicted class label
        GRT::UINT predictedClassLabel = this->dtw.getPredictedClassLabel();
        double maximumLikelihood = this->dtw.getMaximumLikelihood();
        GRT::VectorDouble classLikelihoods = this->dtw.getClassLikelihoods();
        GRT::VectorDouble classDistances = this->dtw.getClassDistances();

        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
        std::cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << std::endl;
    }

    this->testAccuracy = (double(accuracy) / double(testData.getNumSamples())) * 100.0;

    std::cout << "Test Accuracy: " << this->testAccuracy << "%" << std::endl;

    return this->testAccuracy;
}

double GRGrt::getTestAccuracy()
{
    return this->testAccuracy;
}

bool GRGrt::saveModel(std::string filepath)
{
    if(!this->dtw.save(filepath))
    {
        std::cout << "Failed to save the classifier model!\n";
        return false;
    }

    return true;
}

bool GRGrt::loadModel(std::string filepath)
{
    if(!this->dtw.load(filepath))
    {
        std::cout << "Failed to load the classifier model!\n";
        return false;
    }

    return true;
}

bool GRGrt::predict(GRT::MatrixDouble timeseries)
{
    if(!dtw.predict(timeseries))
    {
        std::cout << "Failed to perform prediction!\n";
        return false;
    }

    return true;
}

GRT::UINT GRGrt::getPredictedClassLabel()
{
    return this->dtw.getPredictedClassLabel();
}

double GRGrt::getMaximumLikelihood()
{
    return this->dtw.getMaximumLikelihood();
}

