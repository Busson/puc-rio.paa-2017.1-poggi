#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "cpu_timer/CPUTimer.h"

static CPUTimer totalTimer_;
static int numberItens_ = -1;
static int knapsackSize_ = -1;
static bool fractionItem_ = false;

struct item
{
    unsigned int id;
    int value;
    int weight;    
    float rate; //rate between value and weight.
    float fractionItem;
};

struct knapsack
{
    int numberItens;
    int weightItens;
    float valueItens;
    struct item *itens;    
};

void 
heapifyRateMin (struct item *a, int len, int index) /// O(log n)
{
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    struct item aux;

    if (left > len - 1) 
    {
        return;
    }
    else if (right == len)
    {
        if (a[index].rate > a[left].rate)
        {   //swap left with parent
            aux = a[index];
            a[index] = a[left];
            a[left] = aux;
        }
        return;
    }
    else if (a[index].rate > a[left].rate || a[index].rate > a[right].rate) 
    {
        if (a[left].rate > a[right].rate) 
        {   //swap right with parent
            aux = a[index];
            a[index] = a[right];
            a[right] = aux;
            heapifyRateMin (a,len,right);
        }
        else
        {   //swap left with parent
            aux = a[index];
            a[index] = a[left];
            a[left] = aux;
            heapifyRateMin (a,len,left);
        }
    }
}

void 
heapifyRateMax (struct item *a, int len,  int index) /// O(log n)
{
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    struct item aux;

    if (left > len-1) 
    {
        return;
    }
    else if (right==len){
        if (a[index].rate < a[left].rate)
        {   //swap left with parent
            aux = a[index];
            a[index] = a[left];
            a[left] = aux;
        }
        return;
    }
    else if (a[index].rate < a[left].rate || a[index].rate < a[right].rate) 
    {
        if (a[left].rate < a[right].rate) 
        {   //swap right with parent
            aux = a[index];
            a[index] = a[right];
            a[right] = aux;
            heapifyRateMax (a,len,right);
        }
        else
        {   //swap left with parent
            aux = a[index];
            a[index] = a[left];
            a[left] = aux;
            heapifyRateMax (a,len,left);
        }
    }
}

void 
buildHeapRate (struct item *a, int length, bool asc) /// O(n)
{
    int i;

    if (asc)
    {
        for (i = length - 1; i >= 0; i--) 
        {
            if( 2 * i + 1 > length - 1)
                continue;

            heapifyRateMax (a,length,i);
        }
    }
    else 
    {
        for (i = length - 1; i>= 0; i--) 
        {
            if(2 * i  + 1 > length - 1)
                continue;

            heapifyRateMin (a,length,i);
        }
    }

    return;
}

void 
heapsortRate (struct item *list, int length, bool asc) /// O(nlog n)
{
    struct item aux;
    int cunrrentLength = length;

    buildHeapRate (list,length,asc);

    if (asc)
    { //Ordem crescente.
        while (cunrrentLength > 1)
        {   //swap first with last
            aux = list[0];
            list[0] = list[cunrrentLength - 1];
            list[cunrrentLength - 1] = aux;
            cunrrentLength--;

            //heapify new heap
            heapifyRateMax (list,cunrrentLength,0);
        }

    }
    else 
    { //Ordem decrescente.
        while (cunrrentLength > 1)
        {   //swap first with last
            aux = list[0];
            list[0] = list[cunrrentLength - 1];
            list[cunrrentLength-1] = aux;
            cunrrentLength--;

            //heapify new heap
            heapifyRateMin (list,cunrrentLength,0);
        }
    }
    return;
}

int
median (int left, int right, struct item *item)
{
  struct item aux; 

  for (int i = left; i <= right; i++) 
  {
    for (int j = left; j <= right; j++) 
    {
      if (item[i].rate > item[j].rate)
      {
        aux = item[i];
        item[i] = item[j];
        item[j] = aux;
      }
    }
  }

  int middle = left + ((right - left) / 2);
  return middle;
}

int 
medianOfMedians (struct item *item, int start, int end)
{
  if(end - start < 5)
  {
    return median (start, end, item);
  }

  struct item aux;

  for (int i = 0; i < (end + 1)/5; i++)
  {
    int left = 5 * i;
    int right = left + 4;

    if (right > end){
      right = end;
    }

    int index  = median (left, right, item);
    
    aux = item[index];
    item[index] = item[i];
    item[i] = aux;
  }

  return medianOfMedians (item, 0, end/5);
}

int
partition (struct item *item, int left, int right)
{
  int n = right + 1 - left;
  struct item *auxItem = (struct item *) malloc (sizeof (struct item) * n);
  int auxIndex[n];
  
  for (int i = 0; i < n; i++)
  {
    auxItem[i].id = item[left+i].id;
    auxItem[i].value = item[left+i].value;
    auxItem[i].weight = item[left+i].weight;
    auxItem[i].rate = item[left+i].rate;
    auxItem[i].fractionItem = item[left+i].fractionItem;

    auxIndex[i] = left + i;
  }

  int indexMedian = medianOfMedians (auxItem, 0, n-1);
  int median = auxIndex[indexMedian];

  float pivot = item[median].rate;
  int posPivot = median;
  int i = left;
  int j = right;
  struct item aux;

  while (1)
  {
    for(; item[i].rate >= pivot && i <= right; i++);
    for(; item[j].rate < pivot && j >= left; j--);
    if (i < j)
    {
      if (item[j].rate == pivot)
      {
        posPivot = i;
      }
      aux = item[i];
      item[i] = item[j];
      item[j] = aux;
    }
    else
    {
      aux = item[posPivot];
      item[posPivot] = item[j];
      item[j] = aux;
      return j;
    }
  }
}

