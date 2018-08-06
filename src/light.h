#ifndef LIGHT_H_
#define LIGHT_H_

#include "config.h"
#include "helpers.h"

#include <sys/types.h>
#include <dirent.h>
#include <linux/limits.h>

#define LIGHT_YEAR   "2012-2018"
#define LIGHT_AUTHOR "Fredrik Haikarainen"

#define ASSERT_SET(t,v)							\
	if (v) {							\
		fprintf(stderr, t" arguments cannot be used in conjunction.\n"); \
		return FALSE;						\
	}								\
	v = TRUE;

#define ASSERT_OPSET() ASSERT_SET("Operation", opSet)
#define ASSERT_TARGETSET() ASSERT_SET("Target", targetSet)
#define ASSERT_FIELDSET() ASSERT_SET("Field", fieldSet)
#define ASSERT_CTRLSET() ASSERT_SET("Controller", ctrlSet)
#define ASSERT_VALSET() ASSERT_SET("Value", valSet)

typedef enum LIGHT_FIELD {
  LIGHT_BRIGHTNESS = 0,
  LIGHT_MAX_BRIGHTNESS,
  LIGHT_MIN_CAP,
  LIGHT_SAVERESTORE
} LIGHT_FIELD;

typedef enum LIGHT_TARGET {
  LIGHT_BACKLIGHT = 0,
  LIGHT_KEYBOARD
} LIGHT_TARGET;

typedef enum LIGHT_CTRL_MODE {
  LIGHT_AUTO = 0,
  LIGHT_SPECIFY
} LIGHT_CTRL_MODE;

typedef enum LIGHT_OP_MODE {
  LIGHT_GET = 0,
  LIGHT_SET,
  LIGHT_ADD,
  LIGHT_SUB,
  LIGHT_PRINT_HELP,   /* Prints help and exits  */
  LIGHT_PRINT_VERSION, /* Prints version info and exits */
  LIGHT_LIST_CTRL,
  LIGHT_RESTORE,
  LIGHT_SAVE

} LIGHT_OP_MODE;

typedef enum LIGHT_VAL_MODE {
  LIGHT_RAW = 0,
  LIGHT_PERCENT
} LIGHT_VAL_MODE;

typedef struct light_runtimeArguments_s {
  /* Which controller to use */
  LIGHT_CTRL_MODE controllerMode;
  char            specifiedController[NAME_MAX + 1];

  /* What to do with the controller */
  LIGHT_OP_MODE   operationMode;
  LIGHT_VAL_MODE  valueMode;
  unsigned long   specifiedValueRaw; /* The specified value in raw mode */
  double          specifiedValuePercent; /* The specified value in percent */

  LIGHT_TARGET   target;
  LIGHT_FIELD    field;

  /* Cache data */
  LIGHT_BOOL      hasCachedMaxBrightness;
  unsigned long   cachedMaxBrightness;

} light_runtimeArguments, *light_runtimeArguments_p;

/* -- Global variable holding the settings for the current run -- */
light_runtimeArguments light_Configuration;

/* Sets default values for the configuration */
void light_defaultConfig();


/* Parses the program arguments and sets the configuration accordingly (unsanitized) */
LIGHT_BOOL light_parseArguments(int argc, char** argv);

/* Prints a header if verbosity level > 0 */
void light_printVersion(void);

/* Prints help regardless of verbosity level */
void light_printHelp(void);

/* -- SECTION: Main code -- */

/* Initializes the application */
LIGHT_BOOL light_initialize(int argc, char** argv);

/* Does the work */
LIGHT_BOOL light_execute(void);

/* Frees up resources */
void light_free();

/* SECTION: Controller functionality */

/* WARNING: `buffer` HAS to be freed by the user if not null once returned!
 * Size is always NAME_MAX + 1 */
LIGHT_BOOL light_genPath(char const *controller, LIGHT_TARGET target, LIGHT_FIELD type, char **buffer);

LIGHT_BOOL light_validControllerName(char const *controller);

LIGHT_BOOL light_getBrightness(char const *controller, unsigned long *v);

LIGHT_BOOL light_getMaxBrightness(char const *controller, unsigned long *v);

LIGHT_BOOL light_setBrightness(char const *controller, unsigned long v);

LIGHT_BOOL light_controllerAccessible(char const *controller);

/* WARNING: `controller` HAS to be at most NAME_MAX, otherwise fails */
LIGHT_BOOL light_getBestController(char *controller);

LIGHT_BOOL light_getMinCap(char const *controller, LIGHT_BOOL *hasMinCap, unsigned long *minCap);

LIGHT_BOOL light_setMinCap(char const *controller, unsigned long v);

LIGHT_BOOL light_listControllers();

LIGHT_BOOL light_saveBrightness(char const *controller, unsigned long v);

LIGHT_BOOL light_restoreBrightness(char const *controller);

#endif /* LIGHT_H_ */