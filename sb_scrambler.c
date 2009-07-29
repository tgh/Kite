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
#include "ladspa.h"

//-----------------------
//-- DEFINED CONSTANTS --
//-----------------------
/*
 * These are the port numbers for the plugin
 */
#define XXX_INPUT 0
#define XXX_OUTPUT 1

/*
 * Other constants
 */
// the plugin's unique ID given by Richard Furse (ladspa@muse.demon.co.uk)
#define UNIQUE_ID XXX
// number of ports involved
#define PORT_COUNT XXX

//-------------------------
//-- FUNCTION PROTOTYPES --
//-------------------------

XXX

//--------------------------------
//-- STRUCT FOR PORT CONNECTION --
//--------------------------------

typedef struct {
	// data locations for the input & output audio ports
	LADSPA_Data * Input;
	LADSPA_Data * Output;
} XXX;

//---------------
//-- FUNCTIONS --
//---------------

/*
 * Creates a plugin instance by allocating space for a plugin handle.
 * This function returns a LADSPA_Handle (which is a void * -- a pointer to
 * anything).
 */
LADSPA_Handle instantiate_XXX(const LADSPA_Descriptor * Descriptor,
											  unsigned long sample_rate)
{
	XXX * XXX;
	
	// allocate space for a XXX struct instance
	XXX = (XXX *) malloc(sizeof(XXX));
	
	// send the LADSPA_Handle to the host.  If malloc failed, NULL is returned.
	return XXX;
}

//-----------------------------------------------------------------------------

/*
 * Make a connection between a specified port and it's corresponding data location.
 * For example, the output port should be "connected" to the place in memory where
 * that sound data to be played is located.
 */
void connect_port_to_XXX(LADSPA_Handle instance, unsigned long Port, LADSPA_Data * data_location)
{
	XXX * XXX;
	
	// cast the (void *) instance to (XXX *) and set it to local pointer
	XXX = (XXX *) instance;
	
	// direct the appropriate data pointer to the appropriate data location
	if (Port == XXX_INPUT)
		XXX->Input = data_location;
	else if (Port == XXX_OUTPUT)
		XXX->Output = data_location;
}

//-----------------------------------------------------------------------------

/*
 * Here is where the rubber hits the road.  The actual sound manipulation
 * is done in run().
 * XXX
 */
void run_XXX(LADSPA_Handle instance, unsigned long sample_count)
{
	XXX * XXX = (XXX *) instance;

	/*
	 * NOTE: these special cases should never happen, but you never know--like
	 * if someone is developing a host program and it has some bugs in it, it
	 * might pass some bad data.
	 */
	if (sample_count XXX)
	{
		printf("\nXXX");
		printf("\nPlugin not executed.\n");
		return;
	}
	if (!XXX)
	{
		printf("\nPlugin received NULL pointer for plugin instance.");
		printf("\nPlugin not executed.\n");
		return;
	}
	if (XXX->sample_rate XXX)
	{
		printf("\nXXX");
		printf("\nPlugin not executed.\n");
		return;
	}

	LADSPA_Data * input;			// to point to the input stream
	LADSPA_Data * output;		// to point to the output stream

	// link the local pointers to their appropriate streams passed in through instance
	input = XXX->Input;
	output = XXX->Output;

	XXX
}

//-----------------------------------------------------------------------------

/*
 * Frees dynamic memory associated with the plugin instance.  The host
 * better send the right pointer in or there's gonna be a leak!
 */
void cleanup_XXX(LADSPA_Handle instance)
{
	if (instance)
		free(instance);
}

//-----------------------------------------------------------------------------

/*
 * Global LADSPA_Descriptor variable used in _init(), ladspa_descriptor(),
 * and _fini().
 */
LADSPA_Descriptor * XXX_descriptor = NULL;


/*
 * The _init() function is called whenever this plugin is first loaded
 * by the host using it (when the host program is first opened).
 */
