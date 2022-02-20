#include <math.h>
#include <string.h>
#include <limits.h>

const char* symbols[] = { "A","Alpha","B","C","Chi","D","Delta","E","ElementOf","Empty","Epsilon","Equals","Eta", \
"F","G","Gamma","H","I","Intersection","J","K","L","Lambda","Mu","Notin","Omega","Phi","Pi","Psi","Rho","Sigma", \
"Subset","Superset","Tau","Theta","Union","Xi","Zeta" };

// Convert CNN Output encoding to SAT Translator Encoding.
const int CNN_encode_map[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 32 , 31 , 9 , 36 , 10 , 11 , 12 , 13 , 14 , 15 , 38 , \
16 , 17 , 18 , 19 , 20 , 33 , 21 , 22 , 23 , 24 , 25 , 26 , 34 , 35 , 27 , 28 , 37 , 29 , 30 };

// OUTPUT - Sorted Set Theoretical Symbols/Statements Separated by Zeros -> SAT Translator
int cnn_dsp_string[256];
int cnn_dsp_index = 0;

// Debug Variables
int input1[100];
int input2[100];
int input3[200];

int cnn_output[19] = { 1, 7, 16, 8, 35, 14, 15, 8, 14, 5, 8, 30, 5, 31, 0, 2, 9, 18, 11 };
int osl[19] = { 119, 123, 127, 87, 117, 135, 111, 71, 151, 119, 71, 123, 121, 103, 143, 139, 163, 101, 65 };
int osp[38] = { 153, 203, 147, 688, 160, 448, 159, 318, 174, 574, 351, 457, 344, 217, 354, 322, 535, 216, 541, 465, 539, 328, 705, 216, 719, 470, 719, 350, 886, 219, 885, 457, 897, 710, 879, 336, 888, 587 };

// Sort the symbols in the image to reconstruct set theoretical statements.
void symbol_sort(int CNN_output[100], int output_symbol_length[100], int output_symbol_pos[200]) {

	// CNN Output String Processed Boolean Array - 0 if Not Processed/Sorted & 1 if Processed/Sorted/Output to SAT Translator
	int CNN_output_proc[100];
	// Initialize.
	memset(CNN_output_proc, 0, 100 * sizeof(int));
	int prev_row = -1;	// Save previous row for comparison.
	int prev_size = -1;

	// Sorting Algorithm
	while (true) {

		// Search for "approximate" top-left symbol. Break if no symbols exist.
		int p = 0;
		int initial_index = 0;	// Index of Top-Left Symbol in CNN Output Array
		int min_row = INT_MAX;	// Row of Top-Left Symbol
		int min_col = INT_MAX;	// Column of Top-Left Symbol
		int min_char_size = 0;	// Size/Length of Top-Left Symbol

		// Minimal Symbol Search Algorithm
		while (p < 200) {

			// Analyze symbol.
			if (CNN_output_proc[p / 2]); // Symbol processed. Continue search.
			else if (CNN_output[p / 2] == -1 || output_symbol_pos[p] == -1 || output_symbol_pos[p + 1] == -1) {
				// End of Symbol Array. Terminate minimal (top-left) symbol search.
				break;
			}
			// Search for Symbol via Row Priority
			else if (min_row - output_symbol_pos[p] > (min_char_size + prev_size) / 2) {
				initial_index = p / 2;
				min_row = output_symbol_pos[p];
				min_col = output_symbol_pos[p + 1];
				min_char_size = output_symbol_length[initial_index];
			}
			// Search for Symbol via Column Priority and Row Proximity
			else if (abs(output_symbol_pos[p] - min_row) < (min_char_size + prev_size) / 2 && output_symbol_pos[p + 1] < min_col) {
				initial_index = p / 2;
				min_row = output_symbol_pos[p];
				min_col = output_symbol_pos[p + 1];
				min_char_size = output_symbol_length[initial_index];
			}

			// Increment to next symbol (position).
			p = p + 2;
		}

		// Initialize previous symbol characteristics variables.
		if (prev_row < 0 || prev_size < 0) {
			prev_row = min_row;
			prev_size = min_char_size;
		}

		// Analyze sorted search.
		if (min_row == INT_MAX && min_col == INT_MAX && min_char_size == 0) {
			// Empty symbol array/no symbol detected. Terminate Sorting Algorithm.
			break;
		}
		// Minimal (initial) symbol detected!
		else {
			// Compare current symbol row position with previous symbol row position. If newline, inject a zero.
			if (abs(min_row - prev_row) > (min_char_size + prev_size) / 2) {
				// Push 0 to Sorted Set Theoretical Statement String, denoting end of statement.
				cnn_dsp_string[cnn_dsp_index] = 0;
				cnn_dsp_index = cnn_dsp_index + 1;
			}
			// Push Symbol to Sorted Set Theoretical Statement String.
			cnn_dsp_string[cnn_dsp_index] = CNN_encode_map[CNN_output[initial_index]];
			cnn_dsp_index = cnn_dsp_index + 1;
			// Mark Symbol in CNN_output string as sorted/processed.
			CNN_output_proc[initial_index] = 1;
			// Update previous symbol characteristics.
			prev_row = min_row;
			prev_size = min_char_size;
		}

	}

}


int main() {

	// Initialize.
	memset(cnn_dsp_string, 0, 256 * sizeof(int));

	// Debug.
	memset(input1, -1, 100 * sizeof(int));
	memset(input2, -1, 100 * sizeof(int));
	memset(input3, -1, 200 * sizeof(int));

	for (int k = 0; k < 19; k++) {
		input1[k] = cnn_output[k];
		input2[k] = osl[k];
	}

	for (int k = 0; k < 38; k++) {
		input3[k] = osp[k];
	}

	/* // Symbols
	input1[0] = 1; // Alpha
	input1[1] = 8;	// In
	input1[2] = 7;	// E
	input1[3] = 0; // A
	input1[4] = 18; // Intersection
	input1[5] = 17; // I
	// Size
	input2[0] = 50;
	input2[1] = 35;
	input2[2] = 70;
	input2[3] = 60;
	input2[4] = 70;
	input2[5] = 70;
	// Position
	input3[0] = 400; // Line 2
	input3[1] = 20;
	input3[2] = 480;
	input3[3] = 200;
	input3[4] = 450;
	input3[5] = 600;
	input3[6] = 200; // Line 1
	input3[7] = 150;
	input3[8] = 250;
	input3[9] = 800;
	input3[10] = 185;
	input3[11] = 1000; */
	// Execute.
	symbol_sort(input1, input2, input3);

	return 0;
}