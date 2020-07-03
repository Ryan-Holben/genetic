#pragma once

#include <core/core.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#define readvar(var) read(reinterpret_cast<char*>(&var), sizeof(var))

uint32_t GetLittleEndian4Bytes(std::ifstream& stream) {
    return (stream.get() << 24) + (stream.get() << 16) + (stream.get() << 8) + (stream.get());
}

// http://yann.lecun.com/exdb/mnist/
core::Dataset GetMnistTrainingSet() {
    core::Dataset d;

    std::ifstream labels;
    labels.open("datasets/data/train-labels-idx1-ubyte", std::ifstream::in | std::ifstream::binary);
    if (!labels) {
        std::cout << "Couldn't open training labels.\n";
        return {};
    }

    std::ifstream images;
    images.open("datasets/data/train-images-idx3-ubyte", std::ifstream::in | std::ifstream::binary);
    if (!images) {
        std::cout << "Couldn't open training images.\n";
        return {};
    }

    if (GetLittleEndian4Bytes(labels) != 2049) {
        std::cout << "Label file had the wrong magic number.\n";
        return {};
    }

    if (GetLittleEndian4Bytes(images) != 2051) {
        std::cout << "Image file had the wrong magic number.\n";
        return {};
    }

    const size_t numEntries = GetLittleEndian4Bytes(labels);
    if (GetLittleEndian4Bytes(images) != numEntries) {
        std::cout << "Label and image file have a different number of entries.\n";
    }

    const size_t numRows = GetLittleEndian4Bytes(images);
    const size_t numCols = GetLittleEndian4Bytes(images);
    const size_t imageSize = numRows * numCols;

    for (size_t i = 0; i < 10; i++) {
        core::Datum entry;
        entry.second = {labels.get()};
        for (size_t j = 0; j < imageSize; j++) {
            entry.first.push_back((core::Number)(images.get())/255.0);
        }
        d.push_back(entry);
    }

    labels.close();
    images.close();

    return d;
}
