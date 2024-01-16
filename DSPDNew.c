#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

struct STOCK
{
    int id;
    float open;
    float close;
    float high;
    float low;
    int volume;
    double performance;
};

struct DAY
{
    struct STOCK stock[10];


} day[5];

int search (int pstock_number)
{
    int found = 0, i = 0;
    while ( i < 10 && found == 0)
    {
        if (pstock_number == day[1].stock[i].id)
        {
            found = 1;
        }
        else
        {
            i++;
        }
    }
    return i;
}

//first task
float percent_change ( int pstock_number, int pdate)
{  
    float change;
    change = fabs( ( (day[pdate].stock[pstock_number].open - day[pdate].stock[pstock_number].close) / day[pdate].stock[pstock_number].open ) * 100 );
    return change;
}

//first task
void print_percent_change ( int pstock_number, int pstart_date, int pend_date)
{
    for (int date = pstart_date; date <= pend_date; date++)
    {
        printf ("Day %d: %f\n", date, percent_change(pstock_number, date));
    }
}
//second task
void swap(struct STOCK swap_pstock[], int i, int j)
{
    struct STOCK temp;
    temp = swap_pstock[i];
    swap_pstock[i] = swap_pstock[j];
    swap_pstock[j] = temp;
}

//second task
void quickSort(struct STOCK pstock[], int low, int high)
{
    int i, j, pivot;
    if (low < high)
    {
        pivot = low;
        i = low;
        j = high;
        while (i < j)
        {
            while (pstock[i].volume <= pstock[pivot].volume && i < high)
                i++;
            while (pstock[j].volume > pstock[pivot].volume)
                j--;
            if (i < j)
            {
                swap(pstock, i, j);
            }
        }
        swap(pstock, pivot, j);
        quickSort(pstock, low, j - 1);
        quickSort(pstock, j + 1, high);
    }
}

//second task
void quickSortVolume(struct STOCK pstock[], int low, int high)
{
    int i, j, pivot;
    if (low < high)
    {
        pivot = low;
        i = low;
        j = high;
        while (i < j)
        {
            while (pstock[i].close <= pstock[pivot].close && i < high)
                i++;
            while (pstock[j].close > pstock[pivot].close)
                j--;
            if (i < j)
            {
                swap(pstock, i, j);
            }
        }
        swap(pstock, pivot, j);
        quickSortVolume(pstock, low, j - 1);
        quickSortVolume(pstock, j + 1, high);
    }
}

//second task
void sort_volume_close( int pdate)
{
    quickSort(day[pdate].stock, 0, 9);
    int i = 0, j;
    while (i < 10)
    {
        j = i + 1;
        while (j < 5 && day[pdate].stock[j].volume == day[pdate].stock[i].volume) j++;
        quickSortVolume(day[pdate].stock, i, j - 1);
        i = j;
    }
}

//third task
float profit_loss( struct STOCK pstock_start, struct STOCK pstock_end)
{
    float gain = (pstock_end.high - pstock_start.low) ;
    return gain;
}

//forth task
float highest_performing_stock (struct DAY pday_start, struct DAY pday_end )
{
    int largest = 0;
    float largest_profit = profit_loss (pday_start.stock[0], pday_end.stock[0] ), gain_i;
    for (int i = 1; i < 10 ; i++)
    {
        gain_i = profit_loss (pday_start.stock[i], pday_end.stock[i] );
        if ( gain_i > largest_profit )
        {
            largest_profit = gain_i;
            largest = i;
        }
    }

    if (largest_profit < 0)
    {
        largest = -1;
    }

    return largest;
}

//fifth task
float lowest_performing_stock (struct DAY pday_start, struct DAY pday_end )
{
    int lowest = 0;
    float lowest_profit = profit_loss ( pday_start.stock[0], pday_end.stock[0] ), gain_i;
    for (int i = 1; i < 10 ; i++)
    {
        gain_i = profit_loss ( pday_start.stock[i], pday_end.stock[i] );
        if ( gain_i < lowest_profit && gain_i )
        {
            lowest_profit = gain_i;
            lowest = i;
        }
    }


    if ( lowest_profit < 0)
    {
        lowest = -1;
    }
    return lowest;
}

//sixth task
double overall_performance_of_one_stock ( int pstart_date, int pend_date, int pstock_number )
{
    double o_p = 0;
    if( pend_date == 0 )//invalid
    {
        o_p = -1;
    }
    else
    {  
        if( pstart_date == 0 )
        {
            pstart_date++;
        }
        for ( int date = pstart_date ; date <= pend_date; date++ )
        {
            o_p = o_p + ( fabs(day[date].stock[pstock_number].close - day[date-1].stock[pstock_number].close) * day[date].stock[pstock_number].volume * 100 )/ day[date-1].stock[pstock_number].close;
        }
    }  
    printf ("%f\n", o_p);
    return o_p;
}

