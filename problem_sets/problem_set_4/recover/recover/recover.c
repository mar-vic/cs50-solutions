#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

// Prototypes
int seek_first_jpeg(FILE *image);
int seek_next_jpeg(FILE *image);
int is_at_jpeg(FILE *image);
int recover_jpeg(FILE *image, char* outfile);

int main(int argc, char *argv[])
{
  // Ensure proper usage
  if (argc != 2)
    {
      fprintf(stderr, "Usage: ./recover image\n");
      return 1;
    }

  // Remember image file name
  char* imgfile = argv[1];

  // Open image file
  FILE *image = fopen(imgfile, "r");
  if (image == NULL)
    {
      fprintf(stderr, "Could not open image file %s.\n", imgfile);
      return 1;
    }

  int fcount = 0;
  char outfile[10];
  int firstDigit;
  int secondDigit;
  int thirdDigit;

  seek_first_jpeg(image);

  while (feof(image) == 0 && fcount < 60)
    {
      firstDigit = fcount / 100;
      secondDigit = (fcount / 10) - (10 * firstDigit);
      thirdDigit = fcount - firstDigit * 100 - secondDigit * 10;

      sprintf(outfile, "%i%i%i.jpeg", firstDigit, secondDigit, thirdDigit);
      if(recover_jpeg(image, outfile) == -1)
        {
          break;
        }
      fcount++;
    }

  return 0;
}

// Set file position indicator at the beginning of next jpeg segment. Return
// 1 if such segment is found. Otherwise, return 0.
int seek_first_jpeg(FILE *image)
{
  BYTE first;
  BYTE second;
  BYTE third;
  BYTE fourth;

  while (1)
    {
      fread(&first, sizeof(BYTE), 1, image);
      fread(&second, sizeof(BYTE), 1, image);
      fread(&third, sizeof(BYTE), 1, image);
      fread(&fourth, sizeof(BYTE), 1, image);

      if (first == 0xff
          && second == 0xd8
          && third == 0xff
          && (fourth == 0xe0 || fourth == 0xe1 || fourth == 0xe2 || fourth == 0xe3
              || fourth == 0xe4 || fourth == 0xe5 || fourth == 0xe6 || fourth == 0xe7
              || fourth == 0xe8 || fourth == 0xe9 || fourth == 0xea || fourth == 0xeb
              || fourth == 0xec || fourth == 0xed || fourth == 0xee || fourth == 0xef))
        {
          // If jpeg segment is discovered, set file position indicator at the first byte of
          // jpeg segment.
          fseek(image, -4, SEEK_CUR);
          return 1;
        }

      if(feof(image) == 0)
        {
          fseek(image, -3, SEEK_CUR);
        }
      else
        {
          return 0;
        }
    }
}

int seek_next_jpeg(FILE *image)
{
  BYTE first;
  BYTE second;
  BYTE third;
  BYTE fourth;

  // Check wether the file position indicator is at the beginning of an jpeg
  // segment.
  fread(&first, sizeof(BYTE), 1, image);
  fread(&second, sizeof(BYTE), 1, image);
  fread(&third, sizeof(BYTE), 1, image);
  fread(&fourth, sizeof(BYTE), 1, image);

  if (feof(image) == 1)
    {
      return -1;
    }

  if (first != 0xff
      || second != 0xd8
      || third != 0xff
      || (fourth != 0xe0 && fourth != 0xe1 && fourth != 0xe2 && fourth != 0xe3
          && fourth != 0xe4 && fourth != 0xe5 && fourth != 0xe6 && fourth != 0xe7
          && fourth != 0xe8 && fourth != 0xe9 && fourth != 0xea && fourth != 0xeb
          && fourth != 0xec && fourth != 0xed && fourth != 0xee && fourth != 0xef))
    {
      /* printf("First byte: %X\nSecond byte: %X\nThird byte: %X\nFourth byte: %X\n", first, second, third, fourth); */
      /* printf("Position indicator is not at the beginning of an jpeg segment\n"); */
      /* fseek(image, -4, SEEK_CUR); */
      return 0;
    }

  // Set the position indicator back at the beginning of jpeg segment
  // and remember it.
  fseek(image, -4, SEEK_CUR);

  while (1)
    {
      // Jump by 512 bytes long segments.
      fseek(image, 512, SEEK_CUR);

      // Read next three bytes
      fread(&first, sizeof(BYTE), 1, image);
      fread(&second, sizeof(BYTE), 1, image);
      fread(&third, sizeof(BYTE), 1, image);
      fread(&fourth, sizeof(BYTE), 1, image);

      // Check wether there is an jpeg in the next segment.
      if (first == 0xff
          && second == 0xd8
          && third == 0xff
          && (fourth == 0xe0 || fourth == 0xe1 || fourth == 0xe2 || fourth == 0xe3
              || fourth == 0xe4 || fourth == 0xe5 || fourth == 0xe6 || fourth == 0xe7
              || fourth == 0xe8 || fourth == 0xe9 || fourth == 0xea || fourth == 0xeb
              || fourth == 0xec || fourth == 0xed || fourth == 0xee || fourth == 0xef))
        {
          // If jpeg is discovered, set the file position indicator at its
          // beginning and return 1
          fseek(image, -4, SEEK_CUR);
          return 1;
        }

      if(feof(image) == 0)
        {
          fseek(image, -4, SEEK_CUR);
        }
      else
        {
          return -1;
        }
    }
}

int is_at_jpeg(FILE *image)
{
  BYTE first;
  BYTE second;
  BYTE third;
  BYTE fourth;

  fread(&first, sizeof(BYTE), 1, image);
  fread(&second, sizeof(BYTE), 1, image);
  fread(&third, sizeof(BYTE), 1, image);
  fread(&fourth, sizeof(BYTE), 1, image);

  if (first == 0xff
      && second == 0xd8
      && third == 0xff
      && (fourth == 0xe0 || fourth == 0xe1 || fourth == 0xe2 || fourth == 0xe3
          || fourth == 0xe4 || fourth == 0xe5 || fourth == 0xe6 || fourth == 0xe7
          || fourth == 0xe8 || fourth == 0xe9 || fourth == 0xea || fourth == 0xeb
          || fourth == 0xec || fourth == 0xed || fourth == 0xee || fourth == 0xef))
    {
      fseek(image, -4, SEEK_CUR);
      return 0;
    }
  else
    {
      fseek(image, -4, SEEK_CUR);
      return 1;
    }
}

int recover_jpeg(FILE *image, char* outfile)
{
  // Remember current position
  long curJpeg = ftell(image);
  long jpegLength;

  // Search for next jpeg segment
  int isAtEnd = seek_next_jpeg(image);
  jpegLength = ftell(image) - curJpeg;

  if (isAtEnd == -1 && jpegLength == 0)
    {
      return -1;
    }

  // Return to the beginning of current jpeg segement
  fseek(image, curJpeg, SEEK_SET);

  // Initialize jpeg byte array
  BYTE jpeg[jpegLength];

  // Read the jpeg into the byte array
  fread(jpeg, sizeof(BYTE), jpegLength, image);

  // Save the jpeg into the file
  FILE *fileptr = fopen(outfile, "w");
  if (fileptr == NULL)
    {
      fprintf(stderr, "Could not create %s.\n", outfile);
      return 1;
    }

  // Write into the file
  fwrite(jpeg, sizeof(BYTE), jpegLength, fileptr);
  fclose(fileptr);

  return 0;
}
