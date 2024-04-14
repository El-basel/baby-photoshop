#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <regex>
#include <algorithm>
#include <cstring>
#include <cmath>
#include "Image_Class.h"


bool wantToLoadNew()
{
    while(true)
    {
        std::string choice{};
        std::cout << "do You want to load a new image?(yes/no) :" << std::flush;
        std::getline(std::cin >> std::ws, choice);
        for (int i = 0; i < choice.length(); ++i) {
            choice[i] = std::tolower(choice[i]);
        }
        if(choice == "yes")
        {
            return true;
        }
        else if(choice == "no")
        {
            return false;
        }
        std::cout << "Please enter a valid option" << std::endl;
    }
}
//take the new image name to save it or overwrite the same image
int saveImage(Image& image)
{
    std::string imageName{};
    std::cout << "Please enter the new image name to save it" << std::flush;
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
        std::cout << "Do you want to save the current image or discard it? (save/discard):" << std::flush;
        std::cin >> saveOption;
        for (int i = 0; i < saveOption.length(); ++i) {
            saveOption[i] = std::tolower(saveOption[i]);
        }
        if(saveOption == "save")
        {
            do {
                imageStatus = saveImage(image);
            } while (imageStatus == -1);
            return;
        }
        else if(saveOption == "discard")
        {
            return;
        }
        else
        {
            std::cout << "Please enter a valid option" << std::endl;
        }
    } while (saveOption != "save" or saveOption != "discard");
}
void infrared(Image& image)
{
    int avg;
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int colorssum = 0;
            colorssum += image(i, j, 0);
            colorssum += image(i, j, 1);
            colorssum += image(i, j, 2);
            avg = colorssum / 3;
            image(i, j, 0) = 255 - avg;
            image(i, j, 1) = 255 - avg;
            image(i, j, 2) = 255 - avg;
            image(i, j, 0) += avg;
            if (image(i,j,0) > 255)
            {
                image(i, j, 0) = 255;
            }
        }
    }
    save(image);
}
int resizeOptions(double& xResize, double& yResize, Image& image)
{
    // check if the user wants to enter dimensions or ratio of reduction or ratio of increase
    std::string x, y;
    std::string option{};
    std::regex reg{ "[^0-9]+" };
    int choice{};

    std::cout << "Do you want to enter a new dimensions or "
        "enter a ratio of reduction or increase?" << std::endl;
    do {
        std::cout << "1. new dimensions" << std::endl;
        std::cout << "2. ratio of reduction" << std::endl;
        std::cout << "3. ratio of increase" << std::endl;
        std::cout << "Enter your choice:" << std::flush;
        std::getline(std::cin >> std::ws, option);
        if (std::regex_search(option, reg)) {
            std::cout << "Please enter a valid choice" << std::endl;
            continue;
        }
        try {
            choice = std::stoi(option);
        }
        catch (std::invalid_argument&) {
            std::cout << "Please enter a valid choice" << std::endl;
            continue;
        }
        if (choice < 1 or choice > 3) {
            std::cout << "Please enter a valid choice" << std::endl;
        }

    } while (choice < 1 or choice > 3);

    if (choice == 1)
    {
        std::string dimension{};
        while (true)
        {
            std::cout << "Please enter the X dimension:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if (std::regex_search(dimension, reg))
            {
                std::cout << "Please enter a valid dimension" << std::endl;
                continue;
            }
            try {
                xResize = std::stoi(dimension);
                break;
            }
            catch (std::invalid_argument&) {
                std::cout << "Please enter a valid choice" << std::endl;
                continue;
            }
        }

        while (true)
        {
            std::cout << "Please enter the Y dimension:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if (std::regex_search(dimension, reg))
            {
                std::cout << "Please enter a valid dimension" << std::endl;
                continue;
            }
            try {
                yResize = std::stoi(dimension);
                break;
            }
            catch (std::invalid_argument&) {
                std::cout << "Please enter a valid choice" << std::endl;
                continue;
            }
        }
        return 1;
    }
    else if (choice == 2)
    {
        std::string dimension{};
        while (true)
        {
            std::cout << "Enter the ratio of reduction in percent for X:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if (std::regex_search(dimension, reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
            try {
                xResize = std::stoi(dimension);
                if (xResize > 100 or xResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            }
            catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }

        xResize = image.width - std::floor(double(image.width) * xResize / 100.0);
        while (true)
        {
            std::cout << "Enter the ratio of reduction in percent for Y:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if (std::regex_search(dimension, reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
            try {
                yResize = std::stoi(dimension);
                if (yResize > 100 or yResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            }
            catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }
        yResize = image.height - std::floor(double(image.height) * yResize / 100.0);
        return 2;
    }
    else if (choice == 3)
    {
        std::string dimension{};
        while (true)
        {
            std::cout << "Enter the ratio of increase for X:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if (std::regex_search(dimension, reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
            try {
                xResize = std::stoi(dimension);
                if (xResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            }
            catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }
        xResize = std::floor(double(image.width) * xResize / 100.0) + image.width;
        while (true)
        {
            std::cout << "Enter the ratio of increase in percent for Y:" << std::flush;
            std::getline(std::cin >> std::ws, dimension);
            if (std::regex_search(dimension, reg))
            {
                std::cout << "Please enter a valid ratio" << std::endl;
                continue;
            }
            try {
                yResize = std::stoi(dimension);
                if (yResize < 0)
                {
                    throw std::invalid_argument("");
                }
                break;
            }
            catch (std::invalid_argument&) {
                std::cout << "Please enter a valid percent between 0 and 100" << std::endl;
                continue;
            }
        }
        yResize = std::floor(double(image.height) * yResize / 100.0) + image.height;
        return 3;
    }
    return 0;
}

void resizeImage(Image& image, double xResize = 0, double yResize = 0, std::string calledBy = "other")
{
    int option{ 1 };
    if (xResize == 0 and yResize == 0)
    {
        option = resizeOptions(xResize, yResize, image);
    }
    Image resizedImage(xResize, yResize);

    xResize = std::ceil(double(xResize) / image.width * 100) / 100;
    yResize = std::ceil(double(yResize) / image.height * 100) / 100;


    int originalX{};
    int originalY{};
    for (int i = 0; i < resizedImage.width; ++i) {
        for (int j = 0; j < resizedImage.height; ++j) {
            originalX = std::floor(i / xResize);
            originalY = std::floor(j / yResize);
            if (originalX < 0)
            {
                originalX = 0;
            }
            else if (originalX > image.width - 1)
            {
                originalX = image.width - 1;
            }
            if (originalY < 0)
            {
                originalY = 0;
            }
            else if (originalY > image.height - 1)
            {
                originalY = image.height - 1;
            }
            for (int k = 0; k < 3; ++k) {
                resizedImage(i, j, k) = image(originalX, originalY, k);
            }
        }
    }
    std::swap(image.imageData, resizedImage.imageData);
    std::swap(image.width, resizedImage.width);
    std::swap(image.height, resizedImage.height);
    if (calledBy == "main")
    {
        save(image);
    }
}
//this function calculates the box blur sum
int sumcalc(Image& image, int x, int y, int limit, int k)
{
    int sum = 0;
    for (int i = x; i < x + limit; i++)
    {
        for (int j = y; j < y + limit; j++)
        {
            sum += image(i, j, k);
        }
    }
    return sum;
}
void blur(Image& image)
{
    int n = 0, oldw = 0, oldh = 0;
    bool key = 0;
    while (true)
    {
        std::cout << "choose between 1-10\n";
        std::cout << "level of blur: ";
        std::string radius;
        std::cin >> radius;
        try
        {
            n = stoi(radius);
        }
        catch (std::invalid_argument&)
        {
            std::cout << "invalid input\n";
            continue;
        }
        if (11 > n && n > 0)
        {
            break;
        }
        else
        {
            std::cout << "enter a number between 1-10\n";
        }
    }
    //downsampling the image so it decreases the runtime if the image was too big
    if (image.width > 1500 || image.height > 1900)
    {
        //saving the old width and height to upsample the image to the originial form
        oldw = image.width, oldh = image.height;
        resizeImage(image, oldw / 2, oldh / 2);
        //used to detect if the image been downsampled or not
        key = 1;
    }
    //the image where the blurry image will be done on
    //removing the boundaries to avoid any white frame around it
    Image newimage(image.width - 2 * n, image.height - 2 * n);
    int redsum, greensum, bluesum;
    redsum = greensum = bluesum = 0;
    int limit = (2 * n) + 1;
    int avg = pow((2 * n) + 1, 2);
    int redavg, greenavg, blueavg;
    for (int i = n; i < image.width - n; i++)
    {

        for (int j = n; j < image.height - n; j++)
        {
            //sending the start of the matrices to the sum calc and the size of it to make it count the sum
            redsum = sumcalc(image, i - n, j - n, limit, 0);
            greensum = sumcalc(image, i - n, j - n, limit, 1);
            bluesum = sumcalc(image, i - n, j - n, limit, 2);
            redavg = redsum / avg;
            greenavg = greensum / avg;
            blueavg = bluesum / avg;
            newimage(i - n, j - n, 0) = redavg;
            newimage(i - n, j - n, 1) = greenavg;
            newimage(i - n, j - n, 2) = blueavg;

        }
    }
    //if the image been downsampled it will get upsampled again
    if (key)
    {
        resizeImage(newimage, oldw - 2 * n, oldh - 2 * n);
    }
    std::swap(image.imageData, newimage.imageData);
    std::swap(image.width, newimage.width);
    std::swap(image.height, newimage.height);
    save(image);
}
void frame(Image& image)
{
    std::string choice1, choice2, choice3;
    int n = 1, limit = image.width * 0.13, nR, nB, nG, nR2, nG2, nB2;
    nR = nB = nG = nR2 = nG2 = nB2 = 0;
    bool key1 = 1, key2 = 1;
    // the loop breaks when all conditions are done
    while (key1 || key2)
    {
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "| what type of frame do you want? |" << std::endl;
        std::cout << "| 1.simple                2.fancy |" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "enter the number of your choice: ";
        std::cin >> choice1;
        if (choice1 == "1" || choice1 == "2")
        {
            std::cout << "---------------------------------" << std::endl;
            std::cout << "| what frame color do you want? |" << std::endl;
            std::cout << "| 1.red                 2.white |" << std::endl;
            std::cout << "| 3.green               4.black |" << std::endl;
            std::cout << "| 5.blue                        |" << std::endl;
            std::cout << "---------------------------------" << std::endl;
            std::cout<<"enter the number of your choice: ";
            std::cin >> choice2;
            while (true)
            {
                if (choice2 == "1" || choice2 == "2" || choice2 == "3" || choice2 == "4" || choice2 == "5")
                {
                    if (choice1 == "1")
                    {
                        // there is no need to ask for border colors if the frame was simple so we break the loop
                        key1 = key2 = 0;
                        break;
                    }
                    else
                    {
                        //if the frame is fancy then condition 1 is done
                        key1 = 0;
                        break;
                    }
                }
                else
                {
                    std::cout << "invalid input\nenter a valid choice: ";
                    std::cin >> choice2;
                }
            }
        }
        else
        {
            std::cout << "invalid input\n";
            continue;
        }
        if (choice1 == "2")
        {
            std::cout << "----------------------------------------------" << std::endl;
            std::cout << "| what color do you want the border line ?   |" << std::endl;
            std::cout << "| 1.red                 2.white              |" << std::endl;
            std::cout << "| 3.green               4.black              |" << std::endl;
            std::cout << "| 5.blue                                     |" << std::endl;
            std::cout << "----------------------------------------------" << std::endl;
            std::cout << "enter the number of your choice: ";
            std::cin >> choice3;
            while (true)
            {
                if (choice3 == "1" || choice3 == "2" || choice3 == "3" || choice3 == "4" || choice3 == "5")
                {
                    key2 = 0;
                    break;
                }
                else
                {
                    std::cout << "invalid input\nenter a valid choice: ";
                    std::cin >> choice3;
                }
            }
        }

    }
    int intchoice2 = stoi(choice2);
    int intchoice3 = stoi(choice3);
    //switch case for for every chosen color for the frame
    switch (intchoice2)
    {
    case 1:
        nR = 255;
        nB = 0;
        nG = 0;
        break;
    case 2:
        nR = 255;
        nB = 255;
        nG = 255;
        break;
    case 3:
        nR = 0;
        nB = 0;
        nG = 255;
        break;
    case 4:
        nR = 0;
        nB = 0;
        nG = 0;
        break;
    case 5:
        nR = 0;
        nB = 255;
        nG = 0;
        break;
    default:
        break;
    }
    if (choice1 == "2")
    {
        //swwitch case for every color for the border
        switch (intchoice3)
        {
        case 1:
            nR2 = 255;
            nB2 = 0;
            nG2 = 0;
            break;
        case 2:
            nR2 = 255;
            nB2 = 255;
            nG2 = 255;
            break;
        case 3:
            nR2 = 0;
            nB2 = 0;
            nG2 = 255;
            break;
        case 4:
            nR2 = 0;
            nB2 = 0;
            nG2 = 0;
            break;
        case 5:
            nR2 = 0;
            nB2 = 255;
            nG2 = 0;
            break;
        default:
            break;
        }
    }
    // to make sure that the frame and borders are consistent with any image of any size
    int framewidth = image.width * 0.04;
    int borderwidth = image.width * 0.06;
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {

            if (i < framewidth || i >= image.width - framewidth)
            {
                image(i, j, 0) = nR;
                image(i, j, 1) = nG;
                image(i, j, 2) = nB;
                continue;
            }
            else if (j < framewidth || image.height - j < framewidth)
            {
                image(i, j, 0) = nR;
                image(i, j, 1) = nG;
                image(i, j, 2) = nB;
                continue;
            }
            if (choice1 == "2")
            {
                if (i < borderwidth || i >= image.width - borderwidth)
                {
                    image(i, j, 0) = nR2;
                    image(i, j, 1) = nG2;
                    image(i, j, 2) = nB2;
                    n = 0;
                }
                else if (j < limit || image.height - j < limit)
                {
                    image(i, j, 0) = nR2;
                    image(i, j, 1) = nG2;
                    image(i, j, 2) = nB2;
                    image((image.width - 1) - i, j, 0) = nR2;
                    image((image.width - 1) - i, j, 1) = nG2;
                    image((image.width - 1) - i, j, 2) = nB2;
                }
            }
            
        }
        //to draw the triangle at the corners of the border so it decreases with every loop untill it reaches the normal border width
        if (limit > borderwidth && n == 0) --limit;
    }
    save(image);
}
//we treat an image like a matrix here that we rotate
void image_rotation(Image& image)
{
    Image finalimage;
    std::string choice;
    //to get a valid input from user
    while (true)
    {
        std::cout << "----------------------------------------------------------------" << std::endl;
        std::cout << "|do you want to rotate the image by 90, 180 or 270 degrees? |" << std::endl;
        std::cout << "----------------------------------------------------------------" << std::endl;
        std::cin >> choice;
        if (choice == "90" || choice == "180" || choice == "270")
        {
            break;
        }
        else std::cout << "invalid choice" << std::endl;
    }
    if (choice == "90")
    {
        Image newimage(image.height, image.width);
        int n = image.height - 1;
        for (int i = 0; i < image.width; ++i)
        {
            for (int j = 0; j < image.height; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    //when the image got rotated the new width became equal to the height
                    //and to make it start from the top of the image we needed to inverse the j loop (height loop)
                    newimage(n - j, i, k) = image(i, j, k);
                }
            }
        }
        std::swap(image.imageData, newimage.imageData);
        std::swap(image.width, newimage.width);
        std::swap(image.height, newimage.height);
        save(image);
    }
    else if (choice == "180")
    {
        Image newimage(image.width, image.height);
        int n = image.width - 1;
        int y = image.height - 1;
        for (int i = 0; i < image.width; ++i)
        {
            for (int j = 0; j < image.height; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    //image dimensions are the same but the every pixel is inversed
                    newimage(n - i, y - j, k) = image(i, j, k);
                }
            }
        }
        std::swap(image.imageData, newimage.imageData);
        std::swap(image.width, newimage.width);
        std::swap(image.height, newimage.height);
        save(image);

    }
    else if (choice == "270")
    {
        Image newimage(image.height, image.width);
        int n = image.width - 1;
        for (int i = 0; i < image.width; ++i)
        {
            for (int j = 0; j < image.height; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    //dimensions are the same but difference is the pixel starts from the end of the coulmn (old width)
                    newimage(j, n - i, k) = image(i, j, k);
                }
            }
        }
        std::swap(image.imageData, newimage.imageData);
        std::swap(image.width, newimage.width);
        std::swap(image.height, newimage.height);
        save(image);
    }
}

