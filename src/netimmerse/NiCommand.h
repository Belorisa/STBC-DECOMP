/*
===============================================================================

	NiCommand.h

	The application framework's command-line parser: it splits the command
	line into arguments and answers queries for "-option" arguments, marking
	each argument it has matched. Range limits set before a numeric query
	bound its value.

	Evidence: NiApplication::SetCommandLine (0x7B74D0) allocates it (0x28
	bytes) and calls the constructor (0x7B8E00) with the command line; the
	application's destructor calls the destructor (0x7B8F70). The option
	queries walk the arguments after the program name, skip the ones
	already matched, and store "option not found" (0x976434) as the error
	on a miss; "option requires an argument", "argument out of range" and
	"filename not found" follow it in the same order. The class is the
	Command class of Dave Eberly's Magic Software library, which NetImmerse
	ships as NiCommand: its layout, its range setters (Min, Max, Inf, Sup)
	and its queries match that lineage, and the names follow it.

===============================================================================
*/

#ifndef __NICOMMAND_H__
#define __NICOMMAND_H__

class NiCommand {
public:
							NiCommand( int quantity, char **arguments );
							NiCommand( const char *commandLine );
							~NiCommand();

	// the index of the first argument no query matched; 0 when there is none
	int						ExcessArguments();

	// range limits for the next numeric query: Min and Max are inclusive,
	// Inf and Sup exclusive
	NiCommand &				Min( double value );
	NiCommand &				Max( double value );
	NiCommand &				Inf( double value );
	NiCommand &				Sup( double value );

	// the index of the unmatched argument "-option", which is then marked
	// matched; 0 when there is none
	int						Boolean( const char *option );
	int						Boolean( const char *option, bool &value );
	// the value after "-option"; 0 when it is missing or out of range
	int						Integer( const char *option, int &value );
	int						Float( const char *option, float &value );
	int						Double( const char *option, double &value );
	int						String( const char *option, char *value );
	// the first unmatched argument that is not an option
	int						Filename( char *name );

protected:
	void					Initialize();

	int						quantity;		// 0x00 arguments, the program name included
	char **					arguments;		// 0x04
	char *					commandLine;	// 0x08 the copy the arguments point into
	bool *					used;			// 0x0C one flag per argument
	double					smallLimit;		// 0x10 the Min or Inf limit
	double					largeLimit;		// 0x18 the Max or Sup limit
	bool					minSet;			// 0x20
	bool					maxSet;			// 0x21
	bool					infSet;			// 0x22
	bool					supSet;			// 0x23
	const char *			lastError;		// 0x24 the last query's error message

	static char				optionNotFound[];
	static char				argumentRequired[];
	static char				argumentOutOfRange[];
	static char				filenameNotFound[];
};

#endif /* !__NICOMMAND_H__ */
