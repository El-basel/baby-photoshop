#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <regex>
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
void invertcolor(Image& image)
{
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; i < image.height; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
    save(image);
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
void invertImage()
{

}
void mergeImages()
{

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

void cropImage(Image& image)
{
    // get the x and y coordinate of the original image that will be the starting point for cropping
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "| How do you like to crop the image? |" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::regex reg{"[^0-9]+"};
    std::string xCoord, yCoord,widthEntered, heightEntered;
    int x,y,width,height;

    std::cout << "Enter the coordinates of the starting point" << std::endl;
    while (true) {
        std::cout << "Enter the x-coordinates:" << std::flush;
        std::getline(std::cin >> std::ws, xCoord);
        if (std::regex_search(xCoord, reg)) {
            std::cout << "Please enter a valid coordinates" << std::endl;
            continue;
        }
        try {
            x = std::stoi(xCoord);
        }
        catch (std::invalid_argument &) {
            std::cout << "Please enter a valid coordinate" << std::endl;
            continue;
        }
        // check if the x coordinates the negative or greater than image width
        if (x > image.width or x < 0) {
            std::cout << "Please enter a valid coordinate starting from 0 to image width" << std::endl;
            continue;
        }
        break;
    }
    while (true)
    {
        std::cout << "enter the y-coordinates:" << std::flush;
        std::getline(std::cin >> std::ws, yCoord);
        if(std::regex_search(yCoord,reg))
        {
            std::cout << "Please enter a valid coordinate" << std::endl;
            continue;
        }
        try {
            y = std::stoi(yCoord);
        }
        catch (std::invalid_argument &) {
            std::cout << "Please enter a valid coordinate" << std::endl;
            continue;
        }

        // check if the y coordinate is less than zero or greater than the image height
        if (y > image.height or y < 0) {
            std::cout << "Please enter a valid coordinate" << std::endl;
            continue;
        }
        break;
    }
    std::cout << "Enter the dimensions of the cropping" << std::endl;
    while (true) {
        std::cout << "Enter the desired width for the copped image:" << std::flush;
        std::getline(std::cin >> std::ws, widthEntered);
        if (std::regex_search(widthEntered, reg)) {
            std::cout << "Please enter a valid width" << std::endl;
            continue;
        }
        try
        {
            width = std::stoi(widthEntered);
        }
        catch (std::invalid_argument&) {
            std::cout << "Please enter a valid width" << std::endl;
            continue;
        }
        // check if the width is less than zero or greater than the image width
        if(width > image.width or width < 0)
        {
            std::cout << "width has to be a positive integer starting from 0 to the original image width" << std::endl;
            continue;
        }
        break;
    }
    while (true)
    {
        std::cout << "Enter the desired height for the copped image:" << std::flush;
        std::getline(std::cin >> std::ws, heightEntered);
        if(std::regex_search(heightEntered, reg))
        {
            std::cout << "Please enter a valid height" << std::endl;
            continue;
        }
        try
        {
            height = std::stoi(heightEntered);
        }
        catch (std::invalid_argument&) {
            std::cout << "Please enter a valid height" << std::endl;
            continue;
        }
        // check if the height is less than zero or greater than the image height
        if(height > image.height or height < 0)
        {
            std::cout << "height has to be a positive integer starting from 0 to the original image height" << std::endl;
            continue;
        }
        break;
    }
    // iterate though the width and height of the cropped image
    // start iterating on the original image form the entered x and y coordinates
    // define new variable (iC,jC) to index the cropped image from the starting point (0,0)
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
    // check if the user wants to enter dimensions or ratio of reduction or ratio of increase
    std::string x,y;
    std::string option{};
    std::regex reg{"[^0-9]+"};
    int choice{};
    std::cout << "Do you want to enter a new dimensions or "
                 "enter a ratio of reduction or increase?" << std::endl;
    do {
        std::cout << "1. new dimensions" << std::endl;
        std::cout << "2. ratio of reduction" << std::endl;
        std::cout << "3. ratio of increase" << std::endl;
        std::cout << "Enter your choice:" << std::flush;
        std::getline(std::cin >> std::ws, option);
        if(std::regex_search(option,reg))
        {
            std::cout << "Please enter a valid choice" << std::endl;
            continue;
        }
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
        while (true)
        {
            std::cout << "Please enter the X dimension:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if(std::regex_search(dimension,reg))
            {
                std::cout << "Please enter a valid dimension" << std::endl;
                continue;
            }
            try {
                xResize = std::stoi(dimension);
                break;
            } catch (std::invalid_argument&) {
                std::cout << "Please enter a valid choice" << std::endl;
                continue;
            }
        }

        while (true)
        {
            std::cout << "Please enter the Y dimension:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if(std::regex_search(dimension,reg))
            {
                std::cout << "Please enter a valid dimension" << std::endl;
                continue;
            }
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
            if(std::regex_search(dimension,reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
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
            if(std::regex_search(dimension,reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
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
            if(std::regex_search(dimension,reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
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
            if(std::regex_search(dimension,reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
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
        std::cout << "3. Invert Image" << std::endl;
        std::cout << "4. Merge Images" << std::endl;
        std::cout << "5. Flip Image" << std::endl;
        std::cout << "6. Crop Image" << std::endl;
        std::cout << "7. Resize Image" << std::endl;
        std::cout << "8. Brighten or Darken Image" << std::endl;
        std::cout << "9. invert colors" << std::endl;
        std::cout << "10. Return" << std::endl;
        std::cout << "enter choice:";
        std::cin >> choice;
        if(1 <= stoi(choice) and stoi(choice) <= 10 and choice.length() <= 2)
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
            case 8:
                brightenOrDarken(image);
                break;
            case 9:
                invertcolor(image);
                break;
            default:
                std::cout << "You did not choose a filter" << std::endl;
                save(image);
        }
    }
}