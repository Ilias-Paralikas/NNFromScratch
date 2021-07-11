//read the mnist database, hand writen images
#include "Matrix.h"

#define ImageMagicNumber 2051
#define LabelMagicNumber 2049
#define XPixels 28
#define YPixels 28
#define total 784

using namespace std;

class Mnist
{
private:
    ifstream ImageFile, LabelFile;

public:
    int NumberOfImages;
    Matrix answer, image;

public:
    Mnist(const char *Instruction)
    {
        if (Instruction == "test")
        {
            this->ImageFile.open("./images/t10k-images-idx3-ubyte", ios::in | ios::binary);
            this->LabelFile.open("./images/t10k-labels-idx1-ubyte", ios::in | ios::binary);
            this->NumberOfImages = 10000;
        }
        else if (Instruction == "train")
        {
            this->ImageFile.open("./images/train-images-idx3-ubyte", ios::in | ios::binary);
            this->LabelFile.open("./images/train-labels-idx1-ubyte", ios::in | ios::binary);
            this->NumberOfImages = 60000;
        }
        else
            throw "Wrong instruction ";
        InitializeImages();
        InitializeLabels();
        answer.Set(10, 1);
        image.Set(total, 1);
    }

    ~Mnist()
    {
        //cout << "Mnist stared deleting\n"; sskasre re
        ImageFile.close();
        LabelFile.close();
        //cout << "Mnist stopped deleting\n";
    }

    void ReadFiles()
    {
        this->ReadImage();
        this->CreateAnswer();
    }

    void ShowImage()
    {
        for (int i = 0; i < total; i += 28)
        {
            for (int j = 0; j < YPixels; j++)
            {
                this->image.data[i + j] *= 255;
                if (this->image.data[i + j] < 100)
                    cout << " ";
                if (this->image.data[i + j] < 10)
                    cout << " ";
                cout << " " << this->image.data[i + j];
            }
            cout << endl;
        }
    }

    void ShowAnswer()
    {
        int j;
        for (int i = 0; i < 10; i++)
        {
            cout << this->answer.data[i] << " ";
            if (this->answer.data[i] == 1)
                j = i;
        }
        cout << "\nFinal answer is " << j << endl;
    }

    int FinalAnswer()
    {
        int ans;
        for (int i = 0; i < 10; i++)
        {
            if (this->answer.data[i] == 1)
                ans = i;
        }
        return ans;
    }

private:
    int ReverseInt(int i)
    {
        unsigned char c1, c2, c3, c4;

        c1 = i & 255;
        c2 = (i >> 8) & 255;
        c3 = (i >> 16) & 255;
        c4 = (i >> 24) & 255;

        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    }
    void InitializeImages()
    {
        int buff[3];
        if (this->ImageFile.is_open())
        {
            this->ImageFile.read((char *)&buff[0], sizeof(int));
            if (ReverseInt(buff[0]) != ImageMagicNumber)
            {
                throw "wrong magic number\n";
            }
            //read the rest of useless info
            this->ImageFile.read((char *)buff, 3 * sizeof(int));
        }
        else
            throw "could now open image file";
    }

    void InitializeLabels()
    {
        int buff;
        if (this->LabelFile.is_open())
        {
            this->LabelFile.read((char *)&buff, sizeof(int));
            if (ReverseInt(buff) != LabelMagicNumber)
            {
                throw "wrong magic number\n";
            }
            //read quanitty
            this->LabelFile.read((char *)&buff, sizeof(int));
        }
        else
            throw "could now open label file";
    }

    void ReadImage()
    {
        unsigned char buff;
        for (int i = 0; i < total; ++i)
        {
            this->ImageFile.read((char *)&buff, sizeof(char));
            this->image.data[i] = buff;
            this->image.data[i] /= 255;
        }
    }

    void CreateAnswer()
    {
        char buff;
        this->LabelFile.read((char *)&buff, sizeof(char));
        for (int i = 0; i < 10; ++i)
        {
            this->answer.data[i] = 0;
        }
        this->answer.data[(int)buff]++;
    }
};