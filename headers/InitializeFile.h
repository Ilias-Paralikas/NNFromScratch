#include <iostream>
#include <fstream>
#include <ctime>
#include "NetworkName.h"

using namespace std;

float RandomFloat(float max)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float range = 2 * max;
    return (random * range) - max;
}

void InitializeFile(int NumberOfLayers, int *NeuronsInLayer, float WeightRange, float BiasRange)
{
    srand(time(NULL));
    ofstream network(NetworkName, ios::in | ios::binary);
    if (network.is_open())
    {
        float buff;

        network.write((const char *)&NumberOfLayers, sizeof(int));
        network.write((const char *)NeuronsInLayer, NumberOfLayers * sizeof(int));

        for (int i = 0; i < NumberOfLayers - 1; ++i)
        {
            for (int j = 0; j < NeuronsInLayer[i + 1]; ++j)
            {
                for (int k = 0; k < NeuronsInLayer[i]; ++k)
                {
                    buff = RandomFloat(WeightRange);
                    network.write((const char *)&buff, sizeof(float));
                }
            }
            for (int j = 0; j < NeuronsInLayer[i + 1]; ++j)
            {
                buff = RandomFloat(BiasRange);
                network.write((const char *)&buff, sizeof(float));
            }
        }
        network.close();
    }
    else
        throw "could not open network for initialization";
}

void ConsoleInitializeFile()
{
    int NumberOfLayers, *NeuronsInLayer;
    float WeightRange, BiasRange;
    cout << "How many layers will the network have? :";
    cin >> NumberOfLayers;
    cout << endl;
    NeuronsInLayer = new int[NumberOfLayers];
    for (int i = 0; i < NumberOfLayers; ++i)
    {
        cout << "How many Neurons in layer " << i << " ? :";
        cin >> NeuronsInLayer[i];
        cout << endl;
    }
    cout << "Insert Weight range :";
    cin >> WeightRange;
    cout << endl;
    cout << "Insert Bias range :";
    cin >> BiasRange;
    cout << endl;
    InitializeFile(NumberOfLayers, NeuronsInLayer, WeightRange, BiasRange);
    delete[] NeuronsInLayer;
}