/*
 * Copyright © 2009 Tyler Hayes
 * ALL RIGHTS RESERVED
 * [This program is licensed under the GPL version 3 or later.]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 *
 * Thanks to:
 * - Bart Massey of Portland State University (http://web.cecs.pdx.edu/~bart/)
 *   for suggesting LADSPA plugins as a project.
 */


//----------------
//-- INCLUSIONS --
//----------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "ladspa.h"
#include "xorgens.h"


//-----------------------
//-- DEFINED CONSTANTS --
//-----------------------
/*
 * These are the port numbers for the plugin
 */
// left channel input
#define SCRAMBLER_INPUT_LEFT 0
// right channel output
#define SCRAMBLER_INPUT_RIGHT 1
// left channel input
#define SCRAMBLER_OUTPUT_LEFT 2
// right channel output
#define SCRAMBLER_OUTPUT_RIGHT 3

/*
 * Other constants
 */
// the plugin's unique ID given by Richard Furse (ladspa@muse.demon.co.uk)
#define UNIQUE_ID 4304
// number of ports involved
#define PORT_COUNT 4


//-------------------------
//-- FUNCTION PROTOTYPES --
//-------------------------

// gets a random unsigned long integer
unsigned long GetRandomNaturalNumber(unsigned long upper_bound);


//--------------------------------
//-- STRUCT FOR PORT CONNECTION --
//--------------------------------

typedef struct {
	// data locations for the input & output audio ports
	LADSPA_Data * Input_Left;
	LADSPA_Data * Input_Right;
	LADSPA_Data * Output_Left;
	LADSPA_Data * Output_Right;
} Scrambler;


//---------------
//-- FUNCTIONS --
//---------------

/*
 * Creates a plugin instance by allocating space for a plugin handle.
 * This function returns a LADSPA_Handle (which is a void * -- a pointer to
 * anything).
 */
LADSPA_Handle instantiate_Scrambler(const LADSPA_Descriptor * Descriptor,
											  unsigned long sample_rate)
{
	Scrambler * scrambler;
	
	// allocate space for a Scrambler struct instance
	scrambler = (Scrambler *) malloc(sizeof(Scrambler));
	
	// send the LADSPA_Handle to the host.  If malloc failed, NULL is returned.
	return scrambler;
}

//-----------------------------------------------------------------------------

/*
 * Make a connection between a specified port and it's corresponding data location.
 * For example, the output port should be "connected" to the place in memory where
 * that sound data to be played is located.
 */
void connect_port_to_Scrambler(LADSPA_Handle instance, unsigned long Port, LADSPA_Data * data_location)
{
	Scrambler * scrambler;
	
	// cast the (void *) instance to (Scrambler *) and set it to local pointer
	scrambler = (Scrambler *) instance;
	
	// direct the appropriate data pointer to the appropriate data location
	switch (Port)
	{
		case SCRAMBLER_INPUT_LEFT :
			scrambler->Input_Left = data_location;
			break;
		case SCRAMBLER_INPUT_RIGHT :
			scrambler->Input_Right = data_location;
			break;
		case SCRAMBLER_OUTPUT_LEFT :
			scrambler->Output_Left = data_location;
			break;
		case SCRAMBLER_OUTPUT_RIGHT :
			scrambler->Output_Right = data_location;
			break;	
	}
}

//-----------------------------------------------------------------------------

/*
 * Here is where the rubber hits the road.  The actual sound manipulation
 * is done in run().
 * What is basically does is takes the block of samples and reorders them
 * in random order.
 */
void run_Scrambler(LADSPA_Handle instance, unsigned long total_samples)
{
	Scrambler * scrambler = (Scrambler *) instance;

	/*
	 * NOTE: these special cases should never happen, but you never know--like
	 * if someone is developing a host program and it has some bugs in it, it
	 * might pass some bad data.
	 */
	if (total_samples <= 1)
	{
		printf("\nA sample count of 0 or 1 was sent to plugin.");
		printf("\nPlugin not executed.\n");
		return;
	}
	if (!scrambler)
	{
		printf("\nPlugin received NULL pointer for plugin instance.");
		printf("\nPlugin not executed.\n");
		return;
	}
	
	// buffer pointers
	LADSPA_Data * input;
	LADSPA_Data * output;
	
	// buffer indexes
	unsigned long in_index = 0;
	unsigned long out_index = 0;

	// for the number of samples scrambled so far and remaining
	unsigned long samples_processed = 0;
	unsigned long samples_remaining = 0;
	
	while (samples_processed < total_samples)
	{
		samples_remaining = total_samples - samples_processed;
		
		input = scrambler->Input_Left;
		output = scrambler->Output_Left;
		
		in_index = GetRandomNaturalNumber(samples_remaining);
	
		output[out_index] = input[in_index];
		input[in_index] = input[samples_remaining - 1];
		
		input = scrambler->Input_Right;
		output = scrambler->Output_Right;
		
		output[out_index] = input[in_index];
		input[in_index] = input[samples_remaining - 1];
		
		++samples_processed;
		++out_index;
	}
}

