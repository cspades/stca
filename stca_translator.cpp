#include <string.h>

/* STCA Satisfiability String */
int stca_SAT[100000];
int stca_SAT_index = 0;

/* STCA Satisfiability String Mapping */
//  42 -> Element Gamma is in Set C
// -42 -> Element Gamma is NOT in Set C
int stca_SAT_map[12][18] = { 
{  1,      2,     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,    18 },
{ 19,     20,    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,    36 },
{ 37,     38,    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,    54 },
{ 55,     56,    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,    72 },
{ 73,     74,    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,    88,    89,    90 },
{ 91,     92,    93,    94,    95,    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,   107,   108 },
{ 109,   110,   111,   112,   113,   114,   115,   116,   117,   118,   119,   120,   121,   122,   123,   124,   125,   126 },
{ 127,   128,   129,   130,   131,   132,   133,   134,   135,   136,   137,   138,   139,   140,   141,   142,   143,   144 },
{ 145,   146,   147,   148,   149,   150,   151,   152,   153,   154,   155,   156,   157,   158,   159,   160,   161,   162 },
{ 163,   164,   165,   166,   167,   168,   169,   170,   171,   172,   173,   174,   175,   176,   177,   178,   179,   180 },
{ 181,   182,   183,   184,   185,   186,   187,   188,   189,   190,   191,   192,   193,   194,   195,   196,   197,   198 },
{ 199,   200,   201,   202,   203,   204,   205,   206,   207,   208,   209,   210,   211,   212,   213,   214,   215,   216 } };

/* Coordinate Translation of Elements and Sets for STCA Satisfiability Map */
int stca_2D_map[30] = { 0,0,1,2,1,3,2,4,3,4,5,6,5,7,8,9,10,11,6,7,8,9,10,11,12,13,14,15,16,17 };

int input_string[256]; // Debugging.


/* Element Inclusion/Exclusion Function */
// Inverse = 1 -> Inclusion & Inverse = -1 -> Exclusion
void element_incl(int element, int set, int inverse) {
	stca_SAT[stca_SAT_index] = inverse * stca_SAT_map[stca_2D_map[set]][stca_2D_map[element]];
	stca_SAT_index = stca_SAT_index + 1;
}

// Translates Set Theoretical Statements (Separated by 0) to SAT Encoding
void stca_translate(int cnn_dsp_string[256]) {

	// Clear Satisfiability String.
	memset(stca_SAT, 0, 100000 * sizeof(int));

	/* Translation Variables */
	int object = 0;	// Element or Set.
	int relation = 0;
	int operation = 0;
	int inverse = 0;

	// Translate the CNN/DSP output string to SAT.
	for (int i = 0; i < 256; i++) {
		// Extract Set Theoretical Statement.
		if (object == 0) {	// Set initial element/set.
			object = cnn_dsp_string[i];
		}
		else if (relation == 0) {	// Set initial relation - inclusion, exclusion, containment, or equality.
			relation = cnn_dsp_string[i];
		}
		else {	// Apply context from element/operation to translate Set Theory Statement to Satisfiability.

			while (cnn_dsp_string[i] != 0) {

				if (relation < 34) {	// Element Operation.

					// Set the inclusion/exclusion flag.
					if (relation == 32)
						inverse = 1;
					else if (relation == 33)
						inverse = -1;

					// Push initial set theory fragment to STCA SAT string.
					stca_SAT[stca_SAT_index] = inverse * stca_SAT_map[stca_2D_map[cnn_dsp_string[i] - 1]][stca_2D_map[object - 1]];
					stca_SAT_index = stca_SAT_index + 1;
					i = i + 1;

					// Search for multiple set targets.
					while (cnn_dsp_string[i] != 0) {

						// Set set operation - union or intersection.
						if (operation == 0) {
							operation = cnn_dsp_string[i];
							i = i + 1;
						}
						else if (operation == 37) {	// Set Union.
							// Push set theory fragment to STCA SAT string.
							if (inverse == -1) {
								// DeMorgan
								stca_SAT[stca_SAT_index] = 0;
								stca_SAT_index = stca_SAT_index + 1;
							}
							stca_SAT[stca_SAT_index] = inverse * stca_SAT_map[stca_2D_map[cnn_dsp_string[i] - 1]][stca_2D_map[object - 1]];
							stca_SAT_index = stca_SAT_index + 1;
							i = i + 1;
							// Check if next symbol exists.
							if (cnn_dsp_string[i] != 0)
								i = i + 1;
						}
						else if (operation == 38) {	// Set Intersection.
							// Push set theory fragment to STCA SAT string.
							if (inverse == 1) {
								// DeMorgan
								stca_SAT[stca_SAT_index] = 0;
								stca_SAT_index = stca_SAT_index + 1;
							}
							stca_SAT[stca_SAT_index] = inverse * stca_SAT_map[stca_2D_map[cnn_dsp_string[i] - 1]][stca_2D_map[object - 1]];
							stca_SAT_index = stca_SAT_index + 1;
							i = i + 1;
							// Check if next symbol exists.
							if (cnn_dsp_string[i] != 0)
								i = i + 1;
						}
					}
				}
				else {	// Set Operation.

				}

				// Increment (to next set theory statement).
				stca_SAT[stca_SAT_index] = 0;
				stca_SAT_index = stca_SAT_index + 1;
			}

			// Reset element and operation.
			object = 0;
			relation = 0;
			operation = 0;
			inverse = 0;
		}
	}

}

// Debugging.
/* 
void main() {

	for (int i = 0; i < 256; i++) {
		input_string[i] = 0;
	}
	input_string[0] = 7;
	input_string[1] = 33;
	input_string[2] = 6;
	input_string[3] = 38;
	input_string[4] = 15;
	input_string[5] = 0;
	input_string[6] = 20;
	input_string[7] = 32;
	input_string[8] = 16;
	input_string[9] = 37;
	input_string[10] = 14;
	input_string[11] = 37;
	input_string[12] = 3;

	stca_translate(input_string);
 
} */