/* FCAI - Structured Programming - 2024 - Assignment 3 - Part 1
 * Program Description: a program that add filters to a provided image for the user
 *                      The available filters are: gray scale, black and white, flip the image, darken or brighten the image and color invertor
 * Program name: CS112_A3_Part1_S1_20230376_20230278_20230517.cpp
 * Author1 and ID and Group: Mahmoud Mohamed El-Basel Hegazy,   20230376, Group A
 * Author2 and ID and Group: Fares Mohammed Abdulhamid Sarhan,  20230278, Group A
 * Author3 and ID and Group: Youssef Walid Mohamed Shaker,      20230517, Group A
 * Teaching Assistant: Yomna Esmail Fathy
 *
 * Mahmoud Mohamed El-Basel did: Black and White Filter, and flip the image
 * Youssef Walid did: color invertor
 * Fares Mohammed did: Brighten and darken, and grayscale filters
 *
 * system diagram: https://drive.google.com/file/d/1TVGHaVuiUS-xxS0q51B1S-3m1zOisO0s/view?usp=sharing
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
    // ask the user if they want to save the image or discard it
    // and validate that they entered the right extension
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


void topBottomFlip(Image& flipped, Image& image)
{
    // iterate through each pixel in the original and flipped image, both of them will be accessed using the same index for the width
    // but for flipped image the height will be accessed by (main image - j) to make the flipping happen
    // as index "j" starts from 0 to original image height
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
    // iterate through each pixel in the original and flipped image, both of them will be accessed using the same index for the height
    // but for flipped image the height will be accessed by (main image - i) to make the flipping happen
    // as index "i" starts from 0 to original image width
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
    // get the user choice if they want to flip the image vertically or horizontally
    std::string choice{};
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "| How do you like to flip the image? |" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
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
    // make a new image which its width and height will equal the original image width and height respectively
    Image flipped(image.width,image.height);

    if(choice == "1")
    {
        // if the user wants to flip the image horizontally then call the topBottomFlip with the original image and the flipped as parameters
        topBottomFlip(flipped, image);
    }
    else if(choice == "2")
    {
        // if the user wants to flip the image vertically then call the topBottomFlip with the original image and the flipped as parameters
        leftRightFlip(flipped, image);
    }
    save(flipped);
}
// get each pixel in the image and subtract 255 from it
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

void brightenDarkenImage(Image& image, double multiplier){
    int color;

    //loop through each pixel and apply brightness multiplier

    for (int i = 0; i < image.width; ++i){

        for (int j = 0; j < image.height; ++j){
            
            for (int k = 0; k < 3; ++k){

                color = image(i, j, k);
                color *= multiplier;

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

double brightenDarkenPercent(std::string choice) {
    //Ask user for percentage of brightening or darkening, and return the percentage
    std::string percent;
    while (true) {
        std::cout << "How much do you want to " << choice << " your image by percent?" << std::endl;
        std::cout << "Enter a number between 0 and 100: ";
        std::cin >> percent;
        
        try {
            int value = std::stoi(percent);
            if (value >= 0 && value <= 100) {
                return value;
            }
            else {
                std::cout << "Please enter a valid number between 0 and 100." << std::endl;
            }
        }
        catch (std::invalid_argument const&) {
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
        }
    }
}

void brightenOrDarken(Image& image)
{
    std::string choice;
    int color;
    double percent;
    double multiplierValue;

    //take choice from user to brighten or darken
    choice = brightenDarkenChoice();

    //if choice is brighten ask for percentage of brightening and update multiplier
    if (choice == "1"){
        percent = brightenDarkenPercent("brighten");
        multiplierValue = 1 + (percent / 100);
    }
    //if choice is darken ask for percentage of darkening and update multiplier
    else{
        percent = brightenDarkenPercent("darken");
        multiplierValue = percent / 100;
    }

    brightenDarkenImage(image, multiplierValue);
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
        std::cout << "3. Flip Image" << std::endl;
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
                flipImage(image);
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