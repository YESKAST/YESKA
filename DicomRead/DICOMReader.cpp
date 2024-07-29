#include "DICOMReader.hpp"

DICOMReader::DICOMReader(const std::string &dicomFilePath) : dicomFilePath(dicomFilePath) 
{
    // Register codecs for compressed data
    DJDecoderRegistration::registerCodecs();
    DJLSDecoderRegistration::registerCodecs();
    DJP2KDecoderRegistration::registerCodecs();

    // Load DICOM file
    OFCondition status = fileFormat.loadFile(dicomFilePath.c_str());
    if (!status.good()) 
    {
        std::cerr << "Error: Unable to open DICOM file: " << dicomFilePath << std::endl;
    }
}

// Function to read DICOM tags
void DICOMReader::readDicomTags() 
{
    OFString patientName, modality;
    Uint16 imageWidth, imageHeight;

    // Access DICOM dataset
    DcmDataset *dataset = fileFormat.getDataset();

    // Get Patient Name
    if (dataset->findAndGetOFString(DCM_PatientName, patientName).good()) 
    {
        std::cout << "Patient Name: " << patientName << std::endl;
    } else {
        std::cerr << "Error: Unable to find Patient Name!" << std::endl;
    }

    // Get Modality
    if (dataset->findAndGetOFString(DCM_Modality, modality).good()) 
    {
        std::cout << "Modality: " << modality << std::endl;
    } else {
        std::cerr << "Error: Unable to find Modality!" << std::endl;
    }

    // Get Image Width and Height
    if (dataset->findAndGetUint16(DCM_Rows, imageHeight).good() &&
        dataset->findAndGetUint16(DCM_Columns, imageWidth).good()) 
    {
        std::cout << "Image Width: " << imageWidth << std::endl;
        std::cout << "Image Height: " << imageHeight << std::endl;
    } else {
        std::cerr << "Error: Unable to find Image Dimensions!" << std::endl;
    }
}

// Function to read pixel data and save it as an image
void DICOMReader::readAndSavePixelData(const std::string &outputImagePath)  
{
    DicomImage *image = new DicomImage(dicomFilePath.c_str());

    if (image == nullptr || image->getStatus() != EIS_Normal) 
    {
        std::cerr << "Error: Unable to load DICOM image!" << std::endl;
        delete image;
        return;
    }

    // Check if the image is monochrome or color
    if (image->isMonochrome()) 
    {
        image->setMinMaxWindow();
    } else {
        image->setWindow(0.0, 255.0);
    }

    const int width = image->getWidth();
    const int height = image->getHeight();
    const int depth = image->getDepth(); // Check the bit depth
    const int channels = image->isMonochrome() ? 1 : 3; // Use 3 for RGB, 1 for grayscale

    // Create a buffer to hold pixel data
    std::vector<unsigned char> pixelData(width * height * channels);

    // Get pixel data
    if (image->getOutputData(pixelData.data(), depth, 0)) 
    {
        cv::Mat cvImage;
        if (channels == 1) 
        {
            cvImage = cv::Mat(height, width, CV_8UC1, pixelData.data()); // Grayscale image
        } else {
            cvImage = cv::Mat(height, width, CV_8UC3, pixelData.data()); // RGB image
            cv::cvtColor(cvImage, cvImage, cv::COLOR_RGB2BGR); // Convert from RGB to BGR
        }

        // Save the image using OpenCV
        if (cv::imwrite(outputImagePath, cvImage)) 
        {
            std::cout << "Image saved successfully as " << outputImagePath << std::endl;
        } else {
            std::cerr << "Error saving image with OpenCV!" << std::endl;
        }
    } else {
        std::cerr << "Error: Unable to retrieve pixel data!" << std::endl;
    }

    // Clean up
    delete image;
    
    // Unregister decompression codecs
    DJDecoderRegistration::cleanup();
    DJLSDecoderRegistration::cleanup();
    DJP2KDecoderRegistration::cleanup();
}
