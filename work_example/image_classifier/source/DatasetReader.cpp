// MNIST dataset reader
//
// reads in the 60.000 training and 10.000 testing images of
// the "MNIST database of handwritten digits" (0,1,2,...,9)
// see http://yann.lecun.com/exdb/mnist/
//
// note: download yourself the dataset and extract the files to a folder.
//       after extracting, you should get the MNIST dataset files:
//         t10k-images.idx3-ubyte
//         t10k-labels.idx1-ubyte
//         train-images.idx3-ubyte
//         train-labels.idx1-ubyte
//
// by Prof. Dr.-Ing. Jürgen Brauer, www.juergenbrauer.org
//
// parts of the code are inspired by
// http://stackoverflow.com/questions/8286668/how-to-read-mnist-data-in-c


#include "include/DatasetReader.h"




// read in all the training / test images and their corresponding ground truth labels
DatasetReader::DatasetReader(std::string path_to_extracted_mnist_files, Mat& train_images, Mat& test_images, Vec& train_labels, Vec& test_labels){
	// 1. store path to folder where dataset was extracted by user
	this->path_to_extracted_mnist_files = path_to_extracted_mnist_files;


	// 2. read in training data
	int nr_labels_read;

	// 2.1 read in training images

	read_mnist_images( path_to_extracted_mnist_files + "/" + "train-images.idx3-ubyte", train_images, nr_train_images_read, size_of_image);

	// 2.2 read in training ground truth labels
	read_mnist_labels( path_to_extracted_mnist_files + "/" + "train-labels.idx1-ubyte", train_labels, nr_labels_read);

	// 3. read in test data

	// 3.1 read in test images
	read_mnist_images(path_to_extracted_mnist_files + "/" + "t10k-images.idx3-ubyte", test_images, nr_test_images_read, size_of_image);

	// 3.2 read in testing ground truth labels
	read_mnist_labels(path_to_extracted_mnist_files + "/" + "t10k-labels.idx1-ubyte", test_labels, nr_labels_read);
}

void 	DatasetReader::read_mnist_images(std::string full_path, Mat& image, int& number_of_images, int& image_size){

	// This is to put the data in the correct endianess for intel CPU's. We use it on the file header information
	// which are each 4 bytes of informtion, hence the int input. It essentially boils down to bit masking all but
	// one byte and bit shifting it by the appropriate amount of bytes.
	auto reverseInt = [](int i) {
		unsigned char c1, c2, c3, c4;
		c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24) & 255;
		return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
	};

	// We open the file in as binary and read it in to a 1D array. We then use pointer arithmetic
	// to read each line(image) into an Eigen matrix. This is because Eigen is efficient and manages
	// memory for the matrices internally. It makes the rest of the project code much more readable,
	// easier to debug, and faster.
	std::ifstream file(full_path, std::ios::binary);

	if (file.is_open()) {

		int magic_number = 0, n_rows = 0, n_cols = 0;

		file.read((char *)&magic_number, sizeof(magic_number));
		magic_number = reverseInt(magic_number);

		if (magic_number != 2051) throw std::runtime_error("Invalid MNIST image file!");

		file.read((char *)&number_of_images, sizeof(number_of_images)), number_of_images = reverseInt(number_of_images);
		file.read((char *)&n_rows, sizeof(n_rows)), n_rows = reverseInt(n_rows);
		file.read((char *)&n_cols, sizeof(n_cols)), n_cols = reverseInt(n_cols);


		image_size = n_rows * n_cols;
		unsigned char* _dataset = new unsigned char[number_of_images * image_size];
		file.read((char *)_dataset, number_of_images * image_size);

		image = Mat(number_of_images, image_size);
		for(int i = 0; i < number_of_images; i++){
			for(int j = 0; j < image_size; j++){

				image(i,j) = static_cast<float>(_dataset[i*image_size + j]);
			}
		}

		delete[] _dataset;
		_dataset = nullptr;
	}
	else {
	throw std::runtime_error("Cannot open file `" + full_path + "`!");
	}
	file.close();
}

// Same process as before but instead we only need to read into a vector as there is only
// one label per image.
void 	DatasetReader::read_mnist_labels(std::string full_path, Vec& label, int& number_of_labels){
	auto reverseInt = [](int i) {
		unsigned char c1, c2, c3, c4;
		c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24) & 255;
		return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
	};

	std::ifstream file(full_path, std::ios::binary);

	if (file.is_open()) {

		int magic_number = 0;
		file.read((char *)&magic_number, sizeof(magic_number));
		magic_number = reverseInt(magic_number);

		if (magic_number != 2049) throw std::runtime_error("Invalid MNIST label file!");

		file.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);

		unsigned char* _dataset = new unsigned char[number_of_labels];
		file.read((char*)_dataset, number_of_labels);


		label = Vec(number_of_labels);

		for(int i = 0; i < number_of_labels; i++){

			label(i) = static_cast<float>(_dataset[i]);
		}

		delete[] _dataset;
		_dataset = nullptr;
	}
	else {
		throw std::runtime_error("Unable to open file `" + full_path + "`!");
	}
	file.close();
}


int 	DatasetReader::get_nr_train_images_read(){
	return nr_train_images_read;
}

int 	DatasetReader::get_nr_test_images_read(){
	return nr_test_images_read;
}

int 	DatasetReader::get_size_of_image(){
	return size_of_image;
}
