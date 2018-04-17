#include "include/Classifier.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <Eigen/Dense>
#include <stdlib.h>
#include <functional>
#include <cmath>
#include <tuple>

#include "./include/Neurons.h"

// We normalise the data which helps the network train faster because we end up with the weights growing less quickly.
// Without normalising we have to take care to avoid overflows.
Classifier::Classifier      (Mat& train_images, Mat& test_images, Vec& train_labels, Vec& test_labels){


    _trainImages    = train_images;
    _trainLabels    = train_labels;
    _testImages     = test_images;
    _testLabels     = test_labels;

    _trainImages       /= (255);
	_trainImagesTest   /= (255);
    _testImages        /= (255);
}

// ShuffleData takes the test images and labels and shuffles their positions but keeps the
// associated labels and images together.
void Classifier::ShuffleData(Mat& Data, Vec& Labels){

    if(Data.rows() != Labels.rows()){

        std::cerr << "Size of Data " << Data.rows() << " is not equal to size of labels " << Labels.rows() << std::endl;
        exit(EXIT_FAILURE);
    }

    Mat tmpMat = Data;
    Vec tmpVec = Labels;

    std::vector<int>   position(Data.rows());
    for(int i = 0; i < position.size(); i++){

        position[i] = i;
    }
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(position), std::end(position), rng);

    for(int i = 0; i < Data.rows(); i++){

        Data.row(i) = tmpMat.row(position[i]);
        Labels(i)   = tmpVec(position[i]);
    }
}

// This is where we perform the training of the network and then test the accuracy on a previously
// unseen dataset. We can control the batch size, number of epochs, and stepsize.
void Classifier::NeuralNet  (int BATCH_SIZE, int NUMBER_OF_EPOCHS, float STEP_SIZE){

    // Defining our constants, Neurons, activation function and gradient of the activation function to be passed into the
    // neural nets. We also check if the total number of images is divisible by
    // the batch size so as to not cause core dumps.
    float  Delta       = 1.0f;
    float  lambda      = 0.0001f;

    // Here we use a standard sigmoid function as the activation function.
    auto    sigFun      = [](float x){

                        return (float)(1.0 / (1.0 + std::exp(-x)));
    };
    auto    invSigFun   = [](float x){

                        return (float)((x * (1.0 - x)));
    };
    Neurons HiddenLayer(0, 100, sigFun, invSigFun);
    Neurons OutputLayer(1, 10, sigFun, invSigFun);

    if(!(_trainImages.rows() % BATCH_SIZE == 0)){

        std::cerr << "BATCH_SIZE must be a factor of " << _trainImages.rows() << std::endl;
        exit(EXIT_FAILURE);
    }

    float trainLoss;
    float trainError;
    float testLoss;
    float testError;
    // Training the network occurs here. The test data is split into training and validation
    // sets along with their relative labels. Then batches of images are passed to the network
    // and forward and backward passes are performed.
    for(int i = 0; i < NUMBER_OF_EPOCHS; i++){

        ShuffleData(_trainImages, _trainLabels);

        _trainImagesTest = _trainImages.block(49999, 0, 10000, _trainImages.cols());
        _trainLabelsTest = _trainLabels.segment(49999, 10000);

        Mat tmpMat = _trainImages.block(0, 0, 50000, _trainImages.cols());
        Vec tmpVec = _trainLabels.segment(0, 50000);

        // Each iteration of stochastic gradient descent
        // on a single batch of images
        float    Error  = 0;
        int ITERATIONS  = static_cast<int>(tmpMat.rows() / BATCH_SIZE);
        for(int j = 0; j < ITERATIONS; j++){

            Mat tmpTrainImages = tmpMat.block(j * BATCH_SIZE, 0, BATCH_SIZE, tmpMat.cols());
            Vec tmpTrainLabels = tmpVec.segment(j * BATCH_SIZE, BATCH_SIZE);

            HiddenLayer.ForwardPass(tmpTrainImages, tmpTrainLabels);
            std::tie(trainLoss, trainError) = OutputLayer.LossFunc(HiddenLayer.getOutputs(), HiddenLayer.getOutputLabels(), Delta, STEP_SIZE, lambda);
            Error += trainError;
            HiddenLayer.Backpass(OutputLayer.getGradientsPassBack(), STEP_SIZE, lambda);
        }

        Error = Error / ITERATIONS;
        std::cout << Error << "         ";

        HiddenLayer.ForwardPass(_trainImagesTest, _trainLabelsTest);
        std::tie(testLoss, testError) = OutputLayer.ScoreFunc(HiddenLayer.getOutputs(), HiddenLayer.getOutputLabels(), lambda);
        std::cout << testError << std::endl;
    }


    // Final output of test set to see the cost and score
    float outScore;
    float outLoss;
    std::cout << std::endl;
    HiddenLayer.ForwardPass(_testImages, _testLabels);
    std::tie(outLoss, outScore) = OutputLayer.ScoreFunc(HiddenLayer.getOutputs(), HiddenLayer.getOutputLabels(), lambda);
    std::cout << outScore << std::endl;
}
