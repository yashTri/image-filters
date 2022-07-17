#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    WORD w;//WORD to store sum of rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            w = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            image[i][j].rgbtBlue = w / 3 + (w % 3 >
                                            1);//Rounding of by checking if the remainder is bigger than 1, and making all rgb vals equal
            image[i][j].rgbtGreen = w / 3 + (w % 3 > 1);
            image[i][j].rgbtRed = w / 3 + (w % 3 > 1);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;//temporary RGBTRIPLE to perform swap
    int st, en;
    for (int i = 0; i < height; i++)
    {
        st = 0;
        en = width - 1;
        while (st < en)//swapping values till st and en meet at the middle or cross each other
        {
            temp = image[i][st];
            image[i][st] = image[i][en];
            image[i][en] = temp;
            st++;
            en--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int neighbours;
    int wb, wg, wr;
    int nx, ny;
    int a, b, c;
    int dirs[8][2] =
    {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1},
        {1, 1},
        {-1, -1},
        {1, -1},
        {-1, 1}
    };//array that stores different directions
    RGBTRIPLE new_image[height][width];//new image as if we make changes to the old one, our calculations would become incorrect
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            neighbours = 1;
            wb = image[i][j].rgbtBlue;
            wg = image[i][j].rgbtGreen;
            wr = image[i][j].rgbtRed;
            for (int k = 0; k < 8; k++)
            {
                nx = i + dirs[k][0];
                ny = j + dirs[k][1];
                if (nx < height && nx >= 0 && ny < width && ny >= 0)//checking if the neighbour co-ordinates are valid
                {
                    neighbours++;
                    wb += image[nx][ny].rgbtBlue;
                    wg += image[nx][ny].rgbtGreen;
                    wr += image[nx][ny].rgbtRed;
                }
            }
            a = (int)round(wb * 1.0 / neighbours);
            b = (int)round(wg * 1.0 / neighbours);
            c = (int)round(wr * 1.0 / neighbours);
            new_image[i][j].rgbtBlue = (BYTE)a;
            new_image[i][j].rgbtGreen = (BYTE)b;
            new_image[i][j].rgbtRed = (BYTE)c;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];//copying new_image to image
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int nx, ny;
    RGBTRIPLE new_image[height][width];
    int dirs[8][2] =
    {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1},
        {1, 1},
        {-1, -1},
        {1, -1},
        {-1, 1}
    };//same as blur
    int xwt[8] = {0, 2, 0, -2, 1, -1, -1, 1};//weight values for different dirs for calculating gx
    int ywt[8] = {2, 0, -2, 0, 1, -1, 1, -1};//weight values for different dirs for calculating gx
    int gxb, gxg, gxr, gyb, gyg, gyr, gb, gr, gg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gxb = 0;
            gxr = 0;
            gxg = 0;
            gyr = 0;
            gyg = 0;
            gyb = 0;
            for (int k = 0; k < 8; k++)
            {
                nx = i + dirs[k][0];
                ny = j + dirs[k][1];
                if (nx < height && nx >= 0 && ny < width && ny >= 0)
                {
                    gxb += ((int)image[nx][ny].rgbtBlue) * xwt[k];
                    gxg += ((int)image[nx][ny].rgbtGreen) * xwt[k];
                    gxr += ((int)image[nx][ny].rgbtRed) * xwt[k];
                    gyb += ((int)image[nx][ny].rgbtBlue) * ywt[k];
                    gyg += ((int)image[nx][ny].rgbtGreen) * ywt[k];
                    gyr += ((int)image[nx][ny].rgbtRed) * ywt[k];
                }
            }
            gb = gxb * gxb + gyb * gyb;
            gr = gxr * gxr + gyr * gyr;
            gg = gxg * gxg + gyg * gyg;
            gb = (int)round(sqrt(gb));//this is the new value for blue
            gr = (int)round(sqrt(gr));//for red
            gg = (int)round(sqrt(gg));//for green
            if (gb < 255)//checking if gb is greater than max value of rgbtBlue
            {
                new_image[i][j].rgbtBlue = (BYTE)gb;
            }
            else
            {
                new_image[i][j].rgbtBlue = (BYTE)255;
            }
            if (gg < 255)//same as blue
            {
                new_image[i][j].rgbtGreen = (BYTE)gg;
            }
            else
            {
                new_image[i][j].rgbtGreen = (BYTE)255;
            }
            if (gr < 255)//same as blue
            {
                new_image[i][j].rgbtRed = (BYTE)gr;
            }
            else
            {
                new_image[i][j].rgbtRed = (BYTE)255;
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
    return;
}