void invertcolor(Image& image)
{
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                //to get the opposite color of the pixel we are on
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
    // swap image data between the flipped image and the loaded image
    // to keep the filter after the function flip return
    std::swap(image.imageData, flipped.imageData);

    save(image);
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
    std::swap(image.imageData, cropped.imageData);
    std::swap(image.width, cropped.width);
    std::swap(image.height, cropped.height);
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

void edgeDetect(Image& image){
    //convert image to grayscale to improve edge detection
    grayScale(image);

    //create new image template to output final image
    Image finalImage(image.width, image.height);

    //set threshhold for gradient
    int threshhold = 300;
    
    //loop through each pixel and calculate diffrence in gradient between it and surrounding pixels
    for(int i = 1; i < image.width - 1; ++i){

        for(int j = 1; j < image.height - 1; ++j){

            //use Sobel operators to calculate horizontal (X) and vertical (Y) gradients
            double gradX = (image(i - 1, j - 1, 0) * -1) + (image(i - 1, j, 0) * -2) + (image(i - 1, j + 1, 0) * -1)
                           + (image(i + 1, j - 1, 0)) + (image(i + 1, j, 0) * 2) + (image(i + 1, j + 1, 0));

            double gradY = (image(i - 1, j - 1, 0)) + (image(i, j - 1, 0) * 2) + (image(i + 1, j - 1, 0))
                           + (image(i - 1, j + 1, 0) * -1) + (image(i, j + 1, 0) * -2) + (image(i + 1, j + 1, 0) * -1);
                           
            //Find final gradient
            double grad = sqrt(pow(gradX, 2) + pow(gradY, 2));

            //if gradient > threshhold edge color is set to black, else edge color is set to white
            int edgeValue = grad > threshhold ? 0 : 255;

            //loop through each pixel in Final image and stamp edge color to it
            for (int k = 0; k < 3; ++k){

                finalImage(i, j, k) = edgeValue;

            }
        }
    }

    std::swap(image.imageData, finalImage.imageData);
    std::swap(image.width, finalImage.width);
    std::swap(image.height, finalImage.height);
    save(image);
}

void mergeCrop(Image& image, int& width, int& height){
    Image newImage(width, height);

    //loop through the image pixels and take the desired width and height
    //then set it to the new image's pixels
    for (int i = 0; i < width; ++i){

        for (int j = 0; j < height; ++j){

            for (int k = 0; k < 3; ++k){
                newImage(i, j, k) = image(i, j, k);
            }
        }
    }
    //swap datas of new image and image for saving image
    std::swap(image.imageData, newImage.imageData);
    std::swap(image.width, newImage.width);
    std::swap(image.height, newImage.height);
}

void mergeNewImage(Image& image){
    //take second image from user
    while(true){
        std::string imageName{};
        std::cout << "Please enter the second image name: " << std::flush;
        std::getline(std::cin >> std::ws, imageName);
        try {
            image.loadNewImage(imageName);
        }
        catch (std::invalid_argument& e) {
            std::cerr << std::flush;
            std::cout << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        break;
    }
}

void merge(Image& image1, Image& image2, int width, int height){
    int color, avg;

    //loop through each pixel's color on both images, calculate their avarage, and set
    //color for image 1 as the avarage color to preform merge to that color
    for (int i = 0; i < width; ++i){

        for (int j = 0; j < height; ++j){

            for (int k = 0; k < 3; ++k){

                color = image1(i, j, k) + image2(i, j, k);
                avg = color / 2;
                image1(i, j, k) = avg;
            }
        }
    }
}

std::string mergeChoice(){
    std::string choice;

    //ask user if how he wants to merge both images
    while (true){
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "| How would you like to merge your images? |" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "1. Resize dimentions of images and merge." << std::endl;
        std::cout << "2. Merge shared portion of both images." << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        //return value based on user choice
        if ("1" <= choice and choice <= "2" and choice.length() == 1){
            return choice;
        }
        std::cout << "Please enter a valid choice." << std::endl;
    }
}

void mergeImages(Image& image){
    Image image2;
    //get second image from user
    mergeNewImage(image2);

    //get choice from user
    std::string choice = mergeChoice();

    int minWidth = std::min(image.width, image2.width);
    int minHeight = std::min(image.height, image2.height);

    int maxWidth = std::max(image.width, image2.width);
    int maxHeight = std::max(image.height, image2.height);

    //if choice is 1, then resize both images to largest dimensions and preform merge
    if (choice == "1"){
        resizeImage(image, maxWidth, maxHeight);
        resizeImage(image2, maxWidth, maxHeight);
        merge(image, image2, maxWidth, maxHeight);
    }
    //if choice is 2, then crop both images to smallest dimensions and preform merge
    else{
        mergeCrop(image, minWidth, minHeight);
        mergeCrop(image2, minWidth, minHeight);
        merge(image, image2, minWidth, minHeight);
    }
    save(image);
}

void naturalSunLight(Image& image)
{
    double factor{1.2};
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            if(image(i,j,0) * factor > 255)
            {
                image(i,j,0) = 255;
            }
            else
            {
                image(i,j,0) = std::round(image(i,j,0) * factor);
            }
            if(image(i,j,1) * factor > 255)
            {
                image(i,j,1) = 255;
            }
            else
            {
                image(i,j,1) = std::round(image(i,j,1) * factor);
            }
        }
    }

}

