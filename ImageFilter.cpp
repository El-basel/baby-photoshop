#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "Image_Class.h"

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
void save(Image& image)
{
    int imageStatus{};
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
    int threshold{127};
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
    save(image);
}
void invertImage()
{

}
void mergeImages()
{

}
void topBottomFlip(Image& flipped, Image& image)
{
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                flipped(i, image.height - j - 1, k) = image(i,j,k);
            }
        }
    }
}
void leftRightFlip(Image& flipped, Image& image)
{
    for (int j = 0; j < image.height; ++j) {
        for (int i = 0; i < image.width; ++i) {
            for (int k = 0; k < 3; ++k) {
                flipped(image.width - i - 1, j, k) = image(i,j,k);
            }
        }
    }
}
void flipImage(Image& image)
{
    std::string choice{};
    std::cout << "How do you like to flip the image?" << std::endl;
    do {
        std::cout << "1. Top to Bottom" << std::endl;
        std::cout << "2. Left to Right" << std::endl;
        std::cout << "Enter your choice:";
        std::getline(std::cin >> std::ws, choice);
        if(choice < "1" or choice > "2" or choice.length() != 1)
        {
            std::cout << "please enter a valid choice" << std::endl;
        }
    } while (choice < "1" or choice > "2" or choice.length() != 1);
    Image flipped(image.width,image.height);

    if(choice == "1")
    {
        topBottomFlip(flipped, image);
    }
    else if(choice == "2")
    {
        leftRightFlip(flipped, image);
    }
    save(flipped);
}

void cropImage(Image& image)
{
    std::cout << "How do you like to crop the image?" << std::endl;
    int x,y,width,height;
    while (true)
    {
        std::cout << "Enter the coordinates of the starting point" << std::endl;
        std::cout << "Enter the x-coordinates first then leave a space then enter the y-coordinates:";
        std::cin >> x >> y;
        if(x > image.width or y > image.height or x < 0 or y < 0)
        {
            std::cout << "Please enter a valid coordinates" << std::endl;
            continue;
        }
        std::cout << "Enter the dimensions of the cropping" << std::endl;
        std::cout << "Enter the width first then leave a space then enter the height:";
        std::cin >> width >> height;
        if(width > image.width or height > image.height or width < 0 or height < 0)
        {
            std::cout << "Please enter a valid coordinates" << std::endl;
            continue;
        }
        break;
    }
    Image cropped(width,height);
    for (int i = x , iC = 0; i < x + width - 1; ++i, ++iC) {
        for (int j = y , jC = 0; j < y + height - 1; ++j, ++jC) {
            for (int k = 0; k < 3; ++k) {
                cropped(iC, jC, k) = image(i,j,k);
            }
        }
    }
    save(cropped);
}

int resizeOptions(double& xResize, double& yResize, Image& image)
{
    std::string x,y;
    std::string option{};
    int choice{};
    std::cout << "Do you want to enter a new dimensions or "
                 "enter a ratio of reduction or increase?" << std::endl;
    do {
        std::cout << "1. new dimensions" << std::endl;
        std::cout << "2. ratio of reduction" << std::endl;
        std::cout << "3. ratio of increase" << std::endl;
        std::cout << "Enter your choice:" << std::flush;
        std::getline(std::cin >> std::ws, option);
        try {
            choice = std::stoi(option);
        } catch (std::invalid_argument&) {
            std::cout << "Please enter a valid choice" << std::endl;
            continue;
        }
        if(choice < 1 or choice > 3)
        {
            std::cout << "Please enter a valid choice" << std::endl;
        }

    } while (choice < 1 or choice > 3);

    if(choice == 1)
    {
        std::string dimension{};
        std::cout << "Please enter the X dimension:" << std::flush;
        while (true)
        {
            std::getline(std::cin >> std::ws, dimension);
            try {
                xResize = std::stoi(dimension);
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid choice" << std::endl;
                continue;
            }
        }

        std::cout << "Please enter the Y dimension:" << std::flush;
        while (true)
        {
            std::getline(std::cin >> std::ws, dimension);
            try {
                yResize = std::stoi(dimension);
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid choice" << std::endl;
                continue;
            }
        }
        return 1;
    }
    else if(choice == 2)
    {
        std::string dimension{};
        while (true)
        {
            std::cout << "Enter the ratio of reduction in percent for X:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            try {
                xResize = std::stoi(dimension);
                if(xResize > 100 or xResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }

        xResize = image.width - std::floor(double(image.width) * xResize/100.0);
        while (true)
        {
            std::cout << "Enter the ratio of reduction in percent for Y:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            try {
                yResize = std::stoi(dimension);
                if(yResize > 100 or yResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }
        yResize = image.height - std::floor(double(image.height) * yResize/100.0);
        return 2;
    }
    else if(choice == 3)
    {
        std::string dimension{};
        while (true)
        {
            std::cout << "Enter the ratio of increase for X:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            try {
                xResize = std::stoi(dimension);
                if(xResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }
        xResize = std::floor(double(image.width) * xResize/100.0) + image.width;
        while (true)
        {
            std::cout << "Enter the ratio of increase in percent for Y:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            try {
                yResize = std::stoi(dimension);
                if(yResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }
        yResize = std::floor(double(image.height) * yResize/100.0) + image.height;
        return 3;
    }
    return 0;
}
void resizeImage(Image& image)
{
    double xResize{};
    double yResize{};
    int option = resizeOptions(xResize,yResize,image);
    Image resizedImage(xResize,yResize);
    if(option == 1 or option == 3)
    {
        xResize = std::ceil(double (xResize)/image.width * 100) / 100 ;
        yResize = std::ceil(double (yResize)/image.height * 100) / 100;
    }
    else if(option == 2)
    {
        xResize = image.width - xResize;
        xResize = std::ceil(double(xResize) / image.width * 100) / 100;
        yResize = image.height - yResize;
        yResize = std::ceil(double(yResize) / image.height * 100) / 100;
    }
    int originalX{};
    int originalY{};
    for (int i = 0; i < resizedImage.width; ++i) {
        for (int j = 0; j < resizedImage.height; ++j) {
                originalX = std::floor(i / xResize);
                originalY = std::floor(j / yResize);
                if(originalX < 0)
                {
                    originalX = 0;
                }
                else if(originalX > image.width - 1)
                {
                    originalX = image.width - 1;
                }
                if(originalY < 0)
                {
                    originalY = 0;
                }
                else if(originalY > image.height - 1)
                {
                    originalY = image.height - 1;
                }
            for (int k = 0; k < 3; ++k) {
                resizedImage(i,j,k) = image(originalX,originalY,k);
            }
        }
    }
    save(resizedImage);
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
        std::cout << "6. Crop Image" << std::endl;
        std::cout << "7. Resize Image" << std::endl;
        std::cout << "8. Return" << std::endl;
        std::cout << "enter choice:";
        std::cin >> choice;
        if("1" <= choice and choice <= "8" and choice.length() == 1)
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
                save(image);
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
                flipImage(image);
                break;
            case 6:
                cropImage(image);
                break;
            case 7:
                resizeImage(image);
                break;
            default:
                std::cout << "You did not choose a filter" << std::endl;
                save(image);
        }
    }
}