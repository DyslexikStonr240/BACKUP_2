#ifndef NEURONS
#define NEURONS

#include <Eigen/Dense>
#include <vector>
#include <iostream>

typedef Eigen::MatrixXf Mat;
typedef Eigen::VectorXf Vec;

class Neurons{
public:

    // Input images must be one image per row due to the order in which the dot products are performed.
    // We pass functions in as parameters so we can easily test different activation functions. The
    // downside of this is that the functions won't be inlined which will reduce performance.
    Neurons(int LAYER, int NUMBER_OF_NEURONS, float (*activationFunc)(float), float (*gradientActivationFunc)(float));

    // GETTERS
    // We don't need setters as everything is set up when we initialise a Neuron and as we don't want anyone
    // except for the neuron class to be able to manipulate it's data we exclude setters. We do need setters
    // to pass the weights to the next layer. This could be done with references but that makes it easier
    // to mess up the forward/backward propagation without realising. Again the downside is a decrease in performance.
    Mat getOutputs();
    Mat getInputs();
    Mat getWeights();
    Vec getOutputLabels();
    Mat getGradientsPassBack();

    float getWeightsSum();

    // These functions perform the actual process for training the network and evaluating input.
    void                      ForwardPass (Mat inputs, Vec inputLabels, float weightsSum = 0);
    void                      Backpass    (Mat inputGradients, float STEP_SIZE, float lambda);
    std::tuple<float, float>  LossFunc    (Mat inputs, Vec inputLabels, float Delta, float STEP_SIZE, float lambda, float weightsSum = 0);
    std::tuple<float, float>  ScoreFunc   (Mat inputs, Vec inputLabels, float lambda);

private:

    int _layer;
    int _numberOfNeurons;
    float _numberOfInputNeurons;
    float _weightsSum;
    bool _initialisationCheckFlag;

    //inputs and outputs with their respective labels
    Mat _inputs;
    Mat _outputs;
    Vec _inputLabels;
    Vec _outputLabels;

    //Weights and biases for Neuron
    Mat _weights;
    Eigen::RowVectorXd _biases;

    //Activation Function
    float (*_activationFunc)(float);
    float (*_gradientActivationFunc)(float);

    // Random number generating function used to set up the weights and biases to
    // produce better results.
    std::function<float(float)>  (_random);

    // These are used in backpropagation to make the code more readable.
    Mat _gradientsPassIn;
    Mat _gradientsInternal;
    Mat _gradientsPassBack;
};

#endif
