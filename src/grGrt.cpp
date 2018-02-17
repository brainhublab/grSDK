#include <grGrt.h>
/*constructor
*/
GRGrt::GRGrt()
{

    //utils
    _gestureLabel = 1;
    //datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);
    //datasetDTW.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    //datasetDTW.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text
    _dtw.enableNullRejection(true);

}
/*destructor
*/
GRGrt::~GRGrt()
{

}

/*copy constructor
*/
GRGrt::GRGrt(const GRGrt &t)
{
    this->_dtw = GRT::DTW(t._dtw);
    this->_trainingData = GRT::TimeSeriesClassificationData(t._trainingData);
    this->_testData = GRT::TimeSeriesClassificationData(t._testData);

    this->_testAccuracy = t._testAccuracy;
}

/*operator =
*/
GRGrt& GRGrt::operator=(const GRGrt& t)
{
    this->_dtw = GRT::DTW(t._dtw);
    this->_trainingData = GRT::TimeSeriesClassificationData(t._trainingData);
    this->_testData = GRT::TimeSeriesClassificationData(t._testData);

    this->_testAccuracy = t._testAccuracy;

}

/*loading of training data from filepath
 * training data structor is used from GRT standarts //TODO later need to be from database
 */
bool GRGrt::loadTrainingData(std::string filepath)
{
    if(!this->_trainingData.load(filepath))
    {
        std::cout << "Failed to load training data!\n";
        return false;
    }

    return true;
}

/*loading test data from filepath
*/
bool GRGrt::loadTestData(std::string filepath)
{
    if(!this->_testData.load(filepath))
    {
        std::cout << "Failed to load test data!\n";
        return false;
    }

    return true;
}

/* setting of test data with providing of size of dataset 
*/
bool GRGrt::setTestDataFromTraining(int size)
{
    if(this->_trainingData.getNumSamples() == 0)
    {
        std::cout << "No trainig data to use!\n";
        return false;
    }

    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!\n";
        return false;
    }

    this->_testData = this->_trainingData.split(100 - size);

    return true;
}

/* training 
*/
bool GRGrt::train()
{
    if(!this->_dtw.train(this->_trainingData))
    {
        std::cout << "Failed to train classifier!\n";
        return false;
    }

    return true;
}

/*testing of trained data
*/
double GRGrt::test()
{
    double accuracy = 0;
    for(GRT::UINT i=0; i<this->_testData.getNumSamples(); i++){
        //Get the i'th test sample - this is a timeseries
        GRT::UINT classLabel = this->_testData[i].getClassLabel();
        GRT::MatrixDouble timeseries = this->_testData[i].getData();

        //Perform a prediction using the classifier
        if( !this->_dtw.predict( timeseries ) ){
            std::cout << "Failed to perform prediction for test sampel: " << i <<"\n";
            return EXIT_FAILURE;
        }

        //Get the predicted class label
        GRT::UINT predictedClassLabel = this->_dtw.getPredictedClassLabel();
        double maximumLikelihood = this->_dtw.getMaximumLikelihood();
        GRT::VectorDouble classLikelihoods = this->_dtw.getClassLikelihoods();
        GRT::VectorDouble classDistances = this->_dtw.getClassDistances();

        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
        std::cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << std::endl;
    }

    this->_testAccuracy = (double(accuracy) / double(_testData.getNumSamples())) * 100.0;

    std::cout << "Test Accuracy: " << this->_testAccuracy << "%" << std::endl;

    return this->_testAccuracy;
}

/*get test acuracy
*/
double GRGrt::getTestAccuracy()
{
    return this->_testAccuracy;
}

/*save trained model in filepath 
*/
bool GRGrt::saveModel(std::string filepath)
{
    if(!this->_dtw.save(filepath))
    {
        std::cout << "Failed to save the classifier model!\n";
        return false;
    }

    return true;
}

/*load trained model from filepath
*/
bool GRGrt::loadModel(std::string filepath)
{
    if(!this->_dtw.load(filepath))
    {
        std::cout << "Failed to load the classifier model!\n";
        return false;
    }

    return true;
}

/*prediction
*/
bool GRGrt::predict(GRT::MatrixDouble timeseries)
{
    if(!_dtw.predict(timeseries))
    {
        std::cout << "Failed to perform prediction!\n";
        return false;
    }

    return true;
}

/*getting of label for prediction data
*/
GRT::UINT GRGrt::getPredictedClassLabel()
{
    return this->_dtw.getPredictedClassLabel();
}

/*needet for GRT
*/
double GRGrt::getMaximumLikelihood()
{
    return this->_dtw.getMaximumLikelihood();
}

/*setting dataset sensors datastructures
*/

/*properties for dataset
*/
void GRGrt::setDatasetProperties(std::string dataSetName, std::string infoText, std::string fProp, int dimIn )
{
    _dimensions = dimIn; 
    _trainingData.setNumDimensions(_dimensions);
    _trainingData.setDatasetName(dataSetName);
    _trainingData.setInfoText(infoText);
    _fileProp = fProp + ".grt";

}

/*set nex label in training data
*/
void GRGrt::setNextLabel()
{
    _gestureLabel ++;
}

/*clearing
*/
void GRGrt::clearTrainingData()
{
    _trainingData.clear();

}

bool GRGrt::addSample(std::vector<double>* trajIn, std::vector<double>* quantIn)
{
    GRT::VectorDouble sample;

    sample.resize(this->_dimensions);
    for(int i=0; i < (trajIn->size() + quantIn->size()); i++)
    {
        if(i<trajIn->size())
        {
            sample[i] = trajIn->at(i);
        }
        else
        {
            sample[i] = quantIn->at(i-3);
        }    
    } 
    _gestureSample.push_back(sample);
    return true;
}
/*pushing dataset in DTW algorithm
*/
bool GRGrt::pushGesture()
{
    _trainingData.addSample(_gestureLabel, _gestureSample);
    _gestureSample.clear();
}

/*saving dataset
*/
bool GRGrt::saveDataset()
{
        if(!_trainingData.save(("../trainingData/grTrainingDTW"+_fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file \n";
            return EXIT_FAILURE;
        }
        //datasetDTW.trainingData.clear();TODO
}


#include <nbind/nbind.h>
NBIND_CLASS(GRGrt) {
    construct<>();
    construct<GRGrt>();
}

