#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/time.h>
#include <asm/time.h>
	

asmlinkage unsigned long  sys_randcall( unsigned long int begin , unsigned long int end) {
	unsigned long time_u ;
	unsigned long interval;
	int t_vec[6];
	int op[10];
	int res[16];
	int i,j;
	struct timeval time;
	/*ITERVAL ERROR*/
    if (end <= begin)
        return 0;

    do_gettimeofday(&time);
    time_u = time.tv_usec;
    
    /*Reescala o valor do tempo do sistema no intervalo dado*/
        //Inicializa os vetores
    for(i=0;i<6;i++)
        t_vec[i]=0;
    for(i=0;i<10;i++)
        op[i]=0; 
    for(i=0;i<16;i++)
        res[i]=0; 
    i=0;
    // Faz o vetor t_vec receber os valores de time_u
    while(time_u>0 && i <6 ){
        t_vec[i++]= time_u % 10;
        time_u /=10;
    }
    i=0;
    end = end -begin;
    // Faz o vetor t_vec receber os valores de begin -end
    while(end>0 && i < 10){
        op[i++]= end % 10;
        end /=10;
    }
    // Calcula o valor da multiplicação entre o numeor de op[]  e o numero de t_vec[]
    for(j=0;j<10;j++)
        for(i=0;i<6;i++){
            res[i+j]+=op[j]*t_vec[i];
            if(res[i+j] >=10){
                res[i+j+1]+=res[i+j]/10;
                res[i+j]=res[i+j]%10;
            }
        }

    /*Faz a divisão do resultado por 1000000 e já o coloca numa variável unsigned long int*/
    interval=0;
    for(i=15;i>5;i--){
        interval=res[i]+10*interval;
    }

    if (interval + begin == 0)
	return 1;

    return interval + begin ;
}
