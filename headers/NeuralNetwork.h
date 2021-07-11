#include <cstring>
#include "NetworkName.h"
#include "Matrix.h"
#include "ActivationFunctions.h"

using namespace std;
// for the methods of this class i havent use the this-> thing, becuase the names are arlready too long
// in addition to that, Most of the work is done with class members and the only arguments these methods
// take are the Input and the answers, both of whom are integraded in the class members as soon as they are
// seen, input in FeedForward is copied into Activations[0]
// and the Answers matrix is integraded in the derivative of the cost function in Backpropogation
class NeuralNetwork
{
private:
    int NumberOfLayers, *NeuronsInLayer;
    Matrix *Activations, *RawActivations, *ActivationDerivatives, *Weights, *WeightChanges, *Biases, *BiasesChanges;

public:
    ~NeuralNetwork()
    {
        //cout << "NN starts deleting\n";
        delete[] NeuronsInLayer;
        delete[] Activations;
        delete[] RawActivations;
        delete[] ActivationDerivatives;
        delete[] Weights;
        delete[] WeightChanges;
        delete[] Biases;
        delete[] BiasesChanges;
        //cout << "NN stopped deleting\n";
    }
    NeuralNetwork()
    {
        ifstream network(NetworkName, ios::out | ios::binary);
        if (network.is_open())
        {
            network.read((char *)&(NumberOfLayers), sizeof(int));

            NeuronsInLayer = new int[NumberOfLayers];
            network.read((char *)NeuronsInLayer, NumberOfLayers * sizeof(int));
            // for weights we need N-1 matrixes
            Weights = new Matrix[NumberOfLayers - 1];
            WeightChanges = new Matrix[NumberOfLayers - 1];
            // for biases as well, as the initial layer does not have
            Biases = new Matrix[NumberOfLayers - 1];
            BiasesChanges = new Matrix[NumberOfLayers - 1];

            // for raw activations we dont need to keep the initial layer as it is the same as Activations
            RawActivations = new Matrix[NumberOfLayers - 1];
            //same here, we dont need the activation derivatines of the first layer
            ActivationDerivatives = new Matrix[NumberOfLayers - 1];
            // the activations keep the numbers of the first layer so needs to be N sized
            Activations = new Matrix[NumberOfLayers];
            //everyone else is N-1 so we do the first out of the loop
            Activations[0].Set(NeuronsInLayer[0], 1);
            // here we set the matrixes to their sizes,
            //notice astivations are i+1, because we set Act[0] before
            for (int i = 0; i < (NumberOfLayers - 1); i++)
            {
                WeightChanges[i].Set(NeuronsInLayer[i + 1], NeuronsInLayer[i]);
                Weights[i].Set(NeuronsInLayer[i + 1], NeuronsInLayer[i]);
                Weights[i].ReadMatrix(network);

                BiasesChanges[i].Set(NeuronsInLayer[i + 1], 1);
                Biases[i].Set(NeuronsInLayer[i + 1], 1);
                Biases[i].ReadMatrix(network);

                ActivationDerivatives[i].Set(NeuronsInLayer[i + 1], 1);
                RawActivations[i].Set(NeuronsInLayer[i + 1], 1);
                Activations[i + 1].Set(NeuronsInLayer[i + 1], 1);
            }
            network.close();
        }
        else
            throw "could now open network";
    }

    void FeedForward(const Matrix &Input)
    {
        Activations[0].Equal(Input);
        for (int i = 1; i < NumberOfLayers; i++)
        {
            RawActivations[i - 1].Multiply(Weights[i - 1], Activations[i - 1]);
            RawActivations[i - 1].InPlaceAdd(Biases[i - 1]);

            Activations[i].Equal(RawActivations[i - 1]);
            Activations[i].Map(ActivationFunction);
        }
    }

    void Backpropagate(const Matrix &Answer)
    {
        // cost of final layer is 2 *(MyAnswer - DesiredAnswer)
        ActivationDerivatives[NumberOfLayers - 2].Equal(Activations[NumberOfLayers - 1]);
        ActivationDerivatives[NumberOfLayers - 2].InPlaceSubtract(Answer);
        ActivationDerivatives[NumberOfLayers - 2].InPlaceScaler(2);
        for (int i = NumberOfLayers - 2; i >= 0; i--)
        {
            // we take the RawActivations and apply Activation derivative to get s'(z)
            RawActivations[i].Map(ActivationPrime);
            // now we multipy with the derivative of the cost in respect with the activation of the neuron
            // and we have the s'(z) * 2*(a - y) for the first itteration
            // and the s'(z) * θC/θa
            BiasesChanges[i].MultiplyByElement(ActivationDerivatives[i], RawActivations[i]);
            // Biases changes has dimension x *1 and Activations y*1
            //where x = NeuronsInLayer[i] and y = NeuronsInLayer[i-1]
            // weights changes must be x * y dimension, so we transpose from y*1 to  1*y
            // so  (x * 1) (*MatrixMul*) (1*y) = x*y
            Activations[i].TransposeVector();
            WeightChanges[i].Multiply(BiasesChanges[i], Activations[i]);
            Activations[i].TransposeVector();
            // now we need the activation derivatives for the next itteration
            // we dont want to do this if i = 0
            if (i != 0)
            {
                for (int j = 0; j < NeuronsInLayer[i]; j++)
                {
                    ActivationDerivatives[i - 1].data[j] = 0;
                    for (int k = 0; k < NeuronsInLayer[i + 1]; k++)
                    {
                        ActivationDerivatives[i - 1].data[j] +=
                            BiasesChanges[i].data[k] *
                            Weights[i].data[k * NeuronsInLayer[i] + j];
                    }
                }
            }
        }
    }

    int FinalAnswer()
    {
        int answer;
        float current, max = 0;
        for (int i = 0; i < NeuronsInLayer[NumberOfLayers - 1]; i++)
        {
            current = Activations[NumberOfLayers - 1].data[i];
            if (current > max)
            {
                max = current;
                answer = i;
            }
        }
        return answer;
    }

    void AddChanges()
    {
        for (int i = 0; i < NumberOfLayers - 1; i++)
        {
            Weights[i].InPlaceSubtract(WeightChanges[i]);
            Biases[i].InPlaceSubtract(BiasesChanges[i]);
        }
    }

    void WriteNetwork()
    {
        ofstream network(NetworkName, ios::out | ios::binary);
        if (network.is_open())
        {
            network.write((const char *)&(NumberOfLayers), sizeof(int));
            network.write((const char *)NeuronsInLayer, NumberOfLayers * sizeof(int));

            for (int i = 0; i < (NumberOfLayers - 1); i++)
            {
                Weights[i].WriteMatrix(network);
                Biases[i].WriteMatrix(network);
            }
        }
        else
            throw "could now open network";
    }

    void ShowNetwork()
    {
        for (int i = 0; i < NumberOfLayers - 1; ++i)
        {
            cout << "In Layer " << i << " Weights are :" << endl;
            Weights[i].ShowMatrix();
            cout << endl;
            cout << "And Biases :" << endl;
            Biases[i].ShowMatrix();
            cout << endl;
        }
        cout << endl;
    }
    void ShowActivations()
    {
        for (int i = 0; i < NumberOfLayers; ++i)
        {
            cout << "In Layer " << i << " Activations are :" << endl;
            Activations[i].ShowMatrix();
            cout << endl;
        }
        cout << endl;
    }
};