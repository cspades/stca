#include <string.h>
#include <stdio.h>
#include <math.h>

#include "input_image_test.h"

#define IMAGE_SIZE 7990272
#define IMAGE_LENGTH 2448
#define IMAGE_WIDTH 3264
#define PIXEL_THRSHD 128
#define BOX_THRSHD 100

int input_image_thrshd[IMAGE_SIZE];

int pixel_r;			// Search/Center Pixel Position.
int pixel_c;
int box_rad;			// Radius of Extension Box
int pixel_count;		// Pixel Count in Extension Box
int pixel_r_center;		// Pixel Location Averaging
int pixel_c_center;
int extend;				// Extension Check Variable

int output_symbol_array[IMAGE_SIZE];	// Output Array of Symbol Images/Pixels Split by -1
int symbol_array_index;
int output_symbol_pos[200];				// Output Symbol Positions (Row,Col) in Order Corresponding to Output Symbol Array
int symbol_pos_index;

void image_segmentation() {

	// Initialize.
	memset(output_symbol_array, -1, sizeof(int) * IMAGE_SIZE);
	memset(output_symbol_pos, -1, sizeof(int) * 200);
	symbol_array_index = 0;
	symbol_pos_index = 0;

	// Filter the image to pixels.
	for (int i = 0; i < IMAGE_SIZE; i++) {
		// Pixel Saturation Threshold
		if (input_image[i] > PIXEL_THRSHD)
			input_image_thrshd[i] = 0;
		else
			input_image_thrshd[i] = 1;
	}

	// Search for symbols in the image.
	for (int i = 0; i < IMAGE_SIZE; i++) {
		// Detect pixels.
		if (input_image_thrshd[i]) {

			// Capture the position and boundaries of the detected symbol/pixel.
			pixel_r = floor(i / IMAGE_WIDTH);
			pixel_c = i % IMAGE_WIDTH;
			box_rad = 0;	// Inital Symbol Image Box Radii
			extend = 1;	// Attempt to extend the box.

						// Search for additional pixels via box extension.
			while (extend) {

				// Reset pixel boundary extension Boolean.
				extend = 0;
				// Extend pixel box (bounded by twice the extended radius to account for symbol box centering).
				if (pixel_r - 2 * (box_rad + 1) > -1 && pixel_c - 2 * (box_rad + 1) > -1 && \
					pixel_r + 2 * (box_rad + 1) < IMAGE_LENGTH && pixel_c + 2 * (box_rad + 1) < IMAGE_WIDTH)
					// Extend box radius.
					box_rad = box_rad + 1;
				else
					// Cannot extend box.
					break;

				// Analyze the pixel boundary and check for pixels via continuity of symbol image.

				for (int j = pixel_r - box_rad; j <= pixel_r + box_rad; j++)	// Left
					if (input_image_thrshd[j*IMAGE_WIDTH + pixel_c - box_rad])
						extend = 1;
				for (int j = pixel_r - box_rad; j <= pixel_r + box_rad; j++)	// Right
					if (input_image_thrshd[j*IMAGE_WIDTH + pixel_c + box_rad])
						extend = 1;
				for (int j = pixel_c - box_rad; j <= pixel_c + box_rad; j++)	// Above
					if (input_image_thrshd[(pixel_r - box_rad)*IMAGE_WIDTH + j])
						extend = 1;
				for (int j = pixel_c - box_rad; j <= pixel_c + box_rad; j++)	// Below
					if (input_image_thrshd[(pixel_r + box_rad)*IMAGE_WIDTH + j])
						extend = 1;

			}

			// Compute center of pixel cluster via average.
			pixel_count = 0;
			pixel_r_center = 0;
			pixel_c_center = 0;
			for (int y = pixel_r - box_rad; y <= pixel_r + box_rad; y++) {
				for (int x = pixel_c - box_rad; x <= pixel_c + box_rad; x++) {
					if (input_image_thrshd[y*IMAGE_WIDTH + x]) {
						pixel_count = pixel_count + 1;
						pixel_r_center = pixel_r_center + y;
						pixel_c_center = pixel_c_center + x;
					}
				}
			}
			pixel_r_center = round(pixel_r_center / pixel_count);
			pixel_c_center = round(pixel_c_center / pixel_count);

			// Set Symbol Image and Center Position to Output Arrays
			for (int y = pixel_r_center - box_rad; y <= pixel_r_center + box_rad; y++) {
				for (int x = pixel_c_center - box_rad; x <= pixel_c_center + box_rad; x++) {
					if (box_rad > BOX_THRSHD) {	// Annihilate symbol noise.
						// Send symbol image to output array.
						output_symbol_array[symbol_array_index] = input_image_thrshd[y*IMAGE_WIDTH + x];
						symbol_array_index = symbol_array_index + 1;
					}
					// Delete symbol pixel from image.
					input_image_thrshd[y*IMAGE_WIDTH + x] = 0;
				}
			}
			symbol_array_index = symbol_array_index + 1; // Split/distinguish symbol images with -1.
			// Send symbol position to output array.
			if (box_rad > BOX_THRSHD) { // Annihilate symbol noise.
				output_symbol_pos[symbol_pos_index] = pixel_r_center;
				output_symbol_pos[symbol_pos_index + 1] = pixel_c_center;
				symbol_pos_index = symbol_pos_index + 2;
			}
		}
	}

	// Debug.
	/* int debug_array[13];
	int debug_count = 0;
	for (int debug = 0; debug < IMAGE_SIZE - 1; debug++) {
	if (output_symbol_array[debug] > -1 && output_symbol_array[debug + 1] == -1) {
	debug_array[debug_count] = debug;
	debug_count = debug_count + 1;
	}
	} */

}