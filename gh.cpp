#include "./presentation/presentation.h"

void *fctThreadFenetreGraphique(void *);
void *fctThreadEvenements(void *);
void *fctThreadStanley(void *);
void *fctThreadEnnemis(void *);
void *fctThreadGuepe(void *);
void *fctThreadChenilleG(void *);
void *fctThreadChenilleD(void *);
void *fctThreadAraigneeG(void *);
void *fctThreadAraigneeD(void *);
void *fctThreadInsecticideG(void *);
void *fctThreadInsecticideD(void *);

void handlerSIGINT(int);
void handlerSIGALRM(int);
void handlerSIGUSR1(int);
void handlerSIGUSR2(int);
void handlerSIGQUIT(int); 

void destructeurVS(void *p);
void destructeurI(void *p);

pthread_cond_t condEvenement;
pthread_cond_t condEchec;

pthread_key_t keySpec;
pthread_key_t keyGeupe;
pthread_key_t chenilles;

pthread_mutex_t mutexEtatJeu;
pthread_mutex_t mutexEvenement;
pthread_mutex_t mutexEchec;

pthread_t graphique;
pthread_t evenements;
pthread_t Stanley;
pthread_t ennemis;

typedef struct
{
 int position;
 int orientation;
} S_LOCALISATION;

typedef struct
{
    int presence;
    pthread_t tid;
} S_PRESENCE; 

typedef struct
{
    int etatStanley;
    int positionStanley;
    int actionStanley;
    int etatAmis[5];
    S_PRESENCE guepes[2];
    S_PRESENCE chenillesG[5];
    S_PRESENCE chenillesD[7];
    S_PRESENCE araigneesG[5];
    S_PRESENCE araigneesD[5];
    S_PRESENCE insecticidesG[4];
    S_PRESENCE insecticidesD[4];
    int score;
    int nbEchecs;
} S_ETAT_JEU;

