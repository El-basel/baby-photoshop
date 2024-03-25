/* FCAI - Structured Programming - 2024 - Assignment 3 - Part 1
 * Program Description: a program that add filters to a provided image for the user
 *                      The available filters are: gray scale, black and white, merge images, darken or brighten the image and color invertor
 * Program name: CS112_A3_Part1_S1_20230376_20230278_20230517.cpp
 * Author1 and ID and Group: Mahmoud Mohamed El-Basel Hegazy,   20230376, Group A
 * Author2 and ID and Group: Fares Mohammed Abdulhamid Sarhan,  20230278, Group A
 * Author3 and ID and Group: Youssef Walid Mohamed Shaker,      20230517, Group A
 * Teaching Assistant: Yomna Esmail Fathy
 *
 * Mahmoud Mohamed El-Basel did: Black and White Filter
 * Youssef Walid did: color invertor
 * */

#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <regex>
#include <algorithm>
#include "Image_Class.h"

//take the new image name to save it or overwrite the same image
int saveImage(Image& image)
{
    std::string imageName{};
    std::cout << "Please enter the image name to save it" << std::flush;
    std::cout << " Or type the same name to overwrite it:" << std::flush;
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
        std::cout << "Do you want to save the image or discard it? (save/discard):" << std::flush;
        std::getline(std::cin >> std::ws, saveOption);
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

// gray scale filter by getting the average of the pixel channels then assigning the average to those channels
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

// black and white filter is applied by first making the image gray scale
// if the channels in the pixel are greater than "127" then make this pixel white
// if the channels in the pixel are less than or equal "127" then make this pixel black
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
void mergeCrop(Image& image, int& minWidth, int& minHeight){
    Image newImage(minWidth, minHeight);
    for (int i = 0; i < minWidth; ++i){

        for (int j = 0; j < minHeight; ++j){

            for (int k = 0; k < 3; ++k){
                newImage(i, j, k) = image(i, j, k);
            }
        }
    }
    save(newImage);
}

void mergeImages(Image& image){
    Image image2;

    while(true){
        std::string imageName{};
        std::cout << "Please enter the second image name:" << std::flush;
        std::getline(std::cin >> std::ws, imageName);
        try {
            image2.loadNewImage(imageName);
        }
        catch (std::invalid_argument& e) {
            std::cerr << std::flush;
            std::cout << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        break;
    }

    int width = std::min(image.width, image2.width);
    int height = std::min(image.height, image2.height);

    int color, avg;
    for (int i = 0; i < width; ++i){

        for (int j = 0; j < height; ++j){

            for (int k = 0; k < 3; ++k){

                color = image(i, j, k) + image2(i, j, k);
                avg = color / 2;
                image(i, j, k) = avg;
            }
        }
    }
    mergeCrop(image, width, height);
}

void invertcolor(Image& image)
{
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                //to get the opposite color of the pixel we on
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
    save(image);
}

std::string brightenDarkenChoice(){
    //take choice from user to brighten or darken
    //and return value based on choice
    std::string choice{};
    while (true)
    {
        std::cout << "Do you want to Brighten or Darken the image?" << std::endl;
        std::cout << "1. Brighten" << std::endl;
        std::cout << "2. Darken" << std::endl;
        std::cout << "enter choice: ";
        std::cin >> choice;
        if("1" <= choice and choice <= "2" and choice.length() == 1)
        {
            return choice;
        }
        std::cout << "Please enter a valid choice" << std::endl;
    }
}

void brightenOrDarken(Image& image)
{
    std::string choice;
    int color;
    double multiplierValue;
    //take choice from user to brighten or darken
    choice = brightenDarkenChoice();

    //if choice is brighten, set multiplier to 1.5x (increase brightness by 50%)
    if (choice == "1"){
        multiplierValue = 1.5;
    }

        //if choice is darken, set multiplier to 0.5x (decrease brightness by 50%)
    else{
        multiplierValue = 0.5;
    }

    //loop through each pixel and apply brightness multiplier
    for (int i = 0; i < image.width; ++i){

        for (int j = 0; j < image.height; ++j){

            for (int k = 0; k < 3; ++k){

                color = image(i, j, k);
                color *= multiplierValue;

                //make sure color value dont exceed 255 and not go below 0
                if (color >= 255){
                    color = 255;
                }

                else if (color <= 0){
                    color = 0;
                }

                else{
                    image(i, j, k) = color;
                }

            }
        }
    }
    save(image);
}

std::string chooseFilter()
{
    // get the user choice for filters
    std::string choice{};
    while (true)
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << "| what filter do you want use? |" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << "1. Grayscale" << std::endl;
        std::cout << "2. Black and White" << std::endl;
        std::cout << "3. Merge Images" << std::endl;
        std::cout << "4. invert colors" << std::endl;
        std::cout << "5. Brighten or Darken Image" << std::endl;
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
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Please enter an image name or type exit to close:" << std::flush;
    std::getline(std::cin >> std::ws, imageName);
    std::cout << "-------------------------------------------------" << std::endl;
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
                mergeImages(image);
                break;
            case 4:
                invertcolor(image);
                break;
            case 5:
                brightenOrDarken(image);
                break;
            default:
                std::cout << "You did not choose a filter" << std::endl;
                save(image);
        }
    }
}