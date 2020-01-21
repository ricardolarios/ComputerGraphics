#include "PPM.h"
#include <fstream>
#include <string>
#include <iostream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){
    // TODO:    Load and parse a ppm to get its pixel
    //          data stored properly.
    // Each RGB value for a given pixel is stored as 3 consecutive values in the array.
    // For example, to get the pixel at (29, 75):
    // Multiply the maximum length of the row times 3. Times that by the y value of pixel.
    // Add to that x value times 3. 
    // (row_length * 3) * y + x*3
    std::ifstream image_file;
    image_file.open(fileName);

    if(image_file.is_open())
    {
        std::string cur_line;
        int headers_max = 4; // There are 4 "header lines" to get through before the actual data, so process those separately. 
        for (int i = 0; i < headers_max; i++)
        {
            while (std::getline(image_file, cur_line))
            {
                if(i == 2)
                {
                    int width = -1;
                    int height = -1;
                    std::string temp;
                    for (int j = 0; j < cur_line.size(); j++)
                    {
                        if (cur_line[j] == ' ')
                        {
                            width = width < 0 ? atoi(temp.c_str()) : width;
                        }
                    }
                    height = atoi(temp.c_str());
                    m_width = width;
                    m_height = height;
                    std::cout << "Width: " << width << "and Height: " << height << std::endl;
                }
            }
        }

        this->m_PixelData = new unsigned char[m_width * m_height * 3];
        // Now the actual RGB values need to be deciphered
        std::string next_value;
        int i = 0;
        while (image_file >> next_value)
        {
            m_PixelData[i] = atoi(next_value.c_str());
            i++;
        }
    }
   
    image_file.close();
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){
    // TODO: Save a PPM image to disk
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){
    // TODO: Output a 'filtered' PPM image.
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    // TODO: Implement
}
