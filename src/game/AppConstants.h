#ifndef __APPCONSTANTS_H__
#define __APPCONSTANTS_H__

/*
=====================
AppConstants

Constants of the App module that live in memory instead of being folded into
the code: the SWIG constant table (AppWrap) reads them at start-up. The values
are those the table installs; the owners are open.
=====================
*/

// The ID of no profile (0).
extern const int	PFID_INVALID;

// The conditions' torpedoes-ready event type (0x803B) and its event handler
// type (0x803C).
extern const int	CT_OPT_TORPS_READY;
extern const int	CT_OPT_TR_EVENT_HANDLER;

// The rule number of no rule (-1).
extern const int	RULENUM_INVALID;

#endif /* !__APPCONSTANTS_H__ */
