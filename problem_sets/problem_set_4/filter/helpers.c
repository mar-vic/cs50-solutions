 #include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
        {
          int originalRed = image[i][j].rgbtRed;
          int originalGreen = image[i][j].rgbtGreen;
          int originalBlue = image[i][j].rgbtBlue;

          // Dtermine the brightness of greyscaled pixel by averaging the
          // RGB values of original pixel and rounding the result to nearest
          // integer.
          int grsc_val =  (int) round((float) (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);

          // Greyscale the original RGB values.
          image[i][j].rgbtRed = grsc_val;
          image[i][j].rgbtGreen = grsc_val;
          image[i][j].rgbtBlue = grsc_val;
        }
    }
  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
   int half_width = width / 2;

  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < half_width; j++)
        {
          // Remember the RGB values of the left pixel
          RGBTRIPLE orig_left;
          orig_left.rgbtRed = image[i][j].rgbtRed;
          orig_left.rgbtGreen = image[i][j].rgbtGreen;
          orig_left.rgbtBlue = image[i][j].rgbtBlue;

          // Store the RGB values of reflected right pixel in the left one
          int reflected_index = (width - 1) - j;
          image[i][j].rgbtRed = image[i][reflected_index].rgbtRed;
          image[i][j].rgbtGreen = image[i][reflected_index].rgbtGreen;
          image[i][j].rgbtBlue = image[i][reflected_index].rgbtBlue;

          // Store the RGB values of left pixel in its right side reflection
          image[i][reflected_index].rgbtRed = orig_left.rgbtRed;
          image[i][reflected_index].rgbtGreen = orig_left.rgbtGreen;
          image[i][reflected_index].rgbtBlue = orig_left.rgbtBlue;
        }
    }

  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
  // Make a copy of the bitmap
  RGBTRIPLE image_copy[height][width];

  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
        {
          image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
          image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
          image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

  // Blur the pixels in the original image
  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
        {
          // Calculate the average RGB values of sourrounding pixels
          int aveRed = 0;
          int aveGreen = 0;
          int aveBlue = 0;

          int gridHeight = 3;
          int gridWidth = 3;

          // Modify dimensions of surroudning grid, if pixel is at some edge
          if ((i -1) < 0) // Test for upper edge
            {
              gridHeight--;
            }

          if ((i + 1) == height) // Test for lower
            {
              gridHeight--;
            }

          if ((j - 1) < 0) // Test for left edge
            {
              gridWidth--;
            }

          if ((j + 1) == width)
            {
              gridWidth--;
            }

          for (int k = (i - 1); k <= (i + 1); k++)
            {
              // Skip a cycle, if the pixel is at upper/lower edge
              if (k < 0 || k == height)
                {
                  continue;
                }
              for (int l = (j - 1); l <= (j + 1); l++)
                {
                  // Skip a cycle, if the pixel is at left/right edge
                  if (l < 0 || l == width)
                    {
                      continue;
                    }
                  aveRed += image_copy[k][l].rgbtRed;
                  aveGreen += image_copy[k][l].rgbtGreen;
                  aveBlue += image_copy[k][l].rgbtBlue;
                }
            }

          int gridProduct = gridWidth * gridHeight;

          // Calculate the average RGB values
          aveRed = (int) round((float) aveRed / gridProduct);
          aveGreen = (int) round((float) aveGreen / gridProduct);
          aveBlue = (int) round((float) aveBlue / gridProduct);

          // Blur the pixel in the original bitmap
          image[i][j].rgbtRed = aveRed;
          image[i][j].rgbtGreen = aveGreen;
          image[i][j].rgbtBlue = aveBlue;
        }
    }

  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
  // Make a copy of the bitmap
  RGBTRIPLE image_copy[height][width];

  // Initialize kernels for calculating weighted sums
  int gXKernel[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
  int gYKernel[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
        {
          image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
          image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
          image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

  // Generate the edges
  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
        {
          int wsumRedX = 0;
          int wsumGreenX = 0;
          int wsumBlueX = 0;

          int wsumRedY = 0;
          int wsumGreenY = 0;
          int wsumBlueY = 0;

          for (int k = (i - 1); k <= (i + 1); k++)
            {
              // Assume black border if pixel is at the upper/lower. This means
              // that wieghted sum of RGB values stays unchanged.
              if (k < 0 || k == height)
                {
                  continue;
                }
              for (int l = (j - 1); l <= (j + 1); l++)
                {
                  // Skip a cycle, if the pixel is at left/right edge
                  if (l < 0 || l == width)
                    {
                      continue;
                    }

                  // Indexes for kernels
                  int kX = k - (i - 1);
                  int kY = l - (j - 1);

                  // Store original RGB values for testing purposes
                  int red = image_copy[k][l].rgbtRed;
                  int green = image_copy[k][l].rgbtGreen;
                  int blue = image_copy[k][l].rgbtBlue;

                  // Calculate parts of weighted sums
                  wsumRedX += gXKernel[kX][kY] * image_copy[k][l].rgbtRed;
                  wsumGreenX += gXKernel[kX][kY] * image_copy[k][l].rgbtGreen;
                  wsumBlueX += gXKernel[kX][kY] * image_copy[k][l].rgbtBlue;

                  wsumRedY += gYKernel[kX][kY] * image_copy[k][l].rgbtRed;
                  wsumGreenY += gYKernel[kX][kY] * image_copy[k][l].rgbtGreen;
                  wsumBlueY += gYKernel[kX][kY] * image_copy[k][l].rgbtBlue;

                  // Print calculations for corners for testing purposes
                  if ((i == 0 && j == 0) || (i == 0 && j == (width - 1)) || (i == (height - 1) && j == 0) || (i == (height - 1) && j == (width - 1)))
                    {
                      printf("[%i,%i] : R = %i, G = %i, B = %i\n", kX, kY, red, green, blue);
                    }
                }
            }

          // Combine the wieghted sums
          int finalRed = (int) round(sqrt((float) ((wsumRedX * wsumRedX) + (wsumRedY * wsumRedY))));
          int finalGreen = (int) round(sqrt((float) ((wsumGreenX * wsumGreenX) + (wsumGreenY * wsumGreenY))));
          int finalBlue = (int) round(sqrt((float) ((wsumBlueX * wsumBlueX) + (wsumBlueY * wsumBlueY))));

          // Cap the sums at 255
          if (finalRed > 255)
            {
              finalRed = 255;
            }

          if (finalGreen > 255)
            {
              finalGreen = 255;
            }

          if (finalBlue > 255)
            {
              finalBlue = 255;
            }

          // Store the edges in the original bitmap
          image[i][j].rgbtRed = finalRed;
          image[i][j].rgbtGreen = finalGreen;
          image[i][j].rgbtBlue = finalBlue;

          if ((i == 0 && j == 0) || (i == 0 && j == (width - 1)) || (i == (height - 1) && j == 0) || (i == (height - 1) && j == (width - 1)))
            {
              printf("Weighted sums: R = %i, G = %i, B = %i\n\n", finalRed, finalGreen, finalBlue);
            }
        }
    }

  return;
}
