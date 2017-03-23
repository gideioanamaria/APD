#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define nrThread 4    //numarul de threaduri folosit

int matrice_A[500][500];
int matrice_B[500][500];
int matrice_Rezultat[500][500];

int lA, cA;       // lA,cA - numarul de lini si coloane din matricea A
int lB, cB;       // lB,cB - numarul de lini si coloane din matricea B

//functia care va fi executata de toate thread-urile
void *Inmultire_Matrici(void *param)
{

    int i,j,k;

    for (i = 0; i < lA; i++)
    {
        for(j = 0; j < cB; j++)
        {
            for(k = 0; k < lB; k++)
            {
                matrice_Rezultat[i][j]+=matrice_A[i][k] * matrice_B[k][j];
            }
        }
    }

    pthread_exit(NULL);

}

int main()
{
    pthread_t thread[nrThread];

	int i, j;

	int ok = 1; // ok = 0 inmultirea matricilor nu este posibila, ok = 1 inmultirea matricilor este posibila

	//Citesc de la tastatura numarul de lini si coloane ale matricei A

	printf("Introduce-ti numarul de linii pentru Matricea A :");
    scanf("%d",&lA);

    printf("Introduce-ti numarul de coloane pentru Matricea A :");
    scanf("%d",&cA);

    //Initializare matricea A cu valori random intre 0 si 10

    for(i=0;i<lA;i++)
    {
        for(j=0;j<cA;j++)
        {
            matrice_A[i][j] = rand()%10;
            printf("matrice_A[%d][%d] :",i,j);
            printf("%d \n",matrice_A[i][j]);
        }
    }

    printf("\n");

    //Citesc de la tastatura numarul de lini si coloane ale matricei B

    printf("Introduce-ti numarul de linii pentru Matricea B :");
    scanf("%d",&lB);

    printf("Introduce-ti numarul de coloane pentru Matricea B :");
    scanf("%d",&cB);

    //Initializare matricea B cu valori random intre 0 si 10

    for(i=0;i<lB;i++)
    {
        for(j=0;j<cB;j++)
        {
            matrice_B[i][j] = rand()%10;
            printf("matrice_B[%d][%d] :",i,j);
            printf("%d \n",matrice_B[i][j]);
        }
    }

    //Initializare matrice rezultat cu 0

    for(i=0;i<lA;i=i+2)
        {
            for(j=0;j<cB;j=j+2)
            {
                matrice_Rezultat[i][j]=0;
            }
        }

    //Daca numarul de coloane din matricea A nu este egal cu numarul de lini din matricea B inmultirea nu este posibila

    if(cA!=lB)
    {
        printf("\nInmultirea matricilor nu este posibila !!! \n");
        ok = 0;
    }
    else
    {
        //Vom crea nrThread de thread-uri pentru a inmulti cele doua matrici
        for (i = 0; i < nrThread; i++) {
            pthread_create(&thread[i], NULL, Inmultire_Matrici, (void *)i);
        }

        for (i = 0; i < nrThread; i++)
        {
            pthread_join(thread[i], NULL);
        }
    }

    //Afisam rezultatul inmultiri celor 2 matrici in  cazul in care aceeasta a fost posibila

    if(ok == 1)
    {

        printf("\n Rezultatul inmultiri matricilor A si B : \n");

        for(i=0;i<lA;i++)
        {
            for(j=0;j<cB;j++)
            {
                printf("%d \t",matrice_Rezultat[i][j]);
            }
            printf("\n");
        }
    }

    pthread_exit(NULL);

	return 0;


}


