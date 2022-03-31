#include "filter_functions.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // get number of pixels in each image
    // 2d array of pixels
    // average of rgbt.red,green,blue and round
    // assign average to all rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;

            float average = round((red + green + blue)/3);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Swap pixels horizontally
    for (int i = 0; i < height; i++)
    {
        for (int j = 0;  j < width/2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j+1)];
            image[i][width - (j+1)] = temp;
        }
    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make copy of image
    // do calculations with origianl image
    RGBTRIPLE image_copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = 0;
            float green = 0;
            float blue = 0;
            float counter = 0;
            for (int k = -1; k < 2; k++)
            {
                for (int m = -1; m < 2; m++)
                {
                    // check if pixel is in array
                    if ((i+k >= 0) && (i+k < height) && (j+m >= 0) && (j+m < width))
                    {
                        RGBTRIPLE pixel = image[i+k][j+m];

                        red += pixel.rgbtRed;
                        green += pixel.rgbtGreen;
                        blue += pixel.rgbtBlue;
                        counter++;
                    }
                }
            }
            // get average and assign it
            // set calculations to new image (don't edit original)
            image_copy[i][j].rgbtRed = round(red/counter);
            image_copy[i][j].rgbtGreen = round(green/counter);
            image_copy[i][j].rgbtBlue = round(blue/counter);
        }
    }
    //after all is done, make original be qual to new image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image_copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = image_copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = image_copy[i][j].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
// For each pixel
// take the surrouding pixels of each pixel.
//  Computee G(x)
//  if its an edge/corner pixel, the surrouding pixels may be beyond that image, so:
//  treat it as a solid black pixel - 0 for all rgb values (its will ignore it in the calcs)
//  Multiply each value by the corresponding value in the Gx kernel
//  take the sum of the correspoding values
// Compute G(y)
//  if its an edge/corner pixel, the surrouding pixels may be beyond that image, so:
//  treat it as a solid black pixel - 0 for all rgb values (its will ignore it in the calcs)
//  Multiply each value by the corresponding value in the Gy kernel
//  take the sum of the correspoding values

    RGBTRIPLE image_copy[height][width];

for (int i = 0; i < height; i++)
{
    for (int j = 0; j < width; j++)
    {
        int gx_red = 0;
        int gx_green = 0;
        int gx_blue = 0;

        int gy_red = 0;
        int gy_green = 0;
        int gy_blue = 0;

        for (int k = -1; k < 2; k++)
        {
            for (int m = -1; m < 2; m++)
            {
                if ((i+k < height) && (i+k >= 0) && (j+m < width) && (j+m >= 0))
                {
                    //g(x)
                    if (k != 0) // if pixel is on top or bottom of current pixel
                    {
                    gx_red += m * image[i+k][j+m].rgbtRed;
                    gx_green += m * image[i+k][j+m].rgbtGreen;
                    gx_blue += m * image[i+k][j+m].rgbtBlue;
                    }
                    else
                    {
                        if (m == -1) //if pixel is on the left of current pixel (mid row)
                        {
                            gx_red += -2 * image[i+k][j+m].rgbtRed;
                            gx_green += -2 * image[i+k][j+m].rgbtGreen;
                            gx_blue += -2 * image[i+k][j+m].rgbtBlue;
                        }
                        else if (m == 1)
                        {
                            gx_red += 2 * image[i+k][j+m].rgbtRed;
                            gx_green += 2 * image[i+k][j+m].rgbtGreen;
                            gx_blue += 2 * image[i+k][j+m].rgbtBlue;
                        }
                        else //current pixel (m == 0)
                        {
                            gx_red += 0 * image[i+k][j+m].rgbtRed;
                            gx_green += 0 * image[i+k][j+m].rgbtGreen;
                            gx_blue += 0 * image[i+k][j+m].rgbtBlue;
                        }
                    }
                    // g(y)
                    if (m != 0)
                    {
                    gy_red += k * image[i+k][j+m].rgbtRed;
                    gy_green += k * image[i+k][j+m].rgbtGreen;
                    gy_blue += k * image[i+k][j+m].rgbtBlue;
                    }
                    else
                    {
                        if (k == -1)
                        {
                            gy_red += -2 * image[i+k][j+m].rgbtRed;
                            gy_green += -2 * image[i+k][j+m].rgbtGreen;
                            gy_blue += -2 * image[i+k][j+m].rgbtBlue;
                        }
                        else if (k == 1)
                        {
                            gy_red += 2 * image[i+k][j+m].rgbtRed;
                            gy_green += 2 * image[i+k][j+m].rgbtGreen;
                            gy_blue += 2 * image[i+k][j+m].rgbtBlue;
                        }
                        else
                        {
                            gy_red += 0 * image[i+k][j+m].rgbtRed;
                            gy_green += 0 * image[i+k][j+m].rgbtGreen;
                            gy_blue += 0 * image[i+k][j+m].rgbtBlue;
                        }
                    }
                }
            }
        }
        float red = round(sqrt((gx_red * gx_red) + (gy_red * gy_red)));
        float green = round(sqrt((gx_green * gx_green) + (gy_green * gy_green)));
        float blue = round(sqrt((gx_blue * gx_blue) + (gy_blue * gy_blue)));

        //cap value if greater than 255 maintain normal RGB colors
        if (red > 255)
        {
        red = 255;
        }
        if (blue > 255)
        {
        blue = 255;
        }
        if (green > 255)
        {
        green = 255;
        }
        image_copy[i][j].rgbtRed = red;
        image_copy[i][j].rgbtGreen = green;
        image_copy[i][j].rgbtBlue = blue;
    }
}
    //Set original image to new temporary image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image_copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = image_copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = image_copy[i][j].rgbtBlue;
        }
    }
    return;
}


// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //Calculate each new color value using serpia formula
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            // cap value if > 255 to maintain RGB colors
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

