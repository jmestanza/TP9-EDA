#ifndef USERDATA_H
#define USERDATA_H

//This structure is used in many .c for this program

	typedef struct userDataType{	//User Data storage for the program. Line of communication for Main and Callback via parser.
		unsigned int mode;
		struct userDataType *Flags; //used in callback to prevent recalls of the same argument and used to verify the user has set a value instead of the defaults(which allows for defaults to be set).
		unsigned int width;
		unsigned int heigh;
		unsigned int cRobots;
	}userData;

#endif