
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

// #include <iostream.h>
// #include <iomanip.h>
#include <time.h>     // for seconds()

#define MaxDim  100
#define Rand_Max  100000

// Prototypes

void seedRandom(unsigned int );     
double randomn(void );
double seconds(void );
double randshannon ( void );

void gera_conj_int(int, int );


long int I_SEED = 123456789;

int Dim;
int vt[MaxDim];

// Programa Principal

int main()
{
	int v;

	double cputime;

	printf("n: ");
	scanf("%d", &Dim);

	printf("n_inst: ");
	scanf("%d", &v);

	cputime = seconds();
    gera_conj_int(Dim, v);
    printf(" CPU Time: %lf\n", seconds() - cputime);

	return(0);
}


//  Gera Conjunto de Inteiros
//
//

void gera_conj_int(int n, int n_inst)
{
	int i,j,cont;
    FILE  *p_in;
    char  file[13], j1[2], j2[2], num[8];

        snprintf(num, 8,"%d",n);
	//itoa(n, num, 10);
	printf("Num: %s\n",num);

	for(j=1; j<= n_inst; j++)
	{
      j1[0] = j / 10;
      j2[0] = j - ( 10 * j1[0] );

      j1[0] +=48;
      j2[0] +=48;
      j1[1] = '\0';
      j2[1] = '\0';


	  strcpy(file, "multp_");

      strcat(file,num);
      strcat(file,"_");
      strcat(file,j1);
      strcat(file, j2);
      strcat(file,".dat");

	  p_in = fopen(file, "wt");
	  fprintf(p_in,"%d\n", n);
      

	  cont = 0;

	  for(i=1; i<=n+1; i++)
	  {
 	    fprintf(p_in," %d",(int)(randshannon() * 50.));
		cont++;
		if(cont == 10)
		{
			cont = 0;
		    fprintf(p_in, "\n");
		}
	  }

      cont = 0;
	  fprintf(p_in, "\n\n");

	  for(i=1; i<=n+1; i++)
	  {
 	    fprintf(p_in," %d",(int)(randshannon() * 50.));
		cont++;
		if(cont == 10)
		{
			cont = 0;
		    fprintf(p_in, "\n");
		}
	  }

	  fclose(p_in);
	}
}

 
double seconds()
// cpu time in seconds since start of run.
{
  double secs;
   
  secs = (double)(clock() / 1000.0);
  return(secs);
}


// random number between 0.0 and 1.0  Shannon random number generator.
double randshannon ( void )
{
  double   random;
  long int i,j;

  I_SEED = 16807 * I_SEED;

  if (I_SEED < 0)
  {
    I_SEED += 2147483647;
  }

  i = I_SEED / 32767;
  j = I_SEED - i * 32767;

  random = j/32767.;

  /*  printf("%f\n",*random); */

  return (random);
}




