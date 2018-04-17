#ifndef CLASSIFIER
#define CLASSIFIER

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <Eigen/Dense>
#include <stdlib.h>
#include "DatasetReader.h"


class Classifier{
public:

            Classifier  (Mat& train_images, Mat& test_images, Vec& train_labels, Vec& test_labels);

    // We shuffle the images to reduce the network being able to memorise the data, reducing overfitting.
    void    ShuffleData (Mat& Data, Vec& Labels);
    void    NeuralNet   (int BATCH_SIZE, int NUMBER_OF_EPOCHS, float STEP_SIZE);



private:

    Mat _trainImages;
    Mat _trainImagesTest;
    Mat _testImages;
    Vec _trainLabels;
    Vec _trainLabelsTest;
    Vec _testLabels;

};


#endif
