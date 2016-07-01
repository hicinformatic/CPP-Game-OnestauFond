#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <string.h>


#define no_argument 0
#define required_argument 1 
#define optional_argument 2

const char* xversion = "v0.0-Enjoy";
static bool verbose  = false;
static bool help     = false;
static bool version  = false;
static int port      = 26026;
static int threads   = 1;
static int end;

const char* msgHelp    = "Affiche l'aide";
const char* msgVerbose = "Mode verbeux actif";
const char* msgThreads = "Activation des threads";
const char* msgPort    = "Création d'un socket unique:";
const char* msgEnd     = "Création de plusieurs sockets:";

const char* errEnd     = "Le port de fin(-e, --end) doit être supèrieur au port d'entrée (-p, --port, 26026)";
const char* errPort    = "Impossible d'ouvrir un socket sur le port: ";
const char* errBind    = "Bind impossible!";

int Start();
int addPort(int port);


void msg(int type, const char *msg)
{
    if(verbose && type < 1)
    {
        printf(msg);
        putchar('\n');
    }else{
        printf(msg);
        putchar('\n');
    }
}

int main(int argc, char* argv[])
{
    int c;
    while(1)
    {
        const struct option long_options[] =
        {
            {"help",      no_argument,        0, 'h'},
            {"end",       required_argument,  0, 'e'},
            {"port",      required_argument,  0, 'p'},
            {"threads",   required_argument,  0, 't'},
            {"verbose",   optional_argument,  0, 'v'},
            {"xversion",  no_argument,        0, 'x'},
            {0,0,0,0},
        };

        int option_index = 0;
        c = getopt_long (argc, argv, "he:p:t:v::x",long_options, &option_index);

        if(c == -1)
            break;

        switch(c)
        {
            case 'h':
                help = true;
                break;
            case 'e':
                end = atoi(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                threads = atoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'x':
                version = true;
                break;
            case '?':
                break;
            default:
                help = true;
        }
    }

    if(help)
    {
        msg(0, "help");
        exit(0);
    }

    if(version)
    {
        msg(0, xversion);
        exit(0);
    }

    if(verbose)
    {
        msg(0, msgVerbose);
    }

    Start();
    exit(0);
}

int Start()
{
    if(end)
    {
        if(end <= port)
        {
            msg(2, errEnd);
        }else{
            int portThreads = end - port;
            msg(0, msgEnd);
            char nbr[15];
            sprintf(nbr, "  n Port: %d", portThreads+1);
            msg(0, nbr);

            pthread_t threadA[portThreads];
            for(int p=port; p<=end; ++p)
            {
                addPort(p);
            }
        }
    }else{
        msg(0, msgPort);
        addPort(port);
    }
}

int addPort(int port)
{
    char cp[15];
    sprintf(cp, "  + Port: %d", port);
    msg(0, cp);

    int sockfd, newsockfd, listenFd;
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenFd < 0)
    {
        msg(2, errPort);
        msg(2, cp);
        return 0;
    }

    struct sockaddr_in svrAdd, clntAdd;
    bzero((char*) &svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(port);

    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        msg(2, errBind);
        return 0;
    }
    listen(listenFd, 5);
    len = sizeof(clntAdd);
}