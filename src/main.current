#include "GRT/GRT.h"
#include "grAlgorithm.h"
#include "grGrt.h"
using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{
    GRGrt grt;

    grt.loadTrainingData("../trainingData/grTrainingDTWtest.grt");
    grt.setTestDataFromTraining(80);

    //grAlg.loadModel("./data/DTWModel.grt");

    grt.train();
    grt.test();

    grt.saveModel("../trainingData/DTWModel.grt");
    cout << grt.getTestAccuracy() << endl;

    return 0;
}
