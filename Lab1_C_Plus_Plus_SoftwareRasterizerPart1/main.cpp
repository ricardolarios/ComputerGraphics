/** @file main.cpp
 *  @brief Entry point into our program.
 *  
 *  Welcome to the Great Looking Software Render 
 *  code base (Yes, I needed something with a gl prefix).
 *
 *  This is where you will implement your graphics API.
 *
 *  Compile on the terminal with: 
 *
 *  g++ -std=c++11 main.cpp -o main
 *
 *  @author Mike Shah
 *  @bug No known bugs.
 */

// Load our libraries
#include <iostream>
#include<bits/stdc++.h> 

// Some define values
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 320

// C++ Standard Libraries
#include <iostream>

// User libraries
#include "GL.h"
#include "Color.h"
#include "TGA.h"
#include "Maths.h"

// Create a canvas to draw on.
TGA canvas(WINDOW_WIDTH,WINDOW_HEIGHT);


// Implementation of Bresenham's Line Algorithm
// The input to this algorithm is two points and a color
// This algorithm will then modify a canvas (i.e. image)
// filling in the appropriate colors.
void drawLine(Vec2 v0, Vec2 v1, TGA& image, ColorRGB c){
    bool steep = false;
    if(std::abs(v0.x-v1.x)<std::abs(v0.y-v1.y)){
        // If the line is steep we want to transpose the image.
        std::swap(v0.x,v0.y);
        std::swap(v1.x,v1.y);
        steep = true;
    }
    if(v0.x>v1.x){  // make it left-to-right
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }
    /*
    std::cout << "Vector1: " << v0.x << ", " << v0.y << '\n';
    std::cout << "Vector2: " << v1.x << ", " << v1.y << '\n';
    */
    for(int x = v0.x; x <= v1.x; ++x){
        float t = (x-v0.x)/(float)(v1.x-v0.x);
        int y = v0.y*(1.0f-t) + v1.y*t;
        if(steep){
            canvas.setPixelColor(y,x,c);
        }else{
            canvas.setPixelColor(x,y,c);
        }
    }
}

int compareY(Vec2 first, Vec2 second)
{
    return first.y - second.y;
}

void fillBottomFlatTriangle(Vec2 v0, Vec2 v1, Vec2 v2, TGA& image, ColorRGB c)
{
    float invslope1 = ((float)v1.x - (float)v0.x) / ((float)v1.y - (float)v0.y);
    float invslope2 = ((float)v2.x - (float)v0.x) / ((float)v2.y - (float)v0.y);

    float curx1 = v0.x;
    float curx2 = v0.x;

    for (int scanlineY = v0.y; scanlineY <= v1.y; scanlineY++)
    {
        Vec2* draw_vec_1 = new Vec2(curx1, scanlineY);
        Vec2* draw_vec_2 = new Vec2(curx2, scanlineY);

        drawLine(*draw_vec_1, *draw_vec_2, image, c);
        curx1 += invslope1;
        curx2 += invslope2;
        delete draw_vec_1;
        delete draw_vec_2;
    }
}

void fillTopFlatTriangle(Vec2 v0, Vec2 v1, Vec2 v2, TGA& image, ColorRGB c)
{
    float invslope1 = ((float)v2.x - (float)v0.x) / ((float)v2.y - (float)v0.y);
    float invslope2 = ((float)v2.x - (float)v1.x) / ((float)v2.y - (float)v1.y);

    float curx1 = v2.x;
    float curx2 = v2.x;

    for (int scanlineY = v2.y; scanlineY > v0.y; scanlineY--)
    {
        Vec2* draw_vec_1 = new Vec2(curx1, scanlineY);
        Vec2* draw_vec_2 = new Vec2(curx2, scanlineY);

        drawLine(*draw_vec_1, *draw_vec_2, image, c);
        curx1 -= invslope1;
        curx2 -= invslope2;
        delete draw_vec_1;
        delete draw_vec_2;
    }
}

// Draw a triangle
void triangle(Vec2 v0, Vec2 v1, Vec2 v2,TGA& image, ColorRGB c){
    if(glFillMode==LINE){
        drawLine(v0,v1,image,c);
        drawLine(v1,v2,image,c);
        drawLine(v2,v0,image,c);
    }
    // TODO: Draw a filled triangle
    else
    {
        Vec2 vectors[3] = { v0, v1, v2 };
        
        for (int i = 0; i < 3; i++)
        {
            for (int j = i + 1; j < 3; j++)
            {
                if (vectors[j].has_smaller_y(vectors[i]))
                {
                    Vec2 temp = vectors[i];
                    vectors[i] = vectors[j];
                    vectors[j] = temp;
                }
            }
        }
        
        if (vectors[1].y == vectors[2].y)
        {
            fillBottomFlatTriangle(vectors[0], vectors[1], vectors[2], image, c);
        }
        else if (vectors[0].y == vectors[1].y)
        {
            fillTopFlatTriangle(vectors[0], vectors[1], vectors[2], image, c);
        }
        else
        {
            Vec2* v3 = new Vec2(
                (int)(vectors[0].x + ((float)(vectors[1].y - vectors[0].y) / (float)(vectors[2].y - vectors[0].y)) * (vectors[2].x - vectors[0].x)), vectors[1].y);

            fillBottomFlatTriangle(vectors[0], vectors[1], *v3, image, c);
            fillTopFlatTriangle(vectors[1], *v3, vectors[2], image, c);
        }


       // Algorithm shit
    }
}



// Main
int main(){

    // A sample of color(s) to play with
    ColorRGB red;
    red.r = 255; red.g = 0; red.b = 0;
        
    
    // Points for our Line
    Vec2 line[2] = {Vec2(0,0), Vec2(100,100)};

    // Set the fill mode
    glPolygonMode(FILL);

    // Draw a line
    drawLine(line[0],line[1],canvas,red);

    // Data for our triangle
    Vec2 tri[3] = {Vec2(160,60),Vec2(150,10),Vec2(75,190)};

    // Draw a triangle
    triangle(tri[0],tri[1],tri[2],canvas,red);
    triangle(Vec2(300, 60), Vec2(290, 10), Vec2(60, 100), canvas, red);
    triangle(Vec2(200, 200), Vec2(290, 150), Vec2(140, 170), canvas, red);


    // Output the final image
    canvas.outputTGAImage("graphics_lab2.ppm");

    return 0;
}
