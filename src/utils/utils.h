/*

    This file is the main include file used to include all of the functions that may be used in any projects

*/


/** -------------------------------------------------------------------------
 *                Including user inputs' functions
 * -------------------------------------------------------------------------
 */
#ifdef Inputs
#include "inputs/user_inputs.h"
#endif

/** -------------------------------------------------------------------------
 *                Including files' functions
 * -------------------------------------------------------------------------
 */
#ifdef File
#include "actions/file_acts.h"
#endif

/** -------------------------------------------------------------------------
 *                Including integers' functions
 * -------------------------------------------------------------------------
 */
#ifdef Integer
#include "actions/integer_acts.h"
#endif

/** -------------------------------------------------------------------------
 *                Including types for float
 * -------------------------------------------------------------------------
 */
#ifdef FloatType
#include "types/generic_type.h"
#endif

/** -------------------------------------------------------------------------
 *                Including pointers' functions
 * -------------------------------------------------------------------------
 */
#ifdef Pointer
#include "actions/pointer_acts.h"
#endif

/** -------------------------------------------------------------------------
 *                Including strings' functions (char*)
 * -------------------------------------------------------------------------
 */
#ifdef String
#include "actions/string_acts.h"
#endif


/** -------------------------------------------------------------------------
 *                Including logging files' functions
 * -------------------------------------------------------------------------
 */
#ifdef LogFile
#include "logdev/log.h"
#endif

/** -------------------------------------------------------------------------
 *                Including config files' functions
 * -------------------------------------------------------------------------
 */
#ifdef ConfigurationFile
#include "configurations/config.h"
#endif