S_ETAT_JEU etatJeu = 
     { BAS, 1, NORMAL,
       { NORMAL, NORMAL, NORMAL, NORMAL, NORMAL },
       { { AUCUN, 0 }, { AUCUN, 0 } },
       { { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 } },
       { { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, 
         { AUCUN, 0 }, { AUCUN, 0 } },
       { { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 } },
       { { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 } },
       { { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 } },
       { { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 }, { AUCUN, 0 } },
       0, 0 };

int evenement = AUCUN; 
int echec = AUCUN;

struct sigaction sigAct;

int main(int argc, char* argv[])
{
    int i;
    sigset_t mask;
    int nb=0;
    struct timespec temps = { 3,500000000};
    pthread_mutex_init(&mutexEtatJeu, NULL);
    pthread_mutex_init(&mutexEvenement, NULL);
    pthread_mutex_init(&mutexEchec, NULL);
    pthread_cond_init(&condEchec,NULL);
    pthread_cond_init(&condEvenement,NULL);

    pthread_key_create(&keySpec,destructeurVS);
    pthread_key_create(&keyGeupe,NULL);
    pthread_key_create(&chenilles,NULL);

    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_handler =handlerSIGALRM;
    sigAct.sa_flags = 0;
    sigaction(SIGALRM, &sigAct, NULL);

    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_handler =handlerSIGINT;
    sigAct.sa_flags = 0;
    sigaction(SIGINT, &sigAct, NULL);

    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_handler =handlerSIGUSR1;
    sigAct.sa_flags = 0;
    sigaction(SIGUSR1, &sigAct, NULL);


    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_handler =handlerSIGUSR2;
    sigAct.sa_flags = 0;
    sigaction(SIGUSR2, &sigAct, NULL);

    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_handler =handlerSIGQUIT;
    sigAct.sa_flags = 0;
    sigaction(SIGQUIT, &sigAct, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    ouvrirFenetreGraphique();

    pthread_create(&graphique,NULL,fctThreadFenetreGraphique,NULL);
    pthread_create(&evenements,NULL,fctThreadEvenements,NULL);
    pthread_create(&Stanley,NULL,fctThreadStanley,NULL);
    pthread_create(&ennemis,NULL,fctThreadEnnemis,NULL);

    while(nb<3)
    {
        pthread_mutex_lock(&mutexEchec);
            while(echec==AUCUN)
            {
                pthread_cond_wait(&condEchec,&mutexEchec);
            }
        pthread_mutex_lock(&mutexEtatJeu);
            etatJeu.nbEchecs++;
            nb=etatJeu.nbEchecs;
            etatJeu.etatAmis[echec]=TOUCHE;
            
        pthread_mutex_unlock(&mutexEtatJeu);
        nanosleep(&temps, NULL);
        pthread_mutex_lock(&mutexEtatJeu);
            etatJeu.etatAmis[echec]=NORMAL;
            if(etatJeu.nbEchecs<3)
            {
                echec=AUCUN;
            }
        pthread_mutex_unlock(&mutexEtatJeu);
        pthread_mutex_unlock(&mutexEchec); 
    }
    printf("fin\n");
    pthread_join(evenements,NULL);
    return 0;
   
}
void* fctThreadFenetreGraphique(void*)
{
    int i;
    while(true)
    {
        struct timespec temps = { 0,100000000};
        restaurerImageInterne(); // restaure l’image initiale du jeu en mémoire
        pthread_mutex_lock(&mutexEtatJeu);
            afficherStanley(etatJeu.etatStanley, etatJeu.positionStanley, etatJeu.actionStanley);
            //afficherStanley(HAUT, i, SPRAY);

            //afficherStanley(ECHELLE, 0);
            afficherAmi(FLEUR_HG, etatJeu.etatAmis[FLEUR_HG]);
            afficherAmi(FLEUR_HD, etatJeu.etatAmis[FLEUR_HD]);
            afficherAmi(FLEUR_BG, etatJeu.etatAmis[FLEUR_BG]);
            afficherAmi(FLEUR_BD, etatJeu.etatAmis[FLEUR_BD]);
            afficherAmi(CHAT, etatJeu.etatAmis[CHAT]);
            for(i=0;i<2;i++)
            {
                if(etatJeu.guepes[i].presence==NORMAL)
                {
                    afficherGuepe(i);
                }
            }
            for(i=0;i<5;i++)
            {
                if(etatJeu.chenillesG[i].presence==NORMAL)
                {
                    afficherChenilleG(i);
                }
            }
            for(i=0;i<7;i++)
            {
                if(etatJeu.chenillesD[i].presence==NORMAL)
                {
                    afficherChenilleD(i);
                }
            }
            for(i=0;i<5;i++)
            {
                if(etatJeu.araigneesG[i].presence==NORMAL)
                {
                    afficherAraigneeG(i);
                }
            }
            for(i=0;i<5;i++)
            {
                if(etatJeu.araigneesD[i].presence==NORMAL)
                {
                    afficherAraigneeD(i);
                }
            }
            for(i=0;i<4;i++)
            {
                if(etatJeu.insecticidesG[i].presence==NORMAL)
                {
                    afficherInsecticideG(i);
                }
            }
            for(i=0;i<4;i++)
            {
                if(etatJeu.insecticidesD[i].presence==NORMAL)
                {
                    afficherInsecticideD(i+1);
                }
            }

            afficherEchecs(etatJeu.nbEchecs);
            afficherScore(etatJeu.score);

        pthread_mutex_unlock(&mutexEtatJeu);
        actualiserFenetreGraphique(); // rendre visible l’image interne du jeu
        nanosleep(&temps, NULL);
    }
    pthread_exit(0);
}
void *fctThreadEvenements(void *)
{
    struct timespec temps = { 0,100000000};
    while(1)
    {
        pthread_mutex_lock(&mutexEvenement);
        evenement = lireEvenement();

        switch(evenement)
        {
            case SDL_QUIT:
                exit(0);

            case SDLK_UP:
                printf("KEY_UP\n");
                break;

            case SDLK_DOWN:
                printf("KEY_DOWN\n");
                break;

            case SDLK_LEFT:
                printf("KEY_LEFT\n");
                break;

            case SDLK_RIGHT:
                printf("KEY_RIGHT\n");
                break;

            case SDLK_SPACE:
                printf("SDLK_SPACE\n");
        }
        pthread_mutex_unlock(&mutexEvenement);
        pthread_cond_signal(&condEvenement);
        nanosleep(&temps, NULL);
    }
}

void* fctThreadStanley(void*)
{
    pthread_t ptid;
    struct timespec temps = { 0,200000000}; 
    int i;
    while(true)
    {
        pthread_mutex_lock(&mutexEvenement);
        while(evenement==-1)
        {
            pthread_cond_wait(&condEvenement,&mutexEvenement);
        }

        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);
        
            pthread_mutex_lock(&mutexEtatJeu); 
            switch(etatJeu.etatStanley)
            {
                case BAS:
                    switch(evenement)
                    {
                        case SDLK_SPACE:
                            if(etatJeu.positionStanley == 0 || etatJeu.positionStanley==2 || etatJeu.positionStanley==3 )
                            {

                                etatJeu.actionStanley = SPRAY;
                                pthread_mutex_unlock(&mutexEtatJeu);
                                nanosleep(&temps, NULL);
                                pthread_mutex_lock(&mutexEtatJeu);
                                if(etatJeu.positionStanley==2)
                                {
                                    for(int j=0;j<2;j++){
                                        if(etatJeu.guepes[j].presence==NORMAL)
                                        {

                                            pthread_kill(etatJeu.guepes[j].tid,SIGINT);
                                        }
                                    }
                                    
                                }
                                if(etatJeu.positionStanley==0)
                                {
                                    if(etatJeu.araigneesG[4].presence==NORMAL)
                                    {
                                        pthread_kill(etatJeu.araigneesG[4].tid,SIGUSR2);
                                    }
                                    else
                                    {
                                        pthread_create(&ptid,NULL,fctThreadInsecticideG,NULL);
                                    }
                                }
                                if(etatJeu.positionStanley==3)
                                {
                                    if(etatJeu.araigneesD[0].presence==NORMAL)
                                    {
                                        pthread_kill(etatJeu.araigneesD[0].tid,SIGUSR2);
                                    }
                                    else
                                    {
                                        pthread_create(&ptid,NULL,fctThreadInsecticideD,NULL);
                                    }
                                }
                                etatJeu.actionStanley = NORMAL;

                            }

                            break;
                        case SDLK_LEFT:
                            if(etatJeu.positionStanley > 0)
                                etatJeu.positionStanley--;
                            break;
                        case SDLK_RIGHT:
                            if(etatJeu.positionStanley<3)
                                etatJeu.positionStanley++;
                            break;
                        case SDLK_UP:
                            if(etatJeu.positionStanley==1)
                            {
                                etatJeu.etatStanley=ECHELLE;
                                etatJeu.positionStanley=1;
                            }
                            break;
                    }
                    break;
                case ECHELLE:
                    switch (evenement)
                    {
                        case SDLK_UP:
                            if(etatJeu.positionStanley==1)
                            {
                                etatJeu.positionStanley=0;
                            }
                            else{
                                etatJeu.etatStanley=HAUT;
                                etatJeu.positionStanley=2;
                            }
                            break;
                        case SDLK_DOWN:
                            if(etatJeu.positionStanley==1)
                            {
                                etatJeu.etatStanley=BAS;
                                etatJeu.positionStanley=1;
                            }
                            else
                            {
                                etatJeu.positionStanley=1;
                            }
                            break;
                    }   
                    break;
                case HAUT:
                    switch (evenement)
                    {
                        case SDLK_SPACE:
                            if(etatJeu.positionStanley == 0 || etatJeu.positionStanley==1 || etatJeu.positionStanley==3 || etatJeu.positionStanley==4 ||etatJeu.positionStanley==5 )
                            {
                                etatJeu.actionStanley = SPRAY;
                                pthread_mutex_unlock(&mutexEtatJeu);
                                nanosleep(&temps, NULL);
                                pthread_mutex_lock(&mutexEtatJeu);
                                i=etatJeu.positionStanley;

                                if(etatJeu.positionStanley==0 )
                                {
                                    if(etatJeu.chenillesG[0].presence==NORMAL)
                                    {
                                        pthread_kill(etatJeu.chenillesG[0].tid,SIGUSR1);
                                    }
                                    if(etatJeu.chenillesG[1].presence==NORMAL)
                                    {
                                        pthread_kill(etatJeu.chenillesG[1].tid,SIGUSR1);
                                    }
                                }
                                if(etatJeu.positionStanley==1)
                                {
                                    if(etatJeu.chenillesG[2].presence==NORMAL)
                                    {
                                        pthread_kill(etatJeu.chenillesG[2].tid,SIGUSR1);
                                    }
                                    if(etatJeu.chenillesG[3].presence==NORMAL)
                                    {
                                        pthread_kill(etatJeu.chenillesG[3].tid,SIGUSR1);
                                    }
                                }
                                else{

                                    if(etatJeu.positionStanley==3 || etatJeu.positionStanley==4 ||etatJeu.positionStanley==5)
                                    {
                                        i=etatJeu.positionStanley-3;
                                        for(int j=2*i+1;j<2*i+3;j++)
                                        {
                                            if(etatJeu.chenillesD[j].presence==NORMAL){
                                                pthread_kill(etatJeu.chenillesD[j].tid,SIGUSR1);
                                            }
                                        }  
                                    }
                                }
                                etatJeu.actionStanley = NORMAL;
                            }
                            break;
                        case SDLK_DOWN:
                            if(etatJeu.positionStanley==2)
                            {
                                etatJeu.etatStanley=ECHELLE;
                                etatJeu.positionStanley=0;
                            }
                            break;
                        case SDLK_LEFT:
                            if(etatJeu.positionStanley>0){
                                etatJeu.positionStanley--;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(etatJeu.positionStanley<5){
                                etatJeu.positionStanley++;
                            }
                     
                    }   

                    break;
                       
            }
        }
        else{
            pthread_mutex_unlock(&mutexEchec);
        }

        pthread_mutex_unlock(&mutexEtatJeu);
            evenement = AUCUN;
        pthread_mutex_unlock(&mutexEvenement);              
    }
    pthread_exit(0);

}

void *fctThreadEnnemis(void *)
{
    int i;
    sigset_t mask;
    pthread_t ptid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    alarm(10);
    struct timespec *timrest=(struct timespec*)malloc(sizeof(struct timespec));
    timrest->tv_sec=1;
    timrest->tv_nsec=600000000;
    pthread_setspecific(keySpec,timrest);
    srand(time(NULL));
    while(1)
    {
        nanosleep((struct timespec *)pthread_getspecific(keySpec),NULL);
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN){
            pthread_mutex_unlock(&mutexEchec);
            i=rand()%5;
            switch(i)
            {
                case 0:
                {
                    pthread_create(&ptid,NULL,fctThreadGuepe,NULL);
                }
                break;
                case 1:
                    pthread_create(&ptid,NULL,fctThreadChenilleG,NULL);
                break;
                case 2:
                    pthread_create(&ptid,NULL,fctThreadChenilleD,NULL);
                break;
                case 3:
                    pthread_create(&ptid,NULL,fctThreadAraigneeG,NULL);
                break;
                case 4:
                    pthread_create(&ptid,NULL,fctThreadAraigneeD,NULL);
                break;

            }
        }
        else{
            pthread_mutex_unlock(&mutexEchec);
        }
    }
}
void handlerSIGALRM(int)
{
    struct timespec *timrest=(struct timespec *)pthread_getspecific(keySpec);
    if(timrest->tv_nsec==100000000)
    {
        alarm(0);
        printf("fin de alarme  \n");
    }
    else
    {
        timrest->tv_nsec=timrest->tv_nsec-50000000;
        alarm(10);
        pthread_setspecific(keySpec,timrest);
        printf("alarme lancer \n");
    }
}
void destructeurVS(void *p)
{
    free(p);
}

