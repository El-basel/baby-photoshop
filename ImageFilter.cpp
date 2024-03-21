#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "Image_Class.h"

void grayScale()
{

}

void blackAndWhite()
{

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

int main()
{
    std::string choice{};
    std::cout << "-----------------------------" << std::endl;
    std::cout << "| Welcome to baby photoshop |" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::string imageName{};
    Image image;
    while (true)
    {
        std::cout << "please enter an image name or type exit to close:" << std::flush;
        std::cin >> imageName;
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
            continue;
        }
        std::cout << "hello" << std::endl;
        choice = chooseFilter();
        switch (std::stoi(choice)) {
            case 1:
                grayScale();
                break;
            case 2:
                blackAndWhite();
            case 3:
                invertImage();
            case 4:
                mergeImages();
            case 5:
                flipImage();
            default:
                std::cout << "You did not choose a filter" << std::endl;
        }
        std::cout << "Please enter the image name to save it or type the same name to overwrite it:";
        std::cin >> imageName;
        try {
            image.saveImage(imageName);
        }
        catch (std::invalid_argument& e) {
            std::cerr << std::flush;
            std::cout << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
    }
}