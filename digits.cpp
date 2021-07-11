#include "headers/Mnist.h"
#include "headers/NeuralNetwork.h"
#include "headers/InitializeFile.h"

using namespace std;

void train()
{
    Mnist Mnist("train");
    NeuralNetwork NN;
    for (int i = 0; i < 60000; i++)
    {
        Mnist.ReadFiles();
        NN.FeedForward(Mnist.image);
        NN.Backpropagate(Mnist.answer);
        NN.AddChanges();
    }
    NN.WriteNetwork();
}

void TestAndSee(int number)
{
    Mnist Mnist("test");
    NeuralNetwork NN;
    for (int i = 0; i < number; i++)
    {
        Mnist.ReadFiles();
        Mnist.ShowImage();
        NN.FeedForward(Mnist.image);
        cout << "Mnist says it is : " << Mnist.FinalAnswer() << endl;
        cout << "NeuranNetwork claims it is : " << NN.FinalAnswer() << endl;
    }
}
void test()
{
    Mnist Mnist("test");
    NeuralNetwork NN;
    int faults = 0;
    for (int i = 0; i < 10000; i++)
    {
        Mnist.ReadFiles();
        NN.FeedForward(Mnist.image);
        if (Mnist.FinalAnswer() != NN.FinalAnswer())
        {
            faults++;
        }
    }
    cout << "total faults are : " << faults << endl;
}

void testOnTrainingSet()
{
    Mnist Mnist("train");
    NeuralNetwork NN;
    int faults = 0;
    for (int i = 0; i < 60000; i++)
    {
        Mnist.ReadFiles();
        NN.FeedForward(Mnist.image);
        if (Mnist.FinalAnswer() != NN.FinalAnswer())
        {
            faults++;
        }
    }
    cout << "total faults are : " << faults << endl;
}

int main(int argc, const char *argv[])
{
    ConsoleInitializeFile();
    train();
    TestAndSee(10);
    return 0;
}