//-----------------------------------------------------------------------------

/*
 * Frees dynamic memory associated with the plugin instance.  The host
 * better send the right pointer in or there's gonna be a leak!
 */
void cleanup_Scrambler(LADSPA_Handle instance)
{
	if (instance)
		free(instance);
}

//-----------------------------------------------------------------------------

/*
 * Global LADSPA_Descriptor variable used in _init(), ladspa_descriptor(),
 * and _fini().
 */
LADSPA_Descriptor * Scrambler_descriptor = NULL;


/*
 * The _init() function is called whenever this plugin is first loaded
 * by the host using it (when the host program is first opened).
 */
void _init()
{
	/*
	 * allocate memory for Scrambler_descriptor (it's just a pointer at this point).
	 * in other words create an actual LADSPA_Descriptor struct instance that
	 * Scrambler_descriptor will point to.
	 */
	Scrambler_descriptor = (LADSPA_Descriptor *) malloc(sizeof(LADSPA_Descriptor));
	
	// make sure malloc worked properly before initializing the struct fields
	if (Scrambler_descriptor)
	{
		// assign the unique ID of the plugin given by Richard Furse
		Scrambler_descriptor->UniqueID = UNIQUE_ID;
		
		/*
		 * assign the label of the plugin.
		 * NOTE: it must not have white spaces as per ladspa.h.
		 * NOTE: in case you were wondering, strdup() from the string library makes a duplicate
		 * string of the argument and returns the duplicate's pointer (a char *).
		 */
		Scrambler_descriptor->Label = strdup("Scrambler");
		
		/*
		 * assign the special property of the plugin, which is any of the three
		 * defined in ladspa.h: LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN,
		 * and LADSPA_PROPERTY_HARD_RT_CAPABLE.  They are just ints (1, 2, and 4,
		 * respectively).  See ladspa.h for what they actually mean.
		 */
		Scrambler_descriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE;
		
		// assign the plugin name
		Scrambler_descriptor->Name = strdup("Scrambler");
		
		// assign the author of the plugin
		Scrambler_descriptor->Maker = strdup("Tyler Hayes (tgh@pdx.edu)");
		
		/*
		 * assign the copyright info of the plugin (NOTE: use "None" for no copyright
		 * as per ladspa.h)
		 */
		Scrambler_descriptor->Copyright = strdup("GPL");
		
		/*
		 * assign the number of ports for the plugin.
		 */
		Scrambler_descriptor->PortCount = PORT_COUNT;
		
		LADSPA_PortDescriptor * temp_descriptor_array;	// used for allocating and initailizing a
																		// LADSPA_PortDescriptor array (which is
																		// an array of ints) since Scrambler_descriptor->
																		// PortDescriptors is a const *.
		
		// allocate space for the temporary array with a length of the number of ports (PortCount)
		temp_descriptor_array = (LADSPA_PortDescriptor *) calloc(PORT_COUNT, sizeof(LADSPA_PortDescriptor));
		
		/*
		 * set the instance LADSPA_PortDescriptor array (PortDescriptors) pointer to
		 * the location temp_descriptor_array is pointing at.
		 */
		Scrambler_descriptor->PortDescriptors = (const LADSPA_PortDescriptor *) temp_descriptor_array;
		
		/*
		 * set the port properties by ORing specific bit masks defined in ladspa.h.
		 *
		 * these first two give the input ports the properties that tell the host that
		 * the ports takes input and are audio ports (not control ports).
		 */
		temp_descriptor_array[SCRAMBLER_INPUT_LEFT] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		temp_descriptor_array[SCRAMBLER_INPUT_RIGHT] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		
		/*
		 * this gives the output ports the properties that tell the host that these ports
		 * are output ports and that they are audio ports (I don't see any situation where
		 * one might be an output port but not an audio port...).
		 */
		temp_descriptor_array[SCRAMBLER_OUTPUT_LEFT] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		temp_descriptor_array[SCRAMBLER_OUTPUT_RIGHT] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		
		/*
		 * set temp_descriptor_array to NULL for housekeeping--we don't need that local
		 * variable anymore.
		 */
		temp_descriptor_array = NULL;
		
		char ** temp_port_names;	// temporary local variable (which is a pointer to an array
											// of arrays of characters) for the names of the ports since
											// Scrambler_descriptor->PortNames is a const char * const *.

		// allocate the space for two port names
		temp_port_names = (char **) calloc(PORT_COUNT, sizeof(char *));
		
		/*
		 * set the instance PortNames array pointer to the location temp_port_names
		 * is pointing at.
		 */
		Scrambler_descriptor->PortNames = (const char **) temp_port_names;
		
		// set the name of the input ports
		temp_port_names[SCRAMBLER_INPUT_LEFT] = strdup("Input Left Channel");
		temp_port_names[SCRAMBLER_INPUT_RIGHT] = strdup("Input Right Channel");
		
		// set the name of the ouput ports
		temp_port_names[SCRAMBLER_OUTPUT_LEFT] = strdup("Output Left Channel");
		temp_port_names[SCRAMBLER_OUTPUT_RIGHT] = strdup("Output Right Channel");
		
		// reset temp variable to NULL for housekeeping
		temp_port_names = NULL;
		
		LADSPA_PortRangeHint * temp_hints;	// temporary local variable (pointer to a
														// PortRangeHint struct) since Scrambler_descriptor->
														// PortRangeHints is a const *.
		
		// allocate space for two port hints (see ladspa.h for info on 'hints')									
		temp_hints = (LADSPA_PortRangeHint *) calloc(PORT_COUNT, sizeof(LADSPA_PortRangeHint));
		
		/*
		 * set the instance PortRangeHints pointer to the location temp_hints
		 * is pointed at.
		 */
		Scrambler_descriptor->PortRangeHints = (const LADSPA_PortRangeHint *) temp_hints;
		
		/*
		 * set the port hint descriptors (which are ints).
		 */
		temp_hints[SCRAMBLER_INPUT_LEFT].HintDescriptor = 0;
		temp_hints[SCRAMBLER_INPUT_RIGHT].HintDescriptor = 0;
		temp_hints[SCRAMBLER_OUTPUT_LEFT].HintDescriptor = 0;
		temp_hints[SCRAMBLER_OUTPUT_RIGHT].HintDescriptor = 0;
		
		// reset temp variable to NULL for housekeeping
		temp_hints = NULL;
		
		// set the instance's function pointers to appropriate functions
		Scrambler_descriptor->instantiate = instantiate_Scrambler;
		Scrambler_descriptor->connect_port = connect_port_to_Scrambler;
		Scrambler_descriptor->activate = NULL;
		Scrambler_descriptor->run = run_Scrambler;
		Scrambler_descriptor->run_adding = NULL;
		Scrambler_descriptor->set_run_adding_gain = NULL;
		Scrambler_descriptor->deactivate = NULL;
		Scrambler_descriptor->cleanup = cleanup_Scrambler;
	}
}

