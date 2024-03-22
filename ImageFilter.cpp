#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "Image_Class.h"

void grayScale(Image& image)
{
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned  int avg = 0;

            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k);
            }

            avg /= 3; // Calculate average

            // Set all channels to the average value
            image(i, j, 0) = avg;
            image(i, j, 1) = avg;
            image(i, j, 2) = avg;
        }
    }
}


void blackAndWhite(Image& image)
{
    int threshold{ 127};
    grayScale(image);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            if(image(i,j,0) <= threshold)
            {
                image(i,j,0) = 0;
            }
            else if(image(i,j,0) > threshold)
            {
                image(i,j,0) = 255;
            }

            if(image(i,j,1) <= threshold)
            {
                image(i,j,1) = 0;
            }
            else if(image(i,j,1) > threshold)
            {
                image(i,j,1) = 255;
            }
            if(image(i,j,2) <= threshold)
            {
                image(i,j,2) = 0;
            }
            else if(image(i,j,2) > threshold)
            {
                image(i,j,2) = 255;
            }
        }
    }
}
void invertImage()
{

}
void mergeImages()
{

}
void flipImage()
{

}

std::string chooseFilter()
{
    std::string choice{};
    while (true)
    {
        std::cout << "what filter do you want use?" << std::endl;
        std::cout << "1. Grayscale" << std::endl;
        std::cout << "2. Black and White" << std::endl;
        std::cout << "3. Invert Image" << std::endl;
        std::cout << "4. Merge Images" << std::endl;
        std::cout << "5. Flip Image" << std::endl;
        std::cout << "6. Return" << std::endl;
        std::cout << "enter choice:";
        std::cin >> choice;
        if("1" <= choice and choice <= "6" and choice.length() == 1)
        {
            return choice;
        }
        std::cout << "Please enter a valid choice" << std::endl;
    }
}

int getImage(Image& image)
{
    std::string imageName{};
    std::cout << "Please enter an image name or type exit to close:" << std::flush;
    std::getline(std::cin >> std::ws, imageName);
    if(imageName == "exit")
    {
        std::cout << "------------" << std::endl;
        std::cout << "| GOOD BYE |" << std::endl;
        std::cout << "------------" << std::endl;
        return 0;
    }
    try {
        image.loadNewImage(imageName);
    }
    catch (std::invalid_argument& e) {
        std::cerr << std::flush;
        std::cout << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return -1;
    }
    return 1;
}

//take the new image name to save it or overwrite the same image
int saveImage(Image& image)
{
    std::string imageName{};
    std::cout << "Please enter the image name to save it \nOr type the same name to overwrite it:" << std::flush;
    std::getline(std::cin >> std::ws, imageName);
    try {
        image.saveImage(imageName);
    }
    catch (std::invalid_argument& e) {
        std::cerr << std::flush;
        std::cout << e.what() << std::endl;
        // stop the program for 100ms,
        // because std::cerr and std::cout of the new iteration get printed on the same line
        // which affects the formatting, so print the message from std::cerr the stop a bit then continue
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return -1;
    }
    return 1;
}

int main()
{
    std::cout << "-----------------------------" << std::endl;
    std::cout << "| Welcome to Baby Photoshop |" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    //- Ask the user about the image name that they want to load it
    //- load the image in "image"
    //- define "imageStatus" and use it to know if the operation of loading the image succeeded, failed, or the user wants to exit
    std::string imageName{};
    Image image;
    int imageStatus{};
    while (true)
    {
        imageStatus = getImage(image);
        if(imageStatus == 0)
        {

            break;
        }
        else if(imageStatus == -1)
        {
            continue;
        }
        // check what filters does the user want
        std::string choice{};
        choice = chooseFilter();
        switch (std::stoi(choice)) {
            case 1:
                grayScale(image);
                break;
            case 2:
                blackAndWhite(image);
                break;
            case 3:
                invertImage();
                break;
            case 4:
                mergeImages();
                break;
            case 5:
                flipImage();
                break;
            default:
                std::cout << "You did not choose a filter" << std::endl;
        }
        std::string saveOption{};
        do {
            std::cout << "Do you want to save the image or discard it?(save/discard)" << std::flush;
            std::cin >> saveOption;
            if(saveOption == "save")
            {
                do {
                    imageStatus = saveImage(image);
                } while (imageStatus == -1);
                break;
            }
            else if(saveOption == "discard")
            {
                break;
            }
            else
            {
                std::cout << "Please enter a valid option" << std::endl;
            }
        } while (saveOption != "save" or saveOption != "discard");
    }
}