int
kesimo (struct item *item, int left, int right, float usedWeight)
{
  int middle = partition (item, left, right); //Finding the middle item.

  float sum = usedWeight;
  for (int i = left; i <= middle; i++)
  {
    sum += item[i].weight;
  }

  if (sum == knapsackSize_ || middle >= numberItens_ -1)
  {
    return middle;
  }
  else if ( (sum > knapsackSize_) && (sum - item[middle].weight <= knapsackSize_) )
  {
    return middle;
  }
  else if (sum > knapsackSize_)
  {
    return kesimo (item, left, middle - 1, usedWeight);
  }
  else
  {
    return kesimo (item, middle+1, right, sum);
  }
}

void 
detailItens (struct item *a, int length)
{
    printf ("Total de Objetos: %d; Capacidade da Mochila: %d\n", numberItens_, knapsackSize_);

    int totalWeight = 0;
    int totalProfit = 0;

    for (int i = 0; i < length; i++)
    {
        printf ("Line %d: ", i + 2);
        printf ("Id: %d ; ", a[i].id);
        printf ("Value: %d ; ", a[i].value);
        printf ("Weight: %d ; ", a[i].weight);
        //printf ("Fraction: %f ;", a[i].fractionItem);
        
        printf ("\n");

        totalWeight += a[i].weight;
        totalProfit += a[i].value;
    }

    printf ("\nTotal de objetos: %d; Peso total: %d; Lucro total: %d\n\n",length,totalWeight,totalProfit);
}

void 
showItens (struct knapsack *knapsack, FILE *file) /// O(n)
{
    printf ("\nQuantidade de itens (N): %d\n", numberItens_);

    /*printf ("Quantidade de itens na mochila: %d\nPeso Total dos Itens: %d\nValor total dos Itens: %f\n\n", knapsack->numberItens, knapsack->weightItens, knapsack->valueItens);

    for (int i = 0; i < knapsack->numberItens; i++) //Print Itens 
    {
        printf ("Id: %d; Quantidade: %f %%;\n", knapsack->itens[i].id, knapsack->itens[i].fractionItem*100);
    }*/

    //fprintf (file,"Quantidade de itens (N): %d\n", numberItens_);
    //fprintf (file,"Quantidade de itens na mochila: %d\nPeso Total dos Itens: %d\nValor total dos Itens: %f\n\n", knapsack->numberItens, knapsack->weightItens, knapsack->valueItens);

    /*for (int i = 0; i < knapsack->numberItens; i++) 
    {
        fprintf(file,"Id: %d; Quantidade: %f %%;\n", knapsack->itens[i].id, knapsack->itens[i].fractionItem*100);
    }

    if(fractionItem_){
      printf ("\nHá um item fracionado.\n");
      fprintf(file,"\nHá um item fracionado.\n");
      return;
    }

    printf ("\nTodos os itens estão inteiros.\n");
    fprintf(file,"\nTodos os itens estão inteiros.\n");*/
}

struct knapsack 
*greedyKnapsackFractional (struct item *item, int length, int max) /// O(nlog n)
{
    heapsortRate (item,length,false);  /// O(nlog n)
    //detailItens (item, numberItens_);
    float valueItens = 0;
    int weightItens = 0;
    int count = 0;

    struct knapsack *knapsack = (struct knapsack *) malloc (sizeof (struct knapsack)); 
    knapsack->itens = (struct item *) malloc(sizeof(struct item));

    for(int i = 0; i < length; i++){ /// O(n)
        if(item[i].weight > max){
            knapsack->itens = (struct item *) realloc (knapsack->itens, sizeof (struct item) * (count+1));
            knapsack->itens[count].id = item[i].id;
            knapsack->itens[count].fractionItem = (float) max / item[i].weight;

            valueItens += (float) item[i].value * max / item[i].weight;
            weightItens += max;

            count++;
            fractionItem_ = true;
            break;
        }

        knapsack->itens = (struct item *) realloc ( knapsack->itens, sizeof (struct item) * (count+1));
        knapsack->itens[count].id = item[i].id;
        knapsack->itens[count].fractionItem = item[i].fractionItem;

        valueItens += (float) item[i].value;
        weightItens += item[i].weight;

        count++;

        max = max - item[i].weight;

        if (max == 0)
            break;
    }

    knapsack->numberItens = count;
    knapsack->valueItens = valueItens;
    knapsack->weightItens = weightItens;

    return knapsack;
}

