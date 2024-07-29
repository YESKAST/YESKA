#ifndef DICOMREADER_H
#define DICOMREADER_H

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmjpeg/djdecode.h>   // JPEG
#include <dcmtk/dcmjpls/djdecode.h>   // JPEG-LS
//#include <dcmtk/dcmjp2k/djp2kdecode.h> // JPEG 2000
#include <opencv2/opencv.hpp> // OpenCV header
#include <string>
#include <iostream>

class DICOMReader {
public:
    // Constructor
    DICOMReader(const std::string &dicomFilePath);

    // Function to read DICOM tags
    void readDicomTags();

    // Function to read pixel data and save it as an image
    void readAndSavePixelData(const std::string &outputImagePath) const;

private:
    std::string dicomFilePath;
    DcmFileFormat fileFormat;
};

#endif // DICOMREADER_H
