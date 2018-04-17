#include <iostream>
#include <Eigen/Dense>
#include "include/DatasetReader.h"
#include "include/Classifier.h"



int main(){

	std::srand(time(NULL));
	//image_size x number_of_images = 784 x 60000
	Mat train_images;
	Mat test_images;
	Vec train_labels;
	Vec test_labels;
	DatasetReader reader("./source/include/MNIST_FILES", train_images, test_images, train_labels, test_labels);

	Classifier classifier(train_images, test_images, train_labels, test_labels);
	classifier.NeuralNet(10, 5, 0.5);

	return 0;


}