//sixth task
void overall_performance( int pstart_date, int pend_date )
{
    for (int i = 0; i < 10; i++)
    {
        day[pstart_date].stock[i].performance = overall_performance_of_one_stock(pstart_date, pend_date, i);
        //printf ("%lf\n",day[pstart_date].stock[i].performance );
    }
}

//sixth task
void sort_overall_performance(struct STOCK pstock[], int low, int high)
{
    int i, j, pivot;
    if (low < high)
    {
        pivot = low;
        i = low;
        j = high;
        while (i < j)
        {
            while (pstock[i].performance <= pstock[pivot].performance && i < high)
                i++;
            while (pstock[j].performance > pstock[pivot].performance)
                j--;
            if (i < j)
            {
                swap(pstock, i, j);
            }
        }
        swap(pstock, pivot, j);
        sort_overall_performance(pstock, low, j - 1);
        sort_overall_performance(pstock, j + 1, high);
    }
}

//seventh task
float average ( struct DAY pday[], int pstart_date, int pend_date )
{
    int i_day, i_stock;
    float sum, all_stock_sum = 0, all_stock_avg, days;
    days = pend_date - pstart_date + 1;
    for ( i_stock = 0; i_stock < 10; i_stock++ )
    {
        sum  = 0;
        for ( i_day = pstart_date; i_day <= pend_date; i_day++ )
        {
            sum = sum + ( pday[i_day].stock[i_stock].open * pday[i_day].stock[i_stock].volume );
        }
        all_stock_sum = all_stock_sum + sum;
    }
    all_stock_avg = all_stock_sum / (10 * days);
    return all_stock_avg;
}

