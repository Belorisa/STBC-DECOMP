// NiCommand.cpp -- the command-line parser

#include "NiRTLib.h"
#include "NiCommand.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007b9800 _$E34
// FUNCTION: STBC 0x007b9820 _$E17
// FUNCTION: STBC 0x007b9840 _$E15
// FUNCTION: STBC 0x007b9850 _$E23
// FUNCTION: STBC 0x007b9870 _$E21

// DATA: STBC 0x976434
char NiCommand::optionNotFound[] = "option not found";

// DATA: STBC 0x976448
char NiCommand::argumentRequired[] = "option requires an argument";

// DATA: STBC 0x976464
char NiCommand::argumentOutOfRange[] = "argument out of range";

// DATA: STBC 0x97647C
char NiCommand::filenameNotFound[] = "filename not found";

/*
================
NiCommand::NiCommand

Takes the arguments as main receives them; the parser does not own them.
================
*/
// FUNCTION: STBC 0x7B8DA0 ??0NiCommand@@QAE@HPAPAD@Z
NiCommand::NiCommand( int quantity_, char **arguments_ ) {
	quantity = quantity_;
	arguments = arguments_;
	commandLine = 0;
	Initialize();
}

/*
================
NiCommand::NiCommand

Splits a copy of the command line into arguments at spaces and tabs; a
double quote starts an argument that runs to the next quote. The program
name is not on the command line: argument 0 is the copy itself.

Bug: an empty command line leaves the copy and the match flags unset,
and the destructor frees them.
================
*/
// FUNCTION: STBC 0x7B8E00 ??0NiCommand@@QAE@PBD@Z
NiCommand::NiCommand( const char *commandLine_ ) {
	// the arguments, newest first, before they are counted
	class Argument {
	public:
		char *		item;
		Argument *	next;
	};

	quantity = 0;
	arguments = 0;
	if ( commandLine_ == 0 || strlen( commandLine_ ) == 0 ) {
		return;
	}

	commandLine = new char[strlen( commandLine_ ) + 1];
	strcpy( commandLine, commandLine_ );

	char *token = commandLine;
	Argument *list = 0;
	while ( token && *token ) {
		quantity++;
		Argument *current = new Argument;
		current->next = list;
		list = current;

		if ( *token == '"' ) {
			list->item = ++token;
			while ( *token != 0 && *token != '"' ) {
				token++;
			}
		} else {
			list->item = token;
			while ( *token != 0 && *token != ' ' && *token != '\t' ) {
				token++;
			}
		}
		if ( *token != 0 ) {
			*token++ = 0;
		}
		while ( *token != 0 && ( *token == ' ' || *token == '\t' ) ) {
			token++;
		}
	}

	quantity++;
	arguments = new char *[quantity];
	arguments[0] = commandLine;
	int i = quantity - 1;
	while ( list ) {
		arguments[i--] = list->item;
		Argument *next = list->next;
		delete list;
		list = next;
	}

	Initialize();
}

/*
================
NiCommand::~NiCommand
================
*/
// FUNCTION: STBC 0x7B8F70
NiCommand::~NiCommand() {
	delete [] used;
	if ( commandLine ) {
		delete [] arguments;
		delete [] commandLine;
	}
}

/*
================
NiCommand::Initialize

No argument is matched and no range is set. The last error is left as it
was: unset until a query fails.
================
*/
// FUNCTION: STBC 0x7B8FA0
void NiCommand::Initialize() {
	used = new bool[quantity];
	memset( used, false, quantity * sizeof( bool ) );
	smallLimit = 0.0;
	largeLimit = 0.0;
	minSet = false;
	maxSet = false;
	infSet = false;
	supSet = false;
}

/*
================
NiCommand::ExcessArguments

The queries answer 0 on a null parser.
================
*/
// FUNCTION: STBC 0x7B8FF0
int NiCommand::ExcessArguments() {
	if ( !this ) {
		return 0;
	}
	for ( int i = 1; i < quantity; i++ ) {
		if ( !used[i] ) {
			return i;
		}
	}
	return 0;
}

/*
================
NiCommand::Min
================
*/
// FUNCTION: STBC 0x7B9010
NiCommand &NiCommand::Min( double value ) {
	smallLimit = value;
	minSet = true;
	return *this;
}

/*
================
NiCommand::Max
================
*/
// FUNCTION: STBC 0x7B9030
NiCommand &NiCommand::Max( double value ) {
	largeLimit = value;
	maxSet = true;
	return *this;
}

/*
================
NiCommand::Inf
================
*/
// FUNCTION: STBC 0x7B9050
NiCommand &NiCommand::Inf( double value ) {
	smallLimit = value;
	infSet = true;
	return *this;
}

/*
================
NiCommand::Sup
================
*/
// FUNCTION: STBC 0x7B9070
NiCommand &NiCommand::Sup( double value ) {
	largeLimit = value;
	supSet = true;
	return *this;
}

/*
================
NiCommand::Boolean
================
*/
// FUNCTION: STBC 0x7B9090 ?Boolean@NiCommand@@QAEHPBD@Z
int NiCommand::Boolean( const char *option ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] == '-' && strcmp( option, ++argument ) == 0 ) {
			used[i] = true;
			matchFound = i;
			break;
		}
	}
	if ( matchFound == 0 ) {
		lastError = optionNotFound;
	}
	return matchFound;
}

