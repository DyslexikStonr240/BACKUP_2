#ifndef DATASET_READER
#define DATASET_READER

#include <string>
#include <fstream>
#include <iostream>
#include <Eigen/Dense>

typedef Eigen::MatrixXf Mat;
typedef Eigen::VectorXf Vec;

class DatasetReader{
public:
    // Reads in all training / test images + labels with each row representing one image
    DatasetReader(std::string path_to_extracted_mnist_files, Mat& train_images, Mat& test_images, Vec& train_labels, Vec& test_labels);

    int                     get_nr_train_images_read();          // should be 60.000
    int                     get_nr_test_images_read();
    int                     get_size_of_image();

private:
    // For reading in images and ground truth image labels
    void                    read_mnist_images(std::string full_path, Mat& image, int& number_of_images, int& image_size);
    void                    read_mnist_labels(std::string full_path, Vec& label, int& number_of_labels);

    std::string             path_to_extracted_mnist_files;


    int                     nr_train_images_read;          // should be 60.000
    int                     nr_test_images_read;           // should be 10.000
    int                     size_of_image;

};

#endif