//-----------------------------------------------------------------------------

/*
 * Returns a descriptor of this plugin.
 *
 * NOTE: this function MUST be called 'ladspa_descriptor' or else the plugin
 * will not be recognized.
 */
const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	if (index == 0)
		return Scrambler_descriptor;
	else
		return NULL;
}

//-----------------------------------------------------------------------------

/*
 * This is called automatically when the host quits (when this dynamic library
 * is unloaded).  It frees all dynamically allocated memory associated with
 * the descriptor.
 */
void _fini()
{
	if (Scrambler_descriptor)
	{
		free((char *) Scrambler_descriptor->Label);
		free((char *) Scrambler_descriptor->Name);
		free((char *) Scrambler_descriptor->Maker);
		free((char *) Scrambler_descriptor->Copyright);
		free((LADSPA_PortDescriptor *) Scrambler_descriptor->PortDescriptors);
		
		/*
		 * the for loop here is kind of unnecessary since the number of ports
		 * was hard coded for this plugin as 2, but whatever.
		 */
		int i = 0;
		for(i = 0; i < Scrambler_descriptor->PortCount; ++i)
			free((char *)(Scrambler_descriptor->PortNames[i]));
		
		free((char **) Scrambler_descriptor->PortNames);
		free((LADSPA_PortRangeHint *) Scrambler_descriptor->PortRangeHints);
		
		free(Scrambler_descriptor);
	}
}

//-----------------------------------------------------------------------------

/*
 * This function uses Richard Brent's uniform random number generator (see
 * comments in the function) to get a random unsigned long integer.  It is
 * seeded with the current time's seconds and nanoseconds.
 */
unsigned long GetRandomNaturalNumber(unsigned long upper_bound)
{
	// get the current time to seed the generator
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	/*
	 * This next line uses a uniform random number generator by Richard Brent.
	 * (http://wwwmaths.anu.edu.au/~brent/random.html)
	 * which is licensed under the GNU Public License v2.
	 * See xorgens.c and xorgens.h for the source code.  Many thanks
	 * to Richard Brent.
	 *
	 * NOTE: the tv_sec and tv_usec members of the timeval struct are
	 * long integers that represent the current time in seconds and
	 * nanoseconds, respectively, since Jan. 1, 1970.  They are used
	 * here to seed the generator.  The generator is called with
	 * xor4096i(), which, unlike the C standard generator, is seeded
	 * and returns a number with the same call.
	 */
	unsigned long rand_num;
	// seed the generator and retrieve the random number
	rand_num = xor4096i((unsigned long)(current_time.tv_usec * current_time.tv_sec));
	// force the random number to within the boundaries
	rand_num = rand_num % upper_bound;
	
	return rand_num;
}

// ------------------------------- EOF ----------------------------------------
