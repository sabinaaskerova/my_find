#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define PROG_ERROR 2

#define CHK(op) do { if ((op) == -1) raler (1, #op); } while (0)

noreturn void raler (int syserr, const char *msg, ...)
{
    va_list ap;

    va_start (ap, msg);
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");
    va_end (ap);

    if (syserr == 1)
        perror ("");

    exit (EXIT_FAILURE);
}


void parcours_rep(char *dir, char *com, char *options[], int argc){
    int a;
    //test d'existence d'un répertoire
    if((a = access(dir, F_OK)) == -1){
        fprintf(stderr,"Répertoire n'existe pas\n");
        exit(EXIT_FAILURE);
    }

    DIR *dp = opendir(dir);
    if (dp == NULL){
        raler(1, "opendir");
    } 
    

    struct dirent *d;
    //le nom du fichier en cours
    char buf[PATH_MAX]; 

    //contient la commande pour exec
    char path[PATH_MAX]; 

    errno = 0;
    //lecture du répertoire
    while ((d = readdir(dp)) != NULL ){

        //on met le nom de lu fichier dans le buffer buf
        int ret = snprintf(buf, PATH_MAX, "%s/%s", dir, d->d_name); 
        if(ret<0 || ret >=PATH_MAX){
            exit(PROG_ERROR);
        }

        //on met le nom de la commande dans le buffer path
        int ret_path = snprintf(path, sizeof path, "%s", com);
        if(ret_path<0 || ret_path >=PATH_MAX){
            exit(PROG_ERROR);
        }
        
        
        //raison de wait
        int raison;
        //code de retour de exec
        int ex; 
        //stat du fichier actuel
        struct stat stat_file;
        //on prend en compte les liens symboliques
        CHK(lstat(buf, &stat_file));

        //test si le fichier est un fichier régulier
        if(S_ISREG(stat_file.st_mode)){
            //on met le nom du fichier et NULL dans le tableau d'options
            options[argc-2] = buf;
            options[argc-1] = NULL;

            int code_fork = fork();
            switch(code_fork){
        
            case -1:
                exit(EXIT_FAILURE);
            case 0:
                //execution de la commande
                if((ex = execvp(path, options))==-1){
                    fprintf(stderr,"Problème exec\n");
                    exit(127);
                }
                break;
            default:
                //attente du processus fils
                if(wait(&raison) == -1){
                    raler(1, "wait échoué");
                }
                //test si le processus fils a échoué
                if(WEXITSTATUS(raison) == 127){
                exit(1);
            }
                break;
            }
        } 
        //test si le fichier est un dossier
        else if(S_ISDIR(stat_file.st_mode)){
            //appel récursif sur les sous-répéroires sauf "." et ".."
            if(d->d_name[0] != '.'){
                parcours_rep(buf, com, options, argc);
            }
        } 
        //si ni fichier régulier ni répértoire
        else {
            fprintf(stderr, "Ni répertoire ni fichier");
        } 
    }
    if(errno != 0){
        fprintf(stderr, "Problème avec readdir");
    }
    
    
    CHK(closedir(dp));
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr,"Utilisation : %s <repertoire> <commande>\n",argv[0]);
        exit(PROG_ERROR);
    }
    //le répértoire
    char *dir=argv[1];
    //la commande à éxecuter
    char *com=argv[2];
    //tableau des options de la commande
    char *options[argc];

    //initialisation du tableau des options
    for(int i = 0; i< argc - 2; i++){
        options[i] = argv[i+2];
    }

    parcours_rep(dir, com, options, argc);

    exit(0);
}