void purple(Image& image){
    int temp;

    for (int i = 0; i < image.width; ++i){

        for (int j = 0; j < image.height; ++j){

                for(int k = 0; k < 3; ++k){
                    if(k == 0 || k == 2){
                        temp = image(i, j, k) + 50;
                        
                        if(temp > 255){
                            image(i, j, k) = 255;
                        }
                        else{
                            image(i, j, k) = temp;
                        }
                    }
                }
            }
        }
    save(image);
}

int chooseFilter()
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
        std::cout << "3. Invert colors" << std::endl;
        std::cout << "4. Merge Images" << std::endl;
        std::cout << "5. Flip Image" << std::endl;
        std::cout << "6. Crop Image" << std::endl;
        std::cout << "7. Resize Image" << std::endl;
        std::cout << "8. Brighten or Darken Image" << std::endl;
        std::cout << "9. rotate image" << std::endl;
        std::cout << "10. Natural sun light" << std::endl;
        std::cout << "11. Edge detect" << std::endl;
        std::cout << "12. Frames" << std::endl;
        std::cout << "13. Purple" << std::endl;
        std::cout << "14. blur" << std::endl;
        std::cout << "15. infrared" << std::endl;
        std::cout << "16. Return" << std::endl;
        std::cout << "17. Exit the program" << std::endl;
        std::cout << "enter choice:";
        std::getline(std::cin >> std::ws, choice);
        if(choice.length() > 2)
        {
            std::cout << "Please enter a valid choice" << std::endl;
        }
        try {
            int filter = std::stoi(choice);
            if(filter >= 1 and filter <= 17)
            {
                return filter;
            }
            std::cout << "please enter a valid choice" << std::endl;
        }
        catch (std::invalid_argument&) {
            std::cout << "Please enter a valid choice" << std::endl;
        }
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
    // imageStatus == 0 means exit the program, == 1 means image loaded, == -1 means error in loading
    int imageStatus{ 1 };
    Image image;
    // first loop to load a new image
    while (true)
    {
        imageStatus = getImage(image);
        if (imageStatus == 0)
        {
            return 0;
        }
        else if (imageStatus == 1)
        {
            break;
        }
    }
    bool loadNewImage{ false };
    // second loop to do filters on the loaded image
    // and save or discard this image and load a new one
    // depends on the user choice
    while (true)
    {
        if (loadNewImage)
        {
            save(image);
            while (true)
            {
                imageStatus = getImage(image);
                if (imageStatus == 0)
                {
                    return 0;
                }
                else if (imageStatus == 1)
                {
                    break;
                }
            }
        }
        // check what filters does the user want
        int choice{};
        choice = chooseFilter();
        switch (choice) {
        case 1:
            grayScale(image);
            save(image);
            break;
        case 2:
            blackAndWhite(image);
            save(image);
            break;
        case 3:
            invertcolor(image);
            break;
        case 4:
            mergeImages(image);
            break;
        case 5:
            flipImage(image);
            break;
        case 6:
            cropImage(image);
            break;
        case 7:
            resizeImage(image, 0, 0, "main");
            break;
        case 8:
            brightenOrDarken(image);
            break;
        case 9:
            image_rotation(image);
            break;
        case 10:
            naturalSunLight(image);
            save(image);
            break;
        case 11:
            edgeDetect(image);
            break;
        case 12:
            frame(image);
            break;
        case 13:
            purple(image);
        case 14:
            blur(image);
            break;
        case 15:
            infrared(image);
            break;
        case 17:
            save(image);
            std::cout << "------------" << std::endl;
            std::cout << "| GOOD BYE |" << std::endl;
            std::cout << "------------" << std::endl;
            return 0;
        default:
            std::cout << "You did not choose a filter" << std::endl;
            save(image);
        }
        loadNewImage = wantToLoadNew();
    }
}