struct knapsack 
*linearKnapsackFractional (struct item *item, int length, int max) /// O(n)
{
    kesimo (item, 0, numberItens_ - 1, 0);
    //detailItens (item, numberItens_);
    float valueItens = 0;
    int weightItens = 0;
    int count = 0;

    struct knapsack *knapsack = (struct knapsack *) malloc (sizeof (struct knapsack)); 
    knapsack->itens = (struct item *) malloc(sizeof(struct item));

    for(int i = 0; i < length; i++){ /// O(n)
        if(item[i].weight > max){
            knapsack->itens = (struct item *) realloc (knapsack->itens, sizeof (struct item) * (count+1));
            knapsack->itens[count].id = item[i].id;
            knapsack->itens[count].fractionItem = (float) max / item[i].weight;

            valueItens += (float) item[i].value * max / item[i].weight;
            weightItens += max;

            count++;
            fractionItem_ = true;
            break;
        }

        knapsack->itens = (struct item *) realloc ( knapsack->itens, sizeof (struct item) * (count+1));
        knapsack->itens[count].id = item[i].id;
        knapsack->itens[count].fractionItem = item[i].fractionItem;

        valueItens += (float) item[i].value;
        weightItens += item[i].weight;

        count++;

        max = max - item[i].weight;

        if (max == 0)
            break;
    }

    knapsack->numberItens = count;
    knapsack->valueItens = valueItens;
    knapsack->weightItens = weightItens;

    return knapsack;
}

struct item 
*loadItens (FILE *file) ///O(n)
{
    char line[1000];
    char *result;
    char *pch;

    int i = 0;
    int j = 0;

    result = fgets(line, 1000, file); //Get the first line.
    sscanf(result, "%d", &numberItens_);

    struct item *itens = (struct item *) malloc (sizeof (struct item) * numberItens_); //Instantiate itens struct.
    
    while (!feof (file)) ///O(n)
    { 
        result = fgets(line, 1000, file); //Get the next line.
        pch = strtok(result," ");
        
        j = 0;

        while (pch != NULL) //Get the next string.
        {
            if (strpbrk(pch, "0123456789") == NULL) //Checking whether the string contains some number.
            { 
//                printf("String: %s\n",pch);
            }
            else if ( i >= numberItens_ ) //Get the last line.
            {
              knapsackSize_ = atoi (pch); //Assign capacity to Knapsack.
            }
            else 
            {
                switch (j) 
                {
                  case 0: //First string in the line.
                    itens[i].id = atoi(pch); //Assign id to item.
                    itens[i].fractionItem = 1;
                    break;
                  case 1: //Second string in the line.
                    itens[i].value = atoi(pch); //Assign value to item.
                    break;
                  case 2: //Third string in the line.
                    itens[i].weight = atoi(pch); //Assign weight to item.
                    itens[i].rate = (float)itens[i].value/itens[i].weight; //Assign rate (value/weight) to item.
                    break;
                  default: 
                      break;
                }

                j++;
            }
            pch = strtok (NULL," ");
        }        
        
        i++;
    }

    //detailItens (itens, numberItens_);

    return itens;
}

int 
main (int argc, char **argv)
{
    if (argc == 1)
    {
        printf ("Programa sem parametros\n");
        return 0;
    }

    FILE *fileIn = fopen (argv[1], "r"); // Input file.
    if (fileIn == NULL)
    {
        printf ("Erro, nao foi possivel abrir o arquivo de entrada\n");
        return 0;
    }

    struct item *itens = loadItens (fileIn); // Load instances. / O(n²)

    fclose (fileIn);
 
    FILE *fileOut = fopen ("Output.txt", "w"); // Output file.
    if (fileOut == NULL)
    {
        printf ("Erro, nao foi possivel criar o arquivo de saída\n");
        abort ();
    }

    int select = atoi (argv[2]);    

    struct knapsack *knapsackItens;
    
    guint32 k = 1;
    totalTimer_.reset();

    switch ( select ) // Select question.
    {
      case 1: // O(nlog n)
        while (totalTimer_.getCPUTotalSecs () < 5)
        {
          totalTimer_.start();
          knapsackItens = greedyKnapsackFractional (itens,numberItens_,knapsackSize_);
          totalTimer_.stop();
          k++;
        }
        break;

      case 2: // O(n)
        while (totalTimer_.getCPUTotalSecs () < 5)
        {
          totalTimer_.start();
          knapsackItens = linearKnapsackFractional (itens,numberItens_,knapsackSize_);
          totalTimer_.stop();
          k++;
        }        
        break;

      case 3: 
        //while (totalTimer_.getCPUTotalSecs () < 5)
        //{
          //totalTimer_.start();
          //knapsackItens = pivotKnapsackFractional (itens,numberItens_,knapsackSize_);
          //totalTimer_.stop();
          //k++;
        //}        
        break;

      default:
        printf ("Questao invalida\n");
        break;
    }

    showItens (knapsackItens,fileOut);

    fclose (fileOut);
    
    printf("Time: %f\nk=%d total: %lf\n\n", totalTimer_.getCPUTotalSecs()/k, k, totalTimer_.getCPUTotalSecs() );

    return 0;
}
