#include <EXTERN.h>
#include <perl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <pwd.h>
#include <grp.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "config.h"

/* All this crap is needed to allow perl modules, copied from perlembed manpage */
static void xs_init _((void));

EXTERN_C void boot_DynaLoader _((CV* cv));
EXTERN_C void xs_init()
        {
               char *file = __FILE__;
               newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
        };

static PerlInterpreter *my_perl;

static void resource_limits (void){
	struct rlimit core,openfiles,procs,filesize,cputime;

	core.rlim_cur = core.rlim_max = CORE_SIZE;
 	openfiles.rlim_cur = openfiles.rlim_max = OPEN_FILES;
	procs.rlim_cur = procs.rlim_max = PROCESSES;
	filesize.rlim_cur = filesize.rlim_max = FILE_SIZE;
	cputime.rlim_cur = cputime.rlim_max = CPU_TIME;
	
	if ( setrlimit(RLIMIT_CORE,&core) ){
		printf("Couldn't set corefile size\n");
		exit(1);
	};	 
	if ( setrlimit(RLIMIT_NOFILE,&openfiles) ){
		printf("Couldn't set openfiles\n");
		exit(1);
	};
	if ( setrlimit(RLIMIT_NPROC,&procs) ){
		printf("Couldn't set procs\n");
		exit(1);
	};
	if ( setrlimit(RLIMIT_FSIZE,&filesize) ){
		printf("Couldn't set filesize\n");
		exit(1);
	};
	if ( setrlimit(RLIMIT_CPU,&cputime) ){
		printf("Couldn't set cputime\n");
		exit(1);
	};

}

int main ( int argc, char **argv, char **env) {

	int argvsize = 0, i = 0;
        char *argvbuf,*pwd;
        char tmp;
        struct passwd *pw;
        struct group  *gw;
	int path_max = PATH_MAX + 2; /* The getcwd docs say to do this. */


	if( FIX_PWD ){
		pwd = malloc(path_max);
		getcwd(pwd,path_max);
 	};
	
	if( CHROOT ){
		if( chroot(CHROOT_DIR) ){
			printf("Couldn't chroot\n");
			exit(1);
		} else {
			if( chdir(CHROOT_DIR) ){
				printf("Couldn't chdir\n");
				exit(1);
			}	
		}
	}
	if( FIX_PWD ){
		chdir(pwd);
	}
	
	if( SET_LIMITS )
		resource_limits();

	/* No more need for high privs, lose them */
	if( setuid(geteuid()) ){
		printf("Couldn't setuid\n");
		exit(1);
	};

        for(i = 0; i < argc ; argvsize += strlen(argv[i++]));
	argvsize += argc;
	
	if (!(argvbuf = malloc(argvsize))){
                printf("Couldn't malloc\n");
                exit(1);
        }

        for ( i = 0, *argvbuf = 0; i < argc;){
                strcat(argvbuf,argv[i]);
                if(++i < argc)
                        strcat(argvbuf, " ");
        };

        tmp = argvbuf[LENGTH];
        argvbuf[LENGTH]= '\0';
        
        my_perl = perl_alloc();
        perl_construct(my_perl);

        if( ALLOW_MODULES ){
                perl_parse(my_perl,xs_init, argc, argv, env);
        } else {
                perl_parse(my_perl,NULL,argc,argv,env);
        }
                
        /* Run the entire program */
        perl_run(my_perl);
                                        
        perl_destruct(my_perl);
        perl_free(my_perl);
        pw = getpwuid(geteuid());
 	gw = getgrgid(getegid());	 
	if (  pw && gw && SYSLOG){
        	syslog(FACILITY,"[%d] (%s,%s) ran '%s'",getpid(), pw->pw_name, gw->gr_name, argvbuf ); 
        } else {        
                syslog(FACILITY,"[%d] (%d,%d) ran '%s'",getpid(),getuid(),getgid(), argvbuf  );
        }
        return 0;
};

/* Das boot. */

