#include "PPM.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){
    std::ifstream image_file(fileName);
    if (image_file.good())
    {
        std::string cur_line;
        int headers_max = 4; // There are 4 "header lines" to get through before the actual data, so process those separately. 
        for (int i = 0; i < headers_max; i++)
        {
            std::getline(image_file, cur_line);
            
            // TODO: Change this.
            if (i == 2)
            {
                // Parse the line to get the width and height parameters
                int width = -1;
                int height = -1;
                std::string temp;
                for (int j = 0; j < cur_line.size(); j++)
                {
                    if (cur_line[j] == ' ')
                    {
                        width = width < 0 ? atoi(temp.c_str()) : width;
                        temp.clear();
                    }
                    temp.push_back(cur_line[j]);
                }
                height = atoi(temp.c_str()); // Finished reading this line, so set height.

                assert(width > 0 && height > 0); // Make sure that dimensions initialized
                m_width = width;
                m_height = height;
            }
        }
        
        // Initialize pixel array and ingest all of the pixel data
        this->m_PixelData = new unsigned char[(size_t)m_width * m_height * 3];
        int next_value;
        size_t i = 0;
        while (image_file >> next_value)
        {
            m_PixelData[i] = next_value;
            i++;
        }
    }
   
    image_file.close();
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
    delete[] this->m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){
    // TODO: Save a PPM image to disk
    std::ofstream outFile(outputFileName);
    
    if (outFile.good())
    {
        // Write "P3"
        outFile << "P3\n";
        // Comment
        outFile << "# PPM comment\n";
        // Width Height
        outFile << m_width << " " << m_height << '\n';
        // Max value of this 
        outFile << 255 << '\n';
        // Then write each RGB pixel data to a line.
        for (size_t i = 0; i < (size_t)m_width * m_height * 3; i++)
        {
            outFile << (int) this->m_PixelData[i] << '\n';
        }
    }

    outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){
    size_t max_size = m_width * m_height * 3;
    for (size_t i = 0; i < max_size; i++)
    {
        if (this->m_PixelData[i] >= 50)
        {
            this->m_PixelData[i] -= 50;
        }
        else
        {
            this->m_PixelData[i] = 0;
        }
    }
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    // The pixel values are kept in a single array and each RGB value is just kept as 3 consecutive members.
    // So to get the first of any pixel value, we must multiply the x value by 3.
    // We add that to the y * 3 * width to account for all of the previous rows of pixels.
    size_t index = (x * 3) + (y * 3 * m_width);
    this->m_PixelData[index] = R;
    this->m_PixelData[index + 1] = G;
    this->m_PixelData[index + 2] = B;
}
