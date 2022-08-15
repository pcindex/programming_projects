#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct
{
    char name[50];
    int qty;
    float cost;
    time_t t;
} myInventory;

void logHeader()
{
    printf("\n\n\t\tMarket Inventory Log\t\t\n");
    printf("\t-------------------------------------\t\n");    
}

void logBody(long option, FILE *fileptr)
{
    char input[255];
    char *endptr;

    if(option == 1)
    {
        int numItem = 0;

        if(fileptr == NULL)
        {
            perror("System Error: Cannot Find Inventory Log");
            return;
        }

        printf("\n*** New Inventory ***\n");
        printf("Number of new items: \t");
        scanf("%d", &numItem);

        myInventory *item = malloc(sizeof(myInventory) * numItem);

        for(int i = 0; i < numItem; i++)
        {
            time_t now = time(NULL);

            printf("\nItem (%d) name: \t\t", i+1);
            getchar();
            fgets(item[i].name, sizeof(item[i].name) - 2, stdin);
            printf("Item (%d) quantity: \t", i+1);
            while(scanf("%d", &item[i].qty) != 1)
            {
                printf("--- Wrong format: Please enter whole number only: ");
            }
            printf("Item (%d) price: \t", i+1);
            scanf("%f", &item[i].cost);
            item[i].t = time(&now);

            fprintf(fileptr, "Item Name: \t\t%s", item[i].name);
            fprintf(fileptr, "Item Quantity: \t\t%d\n", item[i].qty);
            fprintf(fileptr, "Item Price: \t\t%.2f\n", trunc(item[i].cost*100.00)/100.00);
            fprintf(fileptr, "Date Log: \t\t%s\n", asctime(localtime(&item[i].t)));
        }
        free(item);
    }
    else if(option == 2)
    {
        char line[255];
        char c = 'y';        //c is assigned for getting more inventory pages

        int page = 1;

        printf("\n*** All Inventory ***\n");
        
        while(c == 'y')
        {
            printf("\nPage %d\n", page);
            printf("------------------------------------------\n");

            int count = 1;      //counting number of lines printed
            while(count <= 15)      //5 lines of information per item * 3 items per page
            {
                if(feof(fileptr))
                {
                    printf("---------- END OF INVENTORY LOG ----------\n\n\n");
                    return;
                }

                fgets(line, sizeof(line), fileptr);
                printf("%s",line);

                count++;
            }

            printf("Load next inventory page? [y,n]:\t");
            scanf("%s", &c);
            page++;

            if(c == 'n') break;

            while(c != 'y' && c != 'n')
            {
                printf("--- Please enter 'y' or 'n':\t\t");
                scanf("%s", &c);
            }
        }
    }
    else if(option == 3)
    {
        printf("\n*** Reset Inventory ***\n");
        if(remove("inventory.txt") == 0)
        {
            printf("Successful inventory reset\n");

        }else
        {
            printf("Unable to reset the inventory. Please try again\n");
        }
    }
}

void logFooter()
{
    printf("\t-------------------------------------\t\n");
    printf("\n\n\t\tSystem Terminates...\t\t\n");
}

int main()     //int argc, char *argv[]
{
    long option = 0;
    char input[255];
    char *endptr;

    FILE *fp = fopen("inventory.txt", "a+");

    logHeader();

    while(option != 4)
    {
        fseek(fp,0,SEEK_SET);

        printf("\nSystem Option:\n");

        printf("\n1. Add Inventory\n");
        printf("2. All Inventory\n");
        printf("3. Reset Inventory\n");
        printf("4. Exit\n");

        printf("\nPlease choose an option: ");
        fgets(input, sizeof(input), stdin);
        option = strtol(input, &endptr, 10);
        
        if(option == 4)
        {
            logFooter();
            break;
        }

        while(option != 1 && option != 2 && option != 3 && option != 4 )
        {
            if(option == '\n'){
                fgets(input, sizeof(input), stdin);
                option = strtol(input, &endptr, 10);    
            }
            printf("--- Please choose a valid option: ");       //FIX::after option = 2 terminates, the user input is skipped and gets within this while loop.
            fgets(input, sizeof(input), stdin);
            option = strtol(input, &endptr, 10);
        }
        logBody(option, fp);
    }
    fclose(fp);
}