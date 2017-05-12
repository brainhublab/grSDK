#include "GRT/GRT.h"
#include "grAlgorithmNew.h"
using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{
    GRAlgorithm grAlg;

    grAlg.loadTrainingData("./data/grTrainingDTWtest.grt");
    grAlg.setTestDataFromTraining(20);

    //grAlg.loadModel("./data/DTWModel.grt");

    grAlg.train();
    grAlg.test();

    grAlg.saveModel("./data/DTWModel.grt");
    cout << grAlg.getTestAccuracy() << endl;

    return 0;
}
