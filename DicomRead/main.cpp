#include "DICOMReader.hpp"

#include <iostream>

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <DICOM File Path> <Output Image Path>" << std::endl;
        return 1;
    }

    std::string dicomFilePath = argv[1];
    std::string outputImagePath = argv[2];

    // Create a DICOMReader object
    DICOMReader dicomReader(dicomFilePath);

    // Read DICOM tags
    dicomReader.readDicomTags();

    // Read pixel data and save as an image
    dicomReader.readAndSavePixelData(outputImagePath);

    return 0;
}
