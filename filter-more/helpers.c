#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterating through the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Finding the average
            int average = round(((float)image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Getting the middle, using the int because im rounding down
    int middle = width / 2;
    RGBTRIPLE temp;
    // Iterating throuht pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < middle; j++)
        {
            // integer for the last pixel
            int l = width - 1 - j;
            // Swapping pixels
            temp = image[i][j];
            image[i][j] = image[i][l];
            image[i][l] = temp;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Setting up variables for later use
    RGBTRIPLE temp_image[height][width];
    RGBTRIPLE average;
    // Iterating through pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // setting the variables for each pixel
            int counter = 0;
            int sumBlue = 0;
            int sumGreen = 0;
            int sumRed = 0;
            // setting up 3x3 box of pixels
            int x[] = {i - 1, i, i + 1};
            int y[] = {j - 1, j, j + 1};
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    // getting the pixel in the box
                    int curX = x[k];
                    int curY = y[l];
                    // out of bounds control
                    if (curX < 0 || curX > height - 1 || curY < 0 || curY > width - 1)
                    {
                        continue;
                    }
                    else
                    {
                        // getting the sums of colors
                        sumBlue += image[curX][curY].rgbtBlue;
                        sumGreen += image[curX][curY].rgbtGreen;
                        sumRed += image[curX][curY].rgbtRed;
                        counter++;
                    }

                }
            }

            // calculating average
            average.rgbtBlue = round((float)sumBlue / counter);
            average.rgbtGreen = round((float)sumGreen / counter);
            average.rgbtRed = round((float)sumRed / counter);
            // putting on a temp image
            temp_image[i][j] = average;


        }
    }
    // making a new image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp_image[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // arrays for the box calculations
    int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Temp image
    RGBTRIPLE temp_image[height][width];
    // iterating through pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // setting up variables
            int gxBlue = 0;
            int gxGreen = 0;
            int gxRed = 0;

            int gyBlue = 0;
            int gyGreen = 0;
            int gyRed = 0;
            // getting the 3x3 box
            int x[] = {i - 1, i, i + 1};
            int y[] = {j - 1, j, j + 1};

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    // current pixel in box
                    int curX = x[k];
                    int curY = y[l];
                    // out of bounds control
                    if ((curX >= 0 && curX < height) && (curY >= 0 && curY < width))
                    {
                        // adding to colors
                        gxBlue += image[curX][curY].rgbtBlue * GX[k][l];
                        gxGreen += image[curX][curY].rgbtGreen * GX[k][l];
                        gxRed += image[curX][curY].rgbtRed * GX[k][l];

                        gyBlue += image[curX][curY].rgbtBlue * GY[k][l];
                        gyGreen += image[curX][curY].rgbtGreen * GY[k][l];
                        gyRed += image[curX][curY].rgbtRed * GY[k][l];

                    }
                }
                // calculating the gx and gy algorithm
                int totalBlue = round(sqrt(((float)gxBlue * gxBlue) + (gyBlue * gyBlue)));
                int totalGreen = round(sqrt(((float)gxGreen * gxGreen) + (gyGreen * gyGreen)));
                int totalRed = round(sqrt(((float)gxRed * gxRed) + (gyRed * gyRed)));
                // making sure the total doesnt exceed the hextabe
                if (totalBlue > 255)
                {
                    totalBlue = 255;
                }
                if (totalGreen > 255)
                {
                    totalGreen = 255;
                }
                if (totalRed > 255)
                {
                    totalRed = 255;
                }
                // Adding to a temp image
                temp_image[i][j].rgbtBlue = totalBlue;
                temp_image[i][j].rgbtGreen = totalGreen;
                temp_image[i][j].rgbtRed = totalRed;
            }

        }
    }
    // putting temp image data into the new image data
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
        }
    }




    return;
}

