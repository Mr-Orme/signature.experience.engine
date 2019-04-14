#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//TODO:: const correctness. Check to see if consts can be put on pointer parameters and class methods.
//TODO:: clear out #includes. Make sure every #include is needed in ever file! Cleared out all includes and have put them
//in definitions
//TODO:: Move initialization to the constructor and stopping/finishing to the destructor.
//TODO:: Move default initialization of member variables to .h file using {} for ALL classes!
//Basic Types


	typedef unsigned int	Uint32;
	typedef float			eFloat;
	typedef Uint32			eInt;


	struct RGBA
	{
		eInt R;
		eInt G;
		eInt B;
		eInt A;
	};

	//Constants
	const eFloat PI = 3.14159f;


#endif