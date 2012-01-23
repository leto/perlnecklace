/* Length of string that is logged to syslog */
#define LENGTH          75 

/* Enable/Disable Modules */
#define ALLOW_MODULES   1       

/* Enable/Disable Syslog */
#define SYSLOG          1

/* Syslog facility name */
#define FACILITY        LOG_LOCAL5

/* Enable Disable chroot */
#define CHROOT		1	

/* Directory to chroot to. All paths starting with / will be mapped to
   this directory. To disable, set to "/" ( with quotes ). */
#define CHROOT_DIR      "/"

/* Resource Limit Maximums */
#define SET_LIMITS	1
#define CORE_SIZE       0
#define OPEN_FILES      8       /* Needs at least 4 to run */   
#define PROCESSES       5
#define FILE_SIZE       20*1024*1024 /* 20MB */
#define CPU_TIME        600          /* seconds */

#ifndef PATH_MAX
	#define PATH_MAX	255	/* Max length of pathname */
#endif

#define FIX_PWD		1	/* Enable/Disable fixing the $PWD */