void *fctThreadGuepe(void *)
{
  sigset_t mask;
    struct timespec temps= { 1,0}; 
    struct timespec t= {1,500000000};
    int*position=(int*)malloc(sizeof(int));
    pthread_setspecific(keyGeupe,position);
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    
    (*position)=0;

    while((*position)<2)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN){
            pthread_mutex_unlock(&mutexEchec);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
            if(etatJeu.etatStanley==BAS && etatJeu.positionStanley==2 && etatJeu.actionStanley==SPRAY)
            {
                if((*position)>0)
                {
                    etatJeu.guepes[(*position)-1].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            if(etatJeu.guepes[(*position)].presence==AUCUN)
            {
                etatJeu.guepes[(*position)].presence=NORMAL;
                etatJeu.guepes[(*position)].tid=pthread_self();
                if((*position)==1)
                {
                    etatJeu.guepes[0].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    sigprocmask(SIG_UNBLOCK, &mask, NULL);
                    nanosleep(&t, NULL);
                    sigprocmask(SIG_BLOCK, &mask, NULL);
                    pthread_mutex_lock(&mutexEchec);
                        echec=CHAT;
                    pthread_mutex_unlock(&mutexEchec);
                    pthread_cond_signal(&condEchec);
                    nanosleep(&t, NULL);
                    pthread_mutex_lock(&mutexEtatJeu);
                    etatJeu.guepes[1].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            else{
                pthread_mutex_unlock(&mutexEtatJeu);
                sigprocmask(SIG_UNBLOCK, &mask, NULL);
                if((*position)==1){
                    (*position)--;
                }
                else{
                    (*position)=2;
                }
            }
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            nanosleep(&temps, NULL);
            (*position)++;
            
        }
        else{
            pthread_mutex_unlock(&mutexEchec);
        }
    }

    pthread_exit(0);
}

void handlerSIGINT(int)
{
    int*i=(int*)pthread_getspecific(keyGeupe);
    printf("SIGINT \n");
    if(pthread_getspecific(keyGeupe)!=NULL)
    {
        pthread_mutex_lock(&mutexEtatJeu);
            etatJeu.guepes[(*i)].presence=AUCUN; 
            etatJeu.score++;
        pthread_mutex_unlock(&mutexEtatJeu);
        pthread_exit(0);
    }
}
void *fctThreadChenilleG(void *){

    sigset_t mask;
    int i;
    struct timespec temps= { 1,0}; 
    struct timespec t= {1,500000000};
    S_LOCALISATION*localisation=(S_LOCALISATION*)malloc(sizeof(S_LOCALISATION));
    pthread_setspecific(chenilles,localisation);

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    localisation->position=4;
    localisation->orientation=0;

    while((localisation->position)>=0)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
            if(etatJeu.etatStanley==HAUT && (etatJeu.positionStanley==0 || etatJeu.positionStanley==1) && etatJeu.actionStanley==SPRAY){
                i=etatJeu.positionStanley;
                if(etatJeu.positionStanley==0 &&((localisation->position+1)==0 || (localisation->position+1)==1)){
                    etatJeu.chenillesG[(localisation->position)+1].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
                if(etatJeu.positionStanley==1 &&((localisation->position+1)==2 || (localisation->position+1)==3))
                {
                    etatJeu.chenillesG[(localisation->position)+1].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }

            }
            
            if(etatJeu.chenillesG[localisation->position].presence==AUCUN)
            {
                etatJeu.chenillesG[localisation->position].presence=NORMAL;
                etatJeu.chenillesG[localisation->position].tid=pthread_self();
                if(localisation->position<4)
                {
                    etatJeu.chenillesG[localisation->position+1].presence=AUCUN;
                }
                pthread_mutex_unlock(&mutexEtatJeu);
                sigprocmask(SIG_UNBLOCK, &mask, NULL);
                if(localisation->position==0)
                {
                    nanosleep(&temps, NULL);
                    sigprocmask(SIG_BLOCK, &mask, NULL);

                    pthread_mutex_lock(&mutexEchec);
                        echec=FLEUR_HG;
                    pthread_mutex_unlock(&mutexEchec);
                    pthread_cond_signal(&condEchec);
                    nanosleep(&t, NULL);

                    pthread_mutex_lock(&mutexEtatJeu);
                    etatJeu.chenillesG[0].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    
                    pthread_exit(0);
                }
            }
            else
            {
                if(localisation->position==4)
                {
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
                else
                {
                    localisation->position++;
                }
            }
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            nanosleep(&temps, NULL);
            localisation->position--;
        }
        else
        {
            pthread_mutex_unlock(&mutexEchec);
        }
    }
    pthread_exit(0);
}
void handlerSIGUSR1(int)
{
    S_LOCALISATION *localisation=(S_LOCALISATION*)pthread_getspecific(chenilles);
    printf("SIGUSR1 \n",pthread_self());
    if(pthread_getspecific(chenilles)!=NULL){
        pthread_mutex_lock(&mutexEtatJeu);
        if(localisation->orientation==0)
        {
            etatJeu.chenillesG[localisation->position].presence=AUCUN; 
        }
        else
        {
            etatJeu.chenillesD[localisation->position].presence=AUCUN; 
        }
        etatJeu.score++;
        pthread_mutex_unlock(&mutexEtatJeu);
        pthread_exit(0);
    }
}
void *fctThreadChenilleD(void *)
{
    int i;
    sigset_t mask;
    struct timespec temps= { 1,0}; 
    struct timespec t= {1,500000000};
    S_LOCALISATION*localisation=(S_LOCALISATION*)malloc(sizeof(S_LOCALISATION));
    pthread_setspecific(chenilles,localisation);

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    localisation->position=0;
    localisation->orientation=1;
    while((localisation->position)<=6)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);

            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
            if(etatJeu.etatStanley==HAUT && (etatJeu.positionStanley==3 || etatJeu.positionStanley==4 || etatJeu.positionStanley==5) && etatJeu.actionStanley==SPRAY)
            {
                if(etatJeu.positionStanley==3 &&((localisation->position-1)==1 || (localisation->position-1)==2))
                {
                    etatJeu.chenillesD[(localisation->position)-1].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
                if(etatJeu.positionStanley==4 &&((localisation->position-1)==3 || (localisation->position-1)==4))
                {
                    etatJeu.chenillesD[(localisation->position)-1].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
                if(etatJeu.positionStanley==5 &&((localisation->position-1)==5 || (localisation->position-1)==6))
                {
                    etatJeu.chenillesD[(localisation->position)-1].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            
            if(etatJeu.chenillesD[localisation->position].presence==AUCUN)
            {
                etatJeu.chenillesD[localisation->position].presence=NORMAL;
                etatJeu.chenillesD[localisation->position].tid=pthread_self();
                if(localisation->position>0)
                {
                    etatJeu.chenillesD[localisation->position-1].presence=AUCUN;
                }
                pthread_mutex_unlock(&mutexEtatJeu);
                sigprocmask(SIG_UNBLOCK, &mask, NULL);
                if(localisation->position==6)
                {
                    nanosleep(&temps, NULL);
                    sigprocmask(SIG_BLOCK, &mask, NULL);

                    pthread_mutex_lock(&mutexEchec);
                        echec=FLEUR_HD;
                    pthread_mutex_unlock(&mutexEchec);
                    pthread_cond_signal(&condEchec);

                    nanosleep(&t, NULL);

                    pthread_mutex_lock(&mutexEtatJeu);
                    etatJeu.chenillesD[6].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    sigprocmask(SIG_UNBLOCK, &mask, NULL);
                    pthread_exit(0);
                }
            }
            else
            {
                if(localisation->position==0)
                {
                    pthread_mutex_unlock(&mutexEtatJeu);  
                    pthread_exit(0);
                }
                else
                {
                    localisation->position--;
                }
            }
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            nanosleep(&temps, NULL);
            localisation->position++;
        }
        else
        {
            pthread_mutex_unlock(&mutexEchec);
        }
    }
    pthread_exit(0);
}
void *fctThreadAraigneeG(void *)
{
    int i;
    sigset_t mask;
    struct timespec temps= { 1,0}; 
    struct timespec t= {1,500000000};
    S_LOCALISATION*localisation=(S_LOCALISATION*)malloc(sizeof(S_LOCALISATION));
    pthread_setspecific(chenilles,localisation);

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    localisation->position=0;
    localisation->orientation=0;

    while((localisation->position)<=4)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
            if(etatJeu.araigneesG[localisation->position].presence==AUCUN)
            {
                etatJeu.araigneesG[localisation->position].presence=NORMAL;
                etatJeu.araigneesG[localisation->position].tid=pthread_self();
                if(localisation->position<4)
                {
                    etatJeu.araigneesG[localisation->position-1].presence=AUCUN;
                }
            }
            else
            {
                if(localisation->position==0)
                {
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
                /*else
                {
                    localisation->position++;
                }*/
            }
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            
            nanosleep(&temps, NULL);
                
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
           
            if((localisation->position)==4)
            {
                if(etatJeu.etatStanley==BAS && etatJeu.positionStanley==0  && etatJeu.actionStanley==SPRAY)
                {
                    etatJeu.araigneesG[(localisation->position)].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            else{
                if(etatJeu.insecticidesG[(localisation->position)].presence==NORMAL)
                {           
                    pthread_kill(etatJeu.insecticidesG[(localisation->position)].tid,SIGQUIT);
                    etatJeu.araigneesG[(localisation->position)].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            localisation->position++;
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
        else
        {
            pthread_mutex_unlock(&mutexEchec);
        }
    }
    sigprocmask(SIG_BLOCK, &mask, NULL);
    pthread_mutex_lock(&mutexEchec);
    echec = FLEUR_BD;
    pthread_mutex_unlock(&mutexEchec);
    pthread_cond_signal(&condEchec);

    nanosleep(&temps, NULL);

    pthread_mutex_lock(&mutexEtatJeu);
    etatJeu.araigneesG[4].presence = AUCUN;
    pthread_mutex_unlock(&mutexEtatJeu);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    
        
    pthread_exit(0);
}

void handlerSIGUSR2(int)
{
    S_LOCALISATION *localisation=(S_LOCALISATION*)pthread_getspecific(chenilles);
    printf("SIGUSR2 \n",pthread_self());
    if(pthread_getspecific(chenilles)!=NULL)
    {
        pthread_mutex_lock(&mutexEtatJeu);
        if(localisation->orientation==0)
        {
            etatJeu.araigneesG[localisation->position].presence=AUCUN; 
        }
        else
        {
            etatJeu.araigneesD[localisation->position].presence=AUCUN; 
        }
        etatJeu.score++;
        pthread_mutex_unlock(&mutexEtatJeu);
        pthread_exit(0);
    }
}
void *fctThreadAraigneeD(void *)
{
    int i;
    sigset_t mask;
    struct timespec temps= { 1,0}; 
    struct timespec t= {1,500000000};
    S_LOCALISATION*localisation=(S_LOCALISATION*)malloc(sizeof(S_LOCALISATION));
    pthread_setspecific(chenilles,localisation);

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    localisation->position=4;
    localisation->orientation=1;

    while((localisation->position)>=0)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
            if(etatJeu.araigneesD[localisation->position].presence==AUCUN)
            {
                etatJeu.araigneesD[localisation->position].presence=NORMAL;
                etatJeu.araigneesD[localisation->position].tid=pthread_self();
                if(localisation->position<4)
                {
                    etatJeu.araigneesD[localisation->position+1].presence=AUCUN;
                }
            }
            else
            {
                if(localisation->position==4)
                {
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
                /*else
                {
                    localisation->position++;
                }*/
            }
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            
            nanosleep(&temps, NULL);
                
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
           
            if((localisation->position)==0)
            {
                if(etatJeu.etatStanley==BAS && etatJeu.positionStanley==3  && etatJeu.actionStanley==SPRAY)
                {
                    etatJeu.araigneesD[(localisation->position)].presence=AUCUN;
                    etatJeu.score++;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            else{
                if(etatJeu.insecticidesD[(localisation->position)-1].presence==NORMAL)
                {           
                    pthread_kill(etatJeu.insecticidesD[(localisation->position)-1].tid,SIGQUIT);
                    etatJeu.araigneesD[(localisation->position)].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }
            }
            localisation->position--;
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
        else
        {
            pthread_mutex_unlock(&mutexEchec);
        }
    }
    sigprocmask(SIG_BLOCK, &mask, NULL);
    pthread_mutex_lock(&mutexEchec);
    echec = FLEUR_BD;
    pthread_mutex_unlock(&mutexEchec);
    pthread_cond_signal(&condEchec);

    nanosleep(&temps, NULL);

    pthread_mutex_lock(&mutexEtatJeu);
    etatJeu.araigneesD[0].presence = AUCUN;
    pthread_mutex_unlock(&mutexEtatJeu);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    
        
    pthread_exit(0);
}
void *fctThreadInsecticideG(void *){
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    struct timespec temps= {0,200000000};
    S_LOCALISATION*localisation=(S_LOCALISATION*)malloc(sizeof(S_LOCALISATION));
    pthread_setspecific(chenilles,localisation);
    localisation->orientation=0;
    localisation->position=3;

    while((localisation->position)>=0)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
                etatJeu.insecticidesG[localisation->position].presence=NORMAL;
                etatJeu.insecticidesG[localisation->position].tid=pthread_self();
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            nanosleep(&temps, NULL);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
                if(etatJeu.araigneesG[localisation->position].presence==NORMAL)
                {
                    pthread_kill(etatJeu.araigneesG[localisation->position].tid,SIGUSR2);
                    etatJeu.insecticidesG[(localisation->position)].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }

                etatJeu.insecticidesG[localisation->position].presence=AUCUN;
                localisation->position--;
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
        else
        {
            pthread_mutex_unlock(&mutexEchec);
        }
    }
    pthread_mutex_lock(&mutexEtatJeu);
    etatJeu.insecticidesG[0].presence=AUCUN;
    pthread_mutex_unlock(&mutexEtatJeu);
    pthread_exit(0);
}
void handlerSIGQUIT(int)
{
    S_LOCALISATION *localisation=(S_LOCALISATION*)pthread_getspecific(chenilles);
    printf("SIGQUIT \n",pthread_self());
    if(pthread_getspecific(chenilles)!=NULL)
    {
        pthread_mutex_lock(&mutexEtatJeu);
        if(localisation->orientation==0)
        {
            etatJeu.insecticidesG[localisation->position].presence=AUCUN; 
        }
        else
        {
            etatJeu.insecticidesD[localisation->position].presence=AUCUN; 
        }
        etatJeu.score++;
        pthread_mutex_unlock(&mutexEtatJeu);
        pthread_exit(0);
    }
}
void *fctThreadInsecticideD(void *){

    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    struct timespec temps= {0,200000000};
    S_LOCALISATION*localisation=(S_LOCALISATION*)malloc(sizeof(S_LOCALISATION));
    pthread_setspecific(chenilles,localisation);
    localisation->orientation=1;
    localisation->position=0;

    while((localisation->position)<=3)
    {
        pthread_mutex_lock(&mutexEchec);
        if(echec==AUCUN)
        {
            pthread_mutex_unlock(&mutexEchec);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
                etatJeu.insecticidesD[localisation->position].presence=NORMAL;
                etatJeu.insecticidesD[localisation->position].tid=pthread_self();
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            nanosleep(&temps, NULL);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            pthread_mutex_lock(&mutexEtatJeu);
                if(etatJeu.araigneesD[localisation->position+1].presence==NORMAL)
                {
                    pthread_kill(etatJeu.araigneesD[localisation->position+1].tid,SIGUSR2);
                    etatJeu.insecticidesD[(localisation->position)].presence=AUCUN;
                    pthread_mutex_unlock(&mutexEtatJeu);
                    pthread_exit(0);
                }

                etatJeu.insecticidesD[localisation->position].presence=AUCUN;
                localisation->position++;
            pthread_mutex_unlock(&mutexEtatJeu);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
        else
        {
            pthread_mutex_unlock(&mutexEchec);
        }
    }
    pthread_mutex_lock(&mutexEtatJeu);
    etatJeu.insecticidesD[3].presence=AUCUN;
    pthread_mutex_unlock(&mutexEtatJeu);
    pthread_exit(0);
}