void _init()
{
	/*
	 * allocate memory for XXX_descriptor (it's just a pointer at this point).
	 * in other words create an actual LADSPA_Descriptor struct instance that
	 * XXX_descriptor will point to.
	 */
	XXX_descriptor = (LADSPA_Descriptor *) malloc(sizeof(LADSPA_Descriptor));
	
	// make sure malloc worked properly before initializing the struct fields
	if (XXX_descriptor)
	{
		// assign the unique ID of the plugin given by Richard Furse
		XXX_descriptor->UniqueID = UNIQUE_ID;
		
		/*
		 * assign the label of the plugin.
		 * NOTE: it must not have white spaces as per ladspa.h.
		 * NOTE: in case you were wondering, strdup() from the string library makes a duplicate
		 * string of the argument and returns the duplicate's pointer (a char *).
		 */
		XXX_descriptor->Label = strdup("XXX");
		
		/*
		 * assign the special property of the plugin, which is any of the three
		 * defined in ladspa.h: LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN,
		 * and LADSPA_PROPERTY_HARD_RT_CAPABLE.  They are just ints (1, 2, and 4,
		 * respectively).  See ladspa.h for what they actually mean.
		 */
		XXX_descriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE;
		
		// assign the plugin name
		XXX_descriptor->Name = strdup("XXX");
		
		// assign the author of the plugin
		XXX_descriptor->Maker = strdup("Tyler Hayes");
		
		/*
		 * assign the copyright info of the plugin (NOTE: use "None" for no copyright
		 * as per ladspa.h)
		 */
		XXX_descriptor->Copyright = strdup("GPL");
		
		/*
		 * assign the number of ports for the plugin.
		 */
		XXX_descriptor->PortCount = PORT_COUNT;
		
		LADSPA_PortDescriptor * temp_descriptor_array;	// used for allocating and initailizing a
																		// LADSPA_PortDescriptor array (which is
																		// an array of ints) since XXX_descriptor->
																		// PortDescriptors is a const *.
		
		// allocate space for the temporary array with a length of the number of ports (PortCount)
		temp_descriptor_array = (LADSPA_PortDescriptor *) calloc(PORT_COUNT, sizeof(LADSPA_PortDescriptor));
		
		/*
		* set the instance LADSPA_PortDescriptor array (PortDescriptors) pointer to
		* the location temp_descriptor_array is pointing at.
		*/
		XXX_descriptor->PortDescriptors = (const LADSPA_PortDescriptor *) temp_descriptor_array;
		
		/*
		 * set the port properties by ORing specific bit masks defined in ladspa.h.
		 *
		 * this first one gives the first port the properties that tell the host that
		 * this port takes input and is an audio port (not a control port).
		 */
		temp_descriptor_array[XXX_INPUT] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		
		/*
		 * this gives the second port the properties that tell the host that this port is
		 * an output port and that it is an audio port (I don't see any situation where
		 * one might be an output port, but not an audio port...).
		 */
		temp_descriptor_array[XXX_OUTPUT] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		
		/*
		 * set temp_descriptor_array to NULL for housekeeping--we don't need that local
		 * variable anymore.
		 */
		temp_descriptor_array = NULL;
		
		char ** temp_port_names;	// temporary local variable (which is a pointer to an array
											// of arrays of characters) for the names of the ports since
											// XXX_descriptor->PortNames is a const char * const *.

		// allocate the space for two port names
		temp_port_names = (char **) calloc(PORT_COUNT, sizeof(char *));
		
		/*
		* set the instance PortNames array pointer to the location temp_port_names
		* is pointing at.
		*/
		XXX_descriptor->PortNames = (const char **) temp_port_names;
		
		// set the name of the input port
		temp_port_names[XXX_INPUT] = strdup("Input");
		
		// set the name of the ouput port
		temp_port_names[XXX_OUTPUT] = strdup("Output");
		
		// reset temp variable to NULL for housekeeping
		temp_port_names = NULL;
		
		LADSPA_PortRangeHint * temp_hints;	// temporary local variable (pointer to a
														// PortRangeHint struct) since XXX_descriptor->
														// PortRangeHints is a const *.
		
		// allocate space for two port hints (see ladspa.h for info on 'hints')									
		temp_hints = (LADSPA_PortRangeHint *) calloc(PORT_COUNT, sizeof(LADSPA_PortRangeHint));
		
		/*
		* set the instance PortRangeHints pointer to the location temp_hints
		* is pointed at.
		*/
		XXX_descriptor->PortRangeHints = (const LADSPA_PortRangeHint *) temp_hints;
		
		/*
		 * set the port hint descriptors (which are ints).
		 */
		temp_hints[XXX_INPUT].HintDescriptor = 0;
		temp_hints[XXX_OUTPUT].HintDescriptor = 0;
		
		// reset temp variable to NULL for housekeeping
		temp_hints = NULL;
		
		// set the instance's function pointers to appropriate functions
		XXX_descriptor->instantiate = instantiate_XXX;
		XXX_descriptor->connect_port = connect_port_to_XXX;
		XXX_descriptor->activate = NULL;
		XXX_descriptor->run = run_XXX;
		XXX_descriptor->run_adding = NULL;
		XXX_descriptor->set_run_adding_gain = NULL;
		XXX_descriptor->deactivate = NULL;
		XXX_descriptor->cleanup = cleanup_XXX;
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
		return XXX_descriptor;
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
	if (XXX_descriptor)
	{
		free((char *) XXX_descriptor->Label);
		free((char *) XXX_descriptor->Name);
		free((char *) XXX_descriptor->Maker);
		free((char *) XXX_descriptor->Copyright);
		free((LADSPA_PortDescriptor *) XXX_descriptor->PortDescriptors);
		
		/*
		 * the for loop here is kind of unnecessary since the number of ports
		 * was hard coded for this plugin as 2, but whatever.
		 */
		int i = 0;
		for(i = 0; i < XXX_descriptor->PortCount; ++i)
			free((char *)(XXX_descriptor->PortNames[i]));
		
		free((char **) XXX_descriptor->PortNames);
		free((LADSPA_PortRangeHint *) XXX_descriptor->PortRangeHints);
		
		free(XXX_descriptor);
	}
}

// ------------------------------- EOF ----------------------------------------
