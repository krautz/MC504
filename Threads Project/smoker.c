/*Project mc504
 *Cigarette Smokers Problem
 * Group: {
 * Caio Krauthamer RA: 165447
 * Heitor Boschirolli Comel RA:169477
 * João Phillipe Cardenuto RA: 170674
 *}
 *This code has contributions of The Little Books of Semaphores
 ***  link: greenteapress.com/wp/semaphores  access in 04/21/2017  ***
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0
#define MAX 3
#define MAX_CIGARETS 10

pthread_t smokers[3];   								//we own three smokers, each one has an infint suppply of a resource 
//smoker[0] has paper
//smoker[1] has tobacco
//smoker[2] has matches

pthread_t Agent, Agent_A, Agent_B, Agent_C;        		//the Agent wakes the Agents_X that supplies 2 products at a time

pthread_t Pusher_A, Pusher_B, Pusher_C;			  		//the pushers_X decide wheter or not each smoker should be awaken or not

pthread_mutex_t pushers, Smoke;							//pushers is a mutex to lock a critical region to only one pusher works at a time
														//Smoke is a mutex to lock a critival region where the smokers smoke and 															increments the number of cigarrets done so far

int product[3];         								//0 -> paper
														//1 -> tobacco
														//2 -> matches


int cigarets_done_so_far = 0, cigarets[3] = {0, 0, 0};	//cigarets_done_so_far is a counter of the number of ciragerets done
														// cigarets[3] counts how many cigarets each smoker has smoken

sem_t tobacco, paper, matches, SemAgent, SemPaper, SemTobacco, SemMatches;
														//paper, matches and tobacco are Sem to wake the Pushers_X
														//SemAgent is to wake the Agent
														//SemPaper, SemTobacco, SemMatches are to wake the smokers


void smoke (int id) {
    pthread_mutex_lock (&Smoke);                        //locking the table
    if(cigarets_done_so_far>=MAX_CIGARETS){				//Checking if we achieved the max numbers of cigarets
        printf("MAX CIGARETTES HAVE BEEN ACHIEVED ");
        pthread_mutex_unlock (&Smoke);                  
        return;
    }
    if (id == 1) {										//incrementing the number of cigarrets done by each smoker
        cigarets[0]++;
    } else if (id == 2) {
        cigarets[1]++;
    } else {
        cigarets[2]++;
    }
    cigarets_done_so_far++;
    pthread_mutex_unlock (&Smoke);                      //Starting smoking
    switch (id){

        case 1:
            printf("Smoker paper  `\\~P~/´  STARTED smoking\n");
            printf("                 |      \n\n");
            break;
        case 2:
            printf("Smoker tobacco    '\\T/'  STARTED smoking...\n");
            printf("                    |      \n\n");
            break;
        case 3:
            printf("Smoker Matches  '\\M/' STARTED smoking... \n");
            printf("                  |      \n\n");
            break;
    }
   sleep(1);
    pthread_mutex_lock (&Smoke);                   		//Finishing smoking
    switch (id){

        case 1:
            printf("Smoker paper  `\\~P~/´  FINESHED smoking\n");
            printf("                 |      \n\n");
            break;
        case 2:
             printf("Smoker tobacco    '\\T/' FINESHED smoking...\n");
             printf("                    |      \n\n");
            break;
        case 3:
            printf("Smoker Matches  '\\M/' FINESHED smoking... \n");
            printf("                  |      \n\n");
            break;
    }
    printf("Cigarets Smoked So far...\n");				//Printing the actual state of the program
    printf("| P | T | M |\n");
    printf("| %d | %d | %d |\n",cigarets[0],cigarets[1],cigarets[2]);
    pthread_mutex_unlock (&Smoke);                        
}


void *pusher_A (void *in) {								//the pushers A produce paper, if the table is full of paper he doesn't do nothing
    while (1) {											//If he can produce paper, he produces it and check if a cigaret can be done
        sem_wait (&paper);								//if this is the case he awakes the correspondent smoker
        pthread_mutex_lock(&pushers);					//else if none cigaret can be produced, he only leaves paper on the table
        if (product[0] == MAX) {
            printf("\n\n");
            printf(" The table is full of paper |##|##|##|  \n");
            printf("                            |##|##|##| \n");
            printf("                            |##|##|##| \n");
            printf("                            |##|##|##| \n");

            if (product[1]) {
                product[0]--;
                product[1]--;
                sem_post (&SemMatches);
            } else if (product[2]) {
                product[2]--;
                product[0]--;
                sem_post (&SemTobacco);
            }
        } else {	
            printf("\n\n");
            printf("Agent: Paper |##| is on the table \n");
            if (product[1]) {
                product[1]--;
                sem_post (&SemMatches);
            } else if (product[2]) {
                product[2]--;
                sem_post (&SemTobacco);
            } else if (product[0] < MAX) {
                product[0]++;
            }
        } 
        pthread_mutex_unlock(&pushers);
    }
}

void *pusher_B (void *in) {								//the pushers B produce tobacco, if the table is full of tobacco he doesn't do 																nothing
														//If he can produce tobacco, he produces it and check if a cigaret can be done
														//if this is the case he awakes the correspondent smoker
														//else if none cigaret can be produced, he only leaves tobacco on the table
    while (1) {
        sem_wait (&tobacco);
        pthread_mutex_lock(&pushers);
        if (product[1] == MAX) {
            printf("\n\n");
            printf(" The table is full of tobacco |*|.|*|.|  \n");
            printf("                              |.|*|.|*|  \n");
            printf("                              |*|.|*|.|  \n");
            printf("                              |.|*|.|*|  \n");
            if (product[0]) {
                product[1]--;
                product[0]--;
                sem_post (&SemMatches);
            } else if (product[2]) {
                product[1]--;
                product[2]--;
                sem_post (&SemPaper);
            }

        } else {
            printf("\n\n");
            printf("Agent: Tobbaco  |.*.|  is on the Table\n");
            if (product[0]) {
                product[0]--;
                sem_post (&SemMatches);
            } else if (product[2]) {
                product[2]--;
                sem_post (&SemPaper);
            } else if (product[1] < MAX) {
                product[1]++;
            }
        }
        pthread_mutex_unlock(&pushers);
    }
}

void *pusher_C (void *in) {								//the pushers C produce matches, if the table is full of matches he doesn't do 																nothing
														//If he can produce matches, he produces it and check if a cigaret can be done
														//if this is the case he awakes the correspondent smoker
														//else if none cigaret can be produced, he only leaves matches on the table
    while (1) {
        sem_wait (&matches);
        pthread_mutex_lock(&pushers);
        if (product[2] == MAX) {
            printf("\n\n");
            printf(" The table is full of matches /O / O  /O / \n");
            printf("                             / |/  | / |/ \n");
            printf("                            /o / o  /o / \n");
            printf("                           / |/  | / |/ \n");
            if (product[0]) {
                product[2]--;
                product[0]--;
                sem_post (&SemTobacco);
            } else if (product[1]) {
                product[2]--;
                product[1]--;
                sem_post (&SemPaper);
            }
        } else {
            printf("\n\n");
            printf("Agent: Matches | ---O | is on the Table\n");
            if (product[0]) {
                product[0]--;
                sem_post (&SemTobacco);
            } else if (product[1]) {
                product[1]--;
                sem_post (&SemPaper);
            } else if (product[2] < 3) {
                product[2]++;
            }
        }
        pthread_mutex_unlock(&pushers);
    }
}


void *agent (void *in) {								//this agent wakes the Agents_X
    while (1) {
        sleep(1);
        sem_post (&SemAgent);
    }
}

void *agent_A (void *in) {								 //The agent A supplies tobacco and matches
    while (1) {
        sem_wait (&SemAgent);		
        printf("\n\n");
        printf("Agent : Suppling(TABACCO AND MATCHES)\n");
        sem_post (&tobacco);
        sem_post (&matches);
    }
}

void *agent_B (void *in) {								 //The agent B supplies paper and matches
    while (1) {
        sem_wait (&SemAgent);
        printf("\n\n");
        printf("Agent : Suppling(PAPER AND MATCHES) \n");
        sem_post (&paper);
        sem_post (&matches);
    }
}

void *agent_C (void *in) {								 //The agent C supplies paper and tobacco
    while (1) {
        sem_wait (&SemAgent);
        printf("\n\n");
        printf("Agent : Suppling (PAPER AND TOBACCO)  \n");

        sem_post (&paper);
        sem_post (&tobacco);
    }
}

void *smoker_paper (void *in) {							//this smoker has only paper
    while (1) {
        sem_wait (&SemPaper);
        pthread_mutex_lock (&Smoke);  
        if (cigarets_done_so_far >= MAX_CIGARETS) {		//if the number of cigarets done is alredy at MAX, he stops producing cigarets
            pthread_mutex_unlock (&Smoke); 
            pthread_exit(NULL);
        } else {
            printf("\n\n");								//else the smoker smokes
            printf("Smoker paper  `\\~P~/´  READY to smoke...\n");
            printf("                 |      \n\n");
            pthread_mutex_unlock (&Smoke);  
            smoke(1);
        }
    }
}

void *smoker_tobacco (void *in) {						//this smoker has only tobacco
    while (1) {
        sem_wait (&SemTobacco);
        pthread_mutex_lock (&Smoke);  
        if (cigarets_done_so_far >= MAX_CIGARETS) {		//if the number of cigarets done is alredy at MAX, he stops producing cigarets
            pthread_mutex_unlock (&Smoke); 
            pthread_exit(NULL);
        } else {
            printf("\n\n");								//else the smoker smokes
            printf("Smoker tobacco    '\\T/'  READY to smoke...\n");
            printf("                    |      \n\n");
            pthread_mutex_unlock (&Smoke);  
            smoke(2);
        }
    }
}

void *smoker_matches (void *in) {						//this smoker has only matches
    while (1) {
        sem_wait (&SemMatches);
        pthread_mutex_lock (&Smoke);
        if (cigarets_done_so_far >= MAX_CIGARETS) {		//if the number of cigarets done is alredy at MAX, he stops producing cigarets
            pthread_mutex_unlock (&Smoke); 
            pthread_exit(NULL);
        } else {  
        	printf("\n\n");								//else the smoker smokes
            printf("Smoker Matches  '\\M/' READY to smoke... \n");
            printf("                  |      \n\n");
            pthread_mutex_unlock (&Smoke);  
            smoke(3);
        }
    }
}

int main () {
    printf("Smoker  with paper  `\\~P~/´   \n");
    printf("                       |      \n\n");

    printf("Smoker  with tobacco    '\\T/'   \n");
    printf("                          |     \n\n");

    printf("Smoker  with Matches     '\\M/'    \n");
    printf("                           |      \n\n");

    srand(time(NULL));
    for(int i = 0; i < 2; i++) {
        product[i] = FALSE;
    }

    sem_init (&tobacco, 1, 0);
    sem_init (&paper, 1, 0);
    sem_init (&matches, 1, 0);
    sem_init (&SemAgent, 1, 0);
    sem_init (&SemPaper, 1, 0);
    sem_init (&SemTobacco, 1, 0);
    sem_init (&SemMatches, 1, 0);

    pthread_create(&Agent, NULL, &agent, NULL);
    pthread_create(&smokers[0], NULL, &smoker_paper, NULL);
    pthread_create(&smokers[1], NULL, &smoker_tobacco, NULL);
    pthread_create(&smokers[2], NULL, &smoker_matches, NULL);
    pthread_create(&Agent_A, NULL, &agent_A, NULL);
    pthread_create(&Agent_B, NULL, &agent_B, NULL);
    pthread_create(&Agent_C, NULL, &agent_C, NULL);
    pthread_create(&Pusher_A, NULL, &pusher_A, NULL);
    pthread_create(&Pusher_B, NULL, &pusher_B, NULL);
    pthread_create(&Pusher_C, NULL, &pusher_C, NULL);

    pthread_join(smokers[0], NULL);
    pthread_join(smokers[1], NULL);
    pthread_join(smokers[2], NULL);
    printf("\n\n");
    printf("Smoker  with paper ## `\\~P~/´    ## smoked %d cigarets\n", cigarets[0]);
    printf("                   ##    |       ##\n\n");

    printf("Smoker  with tobacco *.   '\\T/'    *. smoked %d cigarets\n", cigarets[1]);
    printf("                     .*     |      .*\n\n");

    printf("Smoker  with Matches o    '\\M/'    o   smoked %d cigarets\n", cigarets[2]);
    printf("                     |      |      |\n\n");

    return 0;
} 
