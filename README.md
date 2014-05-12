LCD_Library

    To configure your active display change the settings in LCD_Config.h the current supported lcds are ST7735, ST7735R and the SSD1289 (parallel).

    Please read the readme if you are interested in using my library.
    This Library currently works with the SSD1289, ST7735 and ST7735R -- Will be adding more libraries eventually

    color: - Before you use any of the functions from this library use the corresponding library. colors may be put in 24bit however only 18-16bits worth of that color counts. 0xRRGGBB color.

      x,y,width and height:
      -Send width and height (make sure initial xCorridnate+width or yCorinate+height
      are within the valid LCD Resolutions)

    rotation: for all rotations bottom left side of the screen is always x=0,y=0 & upper right side of the screen is dependant on the current roation.

    0,3 = landscape rotations 
    1,2 = potrait rotation    

    dont worry about sending a box that is 4000pixles wide, my code rejects anything above the current resolution. 
    without that the display would act up and display garbage

      LCD_SendPixel():
          -this will draw a 1x1 pixel anyplace that is valid.

      LCD_Fill_Rectangle();  // fast library
          -draws a rectangle anywhere on the screen within the screens valid resolution. 

      LCD_Draw_Line(); 
          -Draws a line from any initial point (x0,y0) - > (x1, y1)

      LCD_Write_PGM();
          -Takes in a string from the program memory (long strings can take up valuable space on the ram)
          prints out on a string(no limit to the screen wont write chars beyond a valid resolution)

      LCD_Write_Line(); 
          -Same as the PGM, only difference is you can place a string directly without using PGM

      LCD_FillScreen(); 
          -Self explanatory, fills screen with desired color, again 0xRRGGBB color 

      LCD_Write_Char() 
          - I do not recommend using this function, you will need to set the variables for the structures. 

      LCD_Write_Style_Line();
          - Same as the write line except you will be using arial font. Do not use ~ or / keys. they will not print properly
          - Will make updates to allow other fonts