int main()
{
    int start_date, end_date, stock_number = 2, date, task = 0, START_DATE = 20, END_DATE = 24 ;
    float task4, task5, task3, task7;

    int maxDays = 5; // days

    for (int fileIndex = 0; fileIndex < maxDays; fileIndex++) 
    {
        char filename[20];
        snprintf(filename, sizeof(filename), "day%d.csv", fileIndex + 1);
        FILE *file = fopen(filename, "r");

        if (file == NULL) {
            printf("Failed to open file: %s\n", filename);
            return 1;
        }

        char header[256];
        if (fgets(header, sizeof(header), file) == NULL) 
        {
            printf("Error reading header from file %s\n", filename);
            fclose(file);
            return 1;
        }

        int recordIndex = 0;
        char line[256]; 
        while (fgets(line, sizeof(line), file) != NULL && recordIndex <10)
        {

            int read = sscanf(line, "%d,%f,%f,%f,%f,%d,%lf\n", &day[fileIndex].stock[recordIndex].id,  &day[fileIndex].stock[recordIndex].open,
                                    &day[fileIndex].stock[recordIndex].close, &day[fileIndex].stock[recordIndex].high, 
                                    &day[fileIndex].stock[recordIndex].low, &day[fileIndex].stock[recordIndex].volume, &day[fileIndex].stock[recordIndex].performance);
                                

            if (read>=-1) 
            {
                recordIndex++; // Data read successfully
            } 
            else 
            {
                printf("%d\n",read);
                printf("%d\n",recordIndex);
                printf("Error reading data from file %s\n", filename);
                fclose(file);
                return 1;
            }
            
        }

        fclose(file);
    }


    printf ("\n");
    printf ("\033[0;33m");
    printf ("Welcome to our application\n\n");
    printf ("\033[0;34m");
    printf ("Some general instructions\n");
    printf ("\033[0;37m");

    while (task != 8 )
    {
        printf ("\n\nEnter :\n");
        printf ("\"1\" To find percentage change in the price of a stock from start to end date. \n");
        printf ("\"2\" To sort a stock according to their volumes, and then closing prices \n");
        printf ("\"3\" To find profit or loss. \n");
        printf ("\"4\" To find the highest performing stock. \n");
        printf ("\"5\" To find the lowest performing stock. \n");
        printf ("\"6\" To list all stock as per increasing the overall performance. \n");
        printf ("\"7\" To find the average value of all stocks. \n");
        printf ("\"8\" To exit\n");

        scanf ("%d", &task);

        while ( !(task > 0 && task < 9))
        {
            printf ("Invalid Input, re-enter the task number: ");
            scanf ("%d", &task);
        }

        if (task == 1)
        {
            printf ("Enter start date, end date: \n");
            scanf ("%d %d", &start_date, &end_date);
            while (end_date > END_DATE || start_date < START_DATE || end_date < start_date)
            {
                printf ("Information not available, please enter a date between %d and %d", START_DATE, END_DATE);
                scanf ("%d %d", &start_date, &end_date);
            }
            printf ("Enter stock number: \n");
            scanf ("%d", &stock_number);
            while (stock_number > 109 || stock_number < 100)
            {
                printf ("Invalid stock number, re-enter the stock number: \n");
                scanf ("%d", &stock_number);
            }
            stock_number = search (stock_number);
            end_date = end_date - START_DATE;
            start_date = start_date - START_DATE;
            print_percent_change( stock_number, start_date, end_date );
        }
        else if (task == 2)
        {
            printf ("Enter the date\n");
            scanf ("%d", &date);
            while (date > END_DATE || date < START_DATE)
            {
                printf ("Information not available, please enter a date between %d and %d: \n", START_DATE, END_DATE);
                scanf ("%d", &date);
            }
            date = date - START_DATE;
            sort_volume_close(date);
            for (int i = 0; i < 10; i++)
            {
                printf ("\nName %d, Volume %d, close %f", day[date].stock[i].id, day[date].stock[i].volume, day[date].stock[i].close );
            }
        }
        else if (task == 3)
        {
            printf ("Enter start date, end date: \n");
            scanf ("%d %d", &start_date, &end_date);
            while (end_date > END_DATE || start_date < START_DATE || end_date < start_date)
            {
                printf ("Information not available, please enter a date between %d and %d: \n", START_DATE, END_DATE);
                scanf ("%d %d", &start_date, &end_date);
            }
            end_date = end_date - START_DATE;
            start_date = start_date - START_DATE;
            printf ("Enter stock number: \n");
            scanf ("%d", &stock_number);
            while (stock_number > 109 || stock_number < 100)
            {
                printf ("Invalid stock number, re-enter the stock number");
                scanf ("%d", &stock_number);
            }
            stock_number = search (stock_number);
            task3 = profit_loss( day[start_date].stock[stock_number], day[end_date].stock[stock_number] );
            if (task3 >= 0)
            {
                printf ("\nProfit is %f", task3);
            }
            else
            {
                printf ("\nLoss is %f", (task3*(-1)) );
            }
        }
        else if (task == 4)
        {
            printf ("Enter start date, end date: \n");
            scanf ("%d %d", &start_date, &end_date);
            while (end_date > END_DATE || start_date < START_DATE || end_date < start_date)
            {
                printf ("Information not available, please enter a date between %d and %d: ", START_DATE, END_DATE);
                scanf ("%d %d", &start_date, &end_date);
            }
            end_date = end_date - START_DATE;
            start_date = start_date - START_DATE;
            task4 = highest_performing_stock( day[start_date], day[end_date] );
            if (task4 == -1)
            {
                printf ("\nAll stocks are yielding loss.");
            }
            else
            {
                printf ("\nHighest performing task is %d.", task4);
            }
        }
        else if (task == 5)
        {
            printf ("Enter start date, end date: \n");
            scanf ("%d %d", &start_date, &end_date);
            while (end_date > END_DATE || start_date < START_DATE || end_date < start_date)
            {
                printf ("\nInformation not available, please enter a date between %d and %d: ", START_DATE, END_DATE);
                scanf ("%d %d", &start_date, &end_date);
            }
            end_date = end_date - START_DATE;
            start_date = start_date - START_DATE;
            task5 = lowest_performing_stock( day[start_date], day[end_date] );
            printf ("\nLowest performing task is %d.", task5);
        }
        else if (task == 6)
        {
            printf ("Enter start date, end date: \n");
            scanf ("%d %d", &start_date, &end_date);
            while (end_date > END_DATE || start_date < START_DATE || end_date < start_date)
            {
                printf ("\nInformation not available, please enter a date between %d and %d: ", START_DATE, END_DATE);
                scanf ("%d %d", &start_date, &end_date);
            }
            end_date = end_date - START_DATE;
            start_date = start_date - START_DATE;
            overall_performance( start_date, end_date );
            sort_overall_performance(day[start_date].stock, 0, 9);
            for (int i = 0; i < 10; i++)
            {
                printf ("\nID: %d, Overall Performance: %lf", day[start_date].stock[i].id, day[start_date].stock[i].performance);
            }
        }
        else if (task == 7)
        {
            printf ("Enter start date, end date: \n");
            scanf ("%d %d", &start_date, &end_date);
            while (end_date > END_DATE || start_date < START_DATE || end_date < start_date)
            {
                printf ("Information not available, please enter a date between %d and %d", START_DATE, END_DATE);
                scanf ("%d %d", &start_date, &end_date);
            }
            end_date = end_date - START_DATE;
            start_date = start_date - START_DATE;
            task7 = average(day, start_date, end_date);
            printf ("Average is %f", task7);
        }
    }

    return 0;
}