/*
================
NiCommand::Boolean

As above, and value tells whether the option was given.
================
*/
// FUNCTION: STBC 0x7B9150 ?Boolean@NiCommand@@QAEHPBDAA_N@Z
int NiCommand::Boolean( const char *option, bool &value ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	value = false;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] == '-' && strcmp( option, ++argument ) == 0 ) {
			used[i] = true;
			matchFound = i;
			value = true;
			break;
		}
	}
	if ( matchFound == 0 ) {
		lastError = optionNotFound;
	}
	return matchFound;
}

/*
================
NiCommand::Integer

The integer after "-option", within the range set since the last numeric
query; a numeric query clears the range.
================
*/
// FUNCTION: STBC 0x7B9220
int NiCommand::Integer( const char *option, int &value ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] == '-' && strcmp( option, ++argument ) == 0 ) {
			// the argument must follow, and may be a negative number
			argument = arguments[i + 1];
			if ( used[i + 1] || ( argument[0] == '-' && !isdigit( argument[1] ) ) ) {
				lastError = argumentRequired;
				return 0;
			}
			sscanf( argument, "%d", &value );
			if ( ( minSet && value < smallLimit ) || ( maxSet && value > largeLimit )
				|| ( infSet && value <= smallLimit ) || ( supSet && value >= largeLimit ) ) {
				lastError = argumentOutOfRange;
				return 0;
			}
			used[i] = true;
			used[i + 1] = true;
			matchFound = i;
			break;
		}
	}
	minSet = false;
	maxSet = false;
	infSet = false;
	supSet = false;
	if ( matchFound == 0 ) {
		lastError = optionNotFound;
	}
	return matchFound;
}

/*
================
NiCommand::Float
================
*/
// FUNCTION: STBC 0x7B9390
int NiCommand::Float( const char *option, float &value ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] == '-' && strcmp( option, ++argument ) == 0 ) {
			// the argument must follow, and may be a negative number
			argument = arguments[i + 1];
			if ( used[i + 1] || ( argument[0] == '-' && !isdigit( argument[1] ) ) ) {
				lastError = argumentRequired;
				return 0;
			}
			sscanf( argument, "%f", &value );
			if ( ( minSet && value < smallLimit ) || ( maxSet && value > largeLimit )
				|| ( infSet && value <= smallLimit ) || ( supSet && value >= largeLimit ) ) {
				lastError = argumentOutOfRange;
				return 0;
			}
			used[i] = true;
			used[i + 1] = true;
			matchFound = i;
			break;
		}
	}
	minSet = false;
	maxSet = false;
	infSet = false;
	supSet = false;
	if ( matchFound == 0 ) {
		lastError = optionNotFound;
	}
	return matchFound;
}

/*
================
NiCommand::Double
================
*/
// FUNCTION: STBC 0x7B9500
int NiCommand::Double( const char *option, double &value ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] == '-' && strcmp( option, ++argument ) == 0 ) {
			// the argument must follow, and may be a negative number
			argument = arguments[i + 1];
			if ( used[i + 1] || ( argument[0] == '-' && !isdigit( argument[1] ) ) ) {
				lastError = argumentRequired;
				return 0;
			}
			sscanf( argument, "%lf", &value );
			if ( ( minSet && value < smallLimit ) || ( maxSet && value > largeLimit )
				|| ( infSet && value <= smallLimit ) || ( supSet && value >= largeLimit ) ) {
				lastError = argumentOutOfRange;
				return 0;
			}
			used[i] = true;
			used[i + 1] = true;
			matchFound = i;
			break;
		}
	}
	minSet = false;
	maxSet = false;
	infSet = false;
	supSet = false;
	if ( matchFound == 0 ) {
		lastError = optionNotFound;
	}
	return matchFound;
}

/*
================
NiCommand::String

Copies the argument after "-option" into value, which must be largeLimit
enough.
================
*/
// FUNCTION: STBC 0x7B9670
int NiCommand::String( const char *option, char *value ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] == '-' && strcmp( option, ++argument ) == 0 ) {
			// the argument must follow
			argument = arguments[i + 1];
			if ( used[i + 1] || argument[0] == '-' ) {
				lastError = argumentRequired;
				return 0;
			}
			strcpy( value, argument );
			used[i] = true;
			used[i + 1] = true;
			matchFound = i;
			break;
		}
	}
	if ( matchFound == 0 ) {
		lastError = optionNotFound;
	}
	return matchFound;
}

/*
================
NiCommand::Filename

Copies the first unmatched argument that is not an option into name,
which must be largeLimit enough.
================
*/
// FUNCTION: STBC 0x7B9770
int NiCommand::Filename( char *name ) {
	if ( !this ) {
		return 0;
	}
	int matchFound = 0;
	for ( int i = 1; i < quantity; i++ ) {
		char *argument = arguments[i];
		if ( !used[i] && argument[0] != '-' ) {
			strcpy( name, argument );
			used[i] = true;
			matchFound = i;
			break;
		}
	}
	if ( matchFound == 0 ) {
		lastError = filenameNotFound;
	}
	return matchFound;
}
