#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int numberItens_ = -1;
int knapsackSize_ = -1;

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
    int heuristic;
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


void 
detailItens (struct item *a, int length)
{
    printf("%d %d %d\n",numberItens_,knapsackSize_,length);

    //heapsortId(a,length,true);
    int totalWeight = 0;
    int totalProfit = 0;

    for(int i = 0; i < length; i++){
        printf("Line %d: ", i + 2);
        printf("Id: %d ; ", a[i].id);
        printf("Value: %d ; ", a[i].value);
        printf("Weight: %d ; ", a[i].weight);
        
        printf("\n \n");

        totalWeight += a[i].weight;
        totalProfit += a[i].value;
    }
    printf("Total de objetos: %d; Peso total: %d; Lucro total: %d\n",length,totalWeight,totalProfit);
}

void 
showItens (struct knapsack *knapsack, FILE *file) /// O(n)
{
    printf ("%d %d %f\n",knapsack->numberItens,knapsack->weightItens,knapsack->valueItens);

    for (int i=0; i < knapsack->numberItens; i++)
    {
        printf ("%d %f\n", knapsack->itens[i].id, knapsack->itens[i].fractionItem);
    }

    fprintf (file,"%d %d %f\n",knapsack->numberItens,knapsack->weightItens,knapsack->valueItens);

    for (int i=0; i < knapsack->numberItens; i++) 
    {
        fprintf(file,"%d %f\n", knapsack->itens[i].id, knapsack->itens[i].fractionItem);
    }
}

struct knapsack 
*greedyKnapsackFractional (struct item *item,int length,int max) /// O(nlog n)
{
    heapsortRate (item,length,false);  /// O(nlog n)

    float valueItens = 0;
    int weightItens = 0;
    int count = 0;

    struct knapsack *knapsack = (struct knapsack *) malloc (sizeof (struct knapsack)); //Vetor de incidencia de conflitos;
    knapsack->itens = (struct item *) malloc(sizeof(struct item));

    for(int i = 0; i < length; i++){ /// O(n)
        if(item[i].weight > max){
            knapsack->itens = (struct item *) realloc(knapsack->itens,sizeof(struct item)*(count+1));
            knapsack->itens[count].id = item[i].id;

            valueItens += (float)item[i].value * max / item[i].weight;
            weightItens += max;

            count++;

            break;
        }

        knapsack->itens = (struct item *) realloc(knapsack->itens,sizeof(struct item)*(count+1));
        knapsack->itens[count].id = item[i].id;

        valueItens += (float)item[i].value;
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
*loadItens (FILE *file) ///O(n²)
{
    char line[3000];
    char *result;
    char *pch;

    int i = 0;
    int j = 0;

    result = fgets(line, 3000, file); //Pegando a primeira linha
    sscanf(result, "%d", &numberItens_);

    struct item *itens = (struct item *) malloc (sizeof (struct item) * numberItens_); //Vetor de itens

    while (!feof (file)){ ///O(n²)
        result = fgets(line, 3000, file); //Pegando a próxima linha.
        pch = strtok(result," ");
        
        i++;

        while (pch != NULL){ /*Iterando na linha.*/ ///O(m+3)
            if (strpbrk(pch, "0123456789") == NULL){ //Verifica se a string possui algum número
//                printf("String: %s\n",pch);
            }
            else if ( i > numberItens_ )
            {
              knapsackSize_ = atoi (pch);
            }
            else 
            {
                switch (j) {
                case 0: //Primeira string lida na linha
                    itens[i].id = atoi(pch); //Atribui o índice do item
                    break;
                case 1: //Segunda string lida na linha
                    itens[i].value = atoi(pch); //Atribui o valor do item
                    break;
                case 2: //Terceira string lida na linha
                    itens[i].weight = atoi(pch); //Atribui o peso do item
                    itens[i].rate = (float)itens[i].value/itens[i].weight; //Atribui a razão entre o valor e peso do item.
                    break;
                default: //Outras strings lida na linha
                    break;
                }

                j++;
            }
            pch = strtok(NULL," ");
        }        
    }

    detailItens(itens,numberItens_);

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

    FILE *fileIn = fopen (argv[1], "r"); //Arquivo de entrada
    if (fileIn == NULL)
    {
        printf ("Erro, nao foi possivel abrir o arquivo de entrada\n");
        return 0;
    }

    struct item *itens = loadItens (fileIn); /*Carrega os valores da instância.*/ /// O(n²)

    fclose (fileIn);

    FILE *fileOut = fopen ("Output.txt", "w"); //Arquivo de saída
    if (fileOut == NULL)
    {
        printf ("Erro, nao foi possivel criar o arquivo de saída\n");
        abort ();
    }

    showItens ( greedyKnapsackFractional (itens,numberItens_,knapsackSize_),fileOut ); /// O(nlog n)

    fclose (fileOut);

    return 0;
}
