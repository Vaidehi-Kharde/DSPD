#include <stdio.h>
#include <stdlib.h>

typedef struct Time
{
    int Hour;
    int Min;
    int Sec;
} Time;

typedef struct Flight_tag
{
    int flightID;
    Time departureTime;
    Time ETA;
    struct Flight_tag *next;
} Flight;

typedef struct Bucket_tag
{
    Flight *flights;
    int BucketID;
    Time beginningETA;
    Time endETA;
    struct Bucket_tag *next;
} Bucket;

int BucketId = 1000;

Flight *createNode(int ID, Time departureTime, Time ETA)
{
    Flight *nptr;
    nptr = (Flight *)malloc(sizeof(Flight));

    nptr->flightID = ID;
    nptr->next = NULL;

    nptr->departureTime.Hour = departureTime.Hour;
    nptr->departureTime.Min = departureTime.Min;
    nptr->departureTime.Sec = departureTime.Sec;

    nptr->ETA.Hour = ETA.Hour;
    nptr->ETA.Min = ETA.Min;
    nptr->ETA.Sec = ETA.Sec;

    return nptr;
}

void addTime(Time *t, int mins)
{
    t->Min = t->Min + mins;
    if (t->Min > 59)
    {
        t->Min = t->Min - 60;
        t->Hour++;
    }
    if (t->Hour > 23)
    {
        t->Min = 59;
        t->Hour = 23;
    }
}

int maxTime(Time t1, Time t2) // ret_val=1 indicates t1 < t2 and 0 indicates t1 = t2 and -1 indicates t1 > t2
{
    int ret_val = 1;
    if (t1.Hour > t2.Hour)
        ret_val = -1;
    else if (t1.Hour < t2.Hour)
        ret_val = 1;
    else // same hour
    {
        if (t1.Min > t2.Min)
            ret_val = -1;
        else if (t1.Min < t2.Min)
            ret_val = 1;
        else // same min
        {
            if (t1.Sec > t2.Sec)
                ret_val = -1;
            else if (t1.Sec < t2.Sec)
                ret_val = 1;
            else
                ret_val = 0; // same second
        }
    }
    return ret_val;
}

void showBucketList(Bucket *head)
{
    while (head != NULL)
    {
        printf("Bucket Id is %d\n", head->BucketID);
        printf("Bucket begin ETA = %d:%d:%d\n", head->beginningETA.Hour, head->beginningETA.Min, head->beginningETA.Sec);
        printf("Bucket end ETA = %d:%d:%d\n", head->endETA.Hour, head->endETA.Min, head->endETA.Sec);
        printf("This Bucket contains following flightIDs :\n");
        Flight *temp;
        temp = head->flights;

        while (temp != NULL)
        {
            printf("Id: %d ", temp->flightID);
            printf("Departure Time: Hour: %d ", temp->departureTime.Hour);
            printf(" Min: %d ", temp->departureTime.Min);
            printf("ETA: Hour: %d", temp->ETA.Hour);
            printf(" Min %d", temp->ETA.Min);
            printf("\n");
            temp = temp->next;
        }
        printf("\n\n");
        head = head->next;
    }
}

void checkStatus(Bucket *head)
{
    int ID;

    printf("Enter the ID of Flight for checking status : ");
    scanf("%d", &ID);

    int found = 0;

    while (!found && head != NULL)
    {
        Flight *ptr;
        ptr = head->flights;

        while (ptr != NULL && !found)
        {
            if (ptr->flightID == ID)
            {
                found = 1;
                printf("\nFlight is expected to arrive at %d:%d:%d\n", ptr->ETA.Hour, ptr->ETA.Min, ptr->ETA.Sec);
            }
            ptr = ptr->next;
        }
        head = head->next;
    }
    if (!found)
        printf("\nNo such Flight\n");
}

void removeFromFile(int ID)
{
    FILE*fp1,*fp2; 
    fp1 = fopen("Data.csv","r");
    fp2 = fopen("Dummy.txt","w");
    char c[20];

    while(!feof(fp1))
    {
        fgets(c,5,fp1); //Here 4 is the length of ID (4 + 1 for \0), maybe we can use a macro for that
        int fileData;
        fileData = atoi(c);

        char ch = '\0';

        if(fileData!=ID && !feof(fp1))
        {
            fprintf(fp2,"%s",c);
            
            while(ch!='\n' && ch!=EOF)
            {
                ch = fgetc(fp1);
                fprintf(fp2,"%c",ch);
            }
        }
        else while(ch!='\n' && ch!=EOF) ch = fgetc(fp1);
    }
    fclose(fp1);
    fclose(fp2);

    //remove("Data.csv"); 
    rename("Dummy.txt","updatedData.csv");
}

Bucket *cancelFlight(Bucket *head)
{
    int ID;

    printf("Enter the ID of Flight that is to be cancelled : ");
    scanf("%d", &ID);

    int found = 0;
    Bucket *bucketPtr, *prevBucketPtr;
    bucketPtr = prevBucketPtr = head;

    while (!found && bucketPtr != NULL)
    {
        Flight *ptr, *prev;
        ptr = bucketPtr->flights;
        prev = ptr;

        while (ptr != NULL && !found)
        {
            if (ptr->flightID == ID)
            {
                found = 1;
                printf("\nFlight that was expected to arrive at %d:%d:%d is CANCELLED\n", ptr->ETA.Hour, ptr->ETA.Min, ptr->ETA.Sec);
                if (ptr == bucketPtr->flights)//first flight in the bucket
                    bucketPtr->flights = bucketPtr->flights->next;
                else
                    prev->next = ptr->next;
                free(ptr);
                //if the only flight in th bucket is deleted
                if (bucketPtr->flights == NULL && bucketPtr != head)
                {
                    prevBucketPtr->next = bucketPtr->next;
                    free(bucketPtr);
                }
            }
            else
            {
                prev = ptr;
                ptr = ptr->next;
            }
        }
        prevBucketPtr = bucketPtr;
        bucketPtr = bucketPtr->next;
    }
    if (head->flights == NULL)
    {
        bucketPtr = head;
        head = head->next;
        free(bucketPtr);
    }
    if (!found)
        printf("\nNo such Flight\n");
    removeFromFile(ID);
    return head;
}

void rearrangeBuckets(Bucket *firstBucket)
{
    Time t;
    t.Hour = 6;
    t.Min = 15;
    t.Sec = 0;

    Bucket *lptr = firstBucket, *prevlptr = firstBucket;
    Flight *flightPtr, *prevFlight;

    // (removing the "first buckets")
    while (lptr != NULL && maxTime(lptr->endETA, t) == 1)
    {
        prevFlight = flightPtr = lptr->flights;
        while (flightPtr != NULL)
        {
            prevFlight = flightPtr;
            flightPtr = flightPtr->next;
            free(prevFlight);
        }
        prevlptr = lptr;
        lptr = lptr->next;
        free(prevlptr);
    }
    firstBucket = lptr;

    // update time
    while (lptr != NULL)
    {
        addTime(&lptr->beginningETA, t.Min);
        addTime(&lptr->endETA, t.Min);
        lptr = lptr->next;
    }

    // removing unnecessary flights from the first bucket
    flightPtr = prevFlight = firstBucket->flights;
    while (flightPtr != NULL && maxTime(flightPtr->ETA, firstBucket->beginningETA) == 1)
    {
        prevFlight = flightPtr;
        flightPtr = flightPtr->next;
        free(prevFlight);
    }
    firstBucket->flights = flightPtr;

    // if first bucket itself is empty, case is handled later
    prevlptr = lptr = firstBucket;
    lptr = lptr->next;
    while (lptr != NULL)//traversing throgh all the buckets
    {
        // search chain, inside a bucket(departure time active)
        Flight *move = lptr->flights;
        prevFlight = flightPtr = lptr->flights;
        while (flightPtr != NULL && maxTime(flightPtr->departureTime, lptr->beginningETA) == 1)//chain of unnecessary flights
        {
            prevFlight = flightPtr;
            flightPtr = flightPtr->next;
        }

        // move the flights that do not belong to the bucket into move list
        if (prevFlight != flightPtr)
        {
            // remove that list
            move = lptr->flights;
            lptr->flights = flightPtr;
            prevFlight->next = NULL;
            Flight *temp = move;

            // check if appropriate bucket exists, if not, make one!!
            if (maxTime(move->ETA, prevlptr->endETA) != 1 || maxTime(prevlptr->beginningETA, move->ETA) != 1) // no bucket exists
            {
                Bucket *new = (Bucket *)malloc((sizeof(Bucket)));
                new->BucketID = BucketId;
                BucketId++;
                new->beginningETA.Hour = move->ETA.Hour;
                new->beginningETA.Min = 0;
                new->beginningETA.Sec = 0;
                new->endETA.Hour = move->ETA.Hour;
                new->endETA.Min = 59;
                new->endETA.Sec = 59;
                addTime(&new->beginningETA, t.Min);
                addTime(&new->endETA, t.Min);
                new->next = lptr;
                new->flights = NULL;
                prevlptr->next = new;//inserting bucket at first will never arise, since we are deleting first flights
                prevlptr = new;
            }
            Flight *ptr = prevlptr->flights;
            if (ptr == NULL)
            {
                prevlptr->flights = move;
            }
            else
            {
                while (ptr->next != NULL)//insert at last
                {
                    ptr = ptr->next;
                }
                ptr->next = move;
            }
        }
        prevlptr = lptr;
        lptr = lptr->next;
    }

    // remove empty buckets
    prevlptr = NULL;
    lptr = firstBucket;
    while (lptr != NULL)
    {
        if (lptr->flights == NULL)
        {
            Bucket *temp = lptr;
            lptr = lptr->next;
            if (prevlptr == NULL)
            {
                firstBucket = lptr;
            }
            else
            {
                prevlptr->next = lptr;
            }
            free(temp);
        }
        else
        {
            prevlptr = lptr;
            lptr = lptr->next;
        }
    }
    showBucketList(firstBucket);
}

Bucket *insert(Bucket *firstBucket, Flight *planePtr) // hour > 0 and < 24
{
    Bucket *bucketPtr = firstBucket, *prev = firstBucket;
    while (bucketPtr != NULL && (maxTime(bucketPtr->beginningETA, planePtr->ETA) >= 0))
    {
        prev = bucketPtr;
        bucketPtr = bucketPtr->next;
    }
    //prev = NULL when no bucket exists (beginning of the program)
    if (prev == NULL || (maxTime(planePtr->ETA, prev->endETA) < 0)) // no bucket exists, or insert a bucket
    {
        //create Bucket
        Bucket *tempBucket = (Bucket *)malloc(sizeof(Bucket));
        tempBucket->BucketID = BucketId;
        BucketId++;
        tempBucket->beginningETA.Hour = planePtr->ETA.Hour;
        tempBucket->beginningETA.Min = 0;
        tempBucket->beginningETA.Sec = 0;
        tempBucket->endETA.Hour = planePtr->ETA.Hour;
        tempBucket->endETA.Min = 59;
        tempBucket->endETA.Sec = 59;
        tempBucket->next = bucketPtr;
        if (prev != NULL)
            prev->next = tempBucket;
        else
            firstBucket = tempBucket;
        tempBucket->flights = planePtr;
    }
    else // bucket found
    {
        Flight *plane = prev->flights, *prevPlane = prev->flights;
        while (plane != NULL && maxTime(plane->departureTime, planePtr->departureTime) == 1)
        {
            prevPlane = plane;
            plane = plane->next;
        }
        planePtr->next = plane;
        if (plane == prev->flights)//insertion at start
        {
            prev->flights = planePtr;
        }
        else
        {
            prevPlane->next = planePtr;
        }
    }
    return firstBucket;
}

Bucket *setCurrentTime(Bucket *newBucket, Time t)
{
    Bucket *lptr = newBucket, *prevlptr = newBucket;
    Flight *flightPtr, *prevFlight;

    while (lptr != NULL && maxTime(lptr->endETA, t) == 1)
    {
        prevFlight = flightPtr = lptr->flights;
        while (flightPtr != NULL)
        {
            prevFlight = flightPtr;
            flightPtr = flightPtr->next;
            free(prevFlight);
        }
        prevlptr = lptr;
        lptr = lptr->next;
        free(prevlptr);
    }
    return lptr;
}

Time TimeDiff(Time A, Time B)
{
    Time temp;

    if (A.Hour > B.Hour)
    {
        temp.Hour = A.Hour - B.Hour;
        temp.Min = A.Min - B.Min;

        // If seconds are taken into account
        temp.Sec = A.Sec - B.Sec;

        if (temp.Sec < 0)
        {
            temp.Sec = temp.Sec + 60;
            temp.Min--;
        }

        if (temp.Min < 0)
        {
            temp.Min = temp.Min + 60;
            temp.Hour--;
        }
    }

    else if (A.Hour == B.Hour)
    {
        temp.Hour = 0;
        temp.Min = A.Min - B.Min;

        // If seconds are taken into account
        temp.Sec = A.Sec - B.Sec;
        if (temp.Sec < 0)
        {
            if (temp.Min > 0)
            {
                temp.Min--;
                temp.Sec = temp.Sec + 60;
            }

            else
            {
                temp = TimeDiff(B,A);
            }
        }
    }

    else
        temp = TimeDiff(B, A);

    return temp;
}

void showMenu(Bucket *firstBucket)
{
    int option;
    do
    {
        printf("\n\n1 - Set Current Time\n2 - Insert a Flight \n3 - Cancel Flight \n4 - Check Status \n5 - Rearrange Buckets\n6 - Show Bucket List\nPress any other key to exit\n\n");
        printf("Select option : ");
        scanf("%d", &option);
        Time temp, deptTime, ETA;

        switch (option)
        {
        case 1:
            printf("Enter time: ");
            scanf("%d", &temp.Hour);
            scanf("%d", &temp.Min);
            scanf("%d", &temp.Sec);
            if (temp.Hour > 23 || temp.Min > 59 || temp.Sec > 59) printf ("invalid time");
            else
                firstBucket = setCurrentTime(firstBucket, temp);
            break;

        case 2:
            printf("Enter details for the flight to be added :\n");
            int ID;
            printf("Flight ID : ");
            scanf("%d", &ID);
            printf("Departure time : \nhr = ");
            scanf("%d", &deptTime.Hour);
            printf("min = ");
            scanf("%d", &deptTime.Min);
            printf("sec = ");
            scanf("%d", &deptTime.Sec);

            printf("Expected Arrival time : \nhr = ");
            scanf("%d", &ETA.Hour);
            printf("min = ");
            scanf("%d", &ETA.Min);
            printf("sec = ");
            scanf("%d", &ETA.Sec);

            if (deptTime.Hour > 23 || deptTime.Min > 59 || deptTime.Sec >59 ||
                ETA.Hour >23 || ETA.Min >59 || ETA.Sec > 59) printf ("Invalid Time");
            else 
            {
                Flight *newNode;
                newNode = createNode(ID, deptTime, ETA);
                firstBucket = insert(firstBucket, newNode);
                printf("Flight Added Successfully.\n");
            }
            break;

        case 3:
            firstBucket = cancelFlight(firstBucket);
            break;

        case 4:
            checkStatus(firstBucket);
            break;

        case 5:
            rearrangeBuckets(firstBucket);
            break;

        case 6:
            showBucketList(firstBucket);
        }
    } while (option > 0 && option < 7);
}

int main()
{
    int tempFlightID;
    Time tempDepartureTime, tempETA;
    FILE *fptr = fopen("Data.csv", "r");

    Flight *planeNode; // temp pointer to link nodes in list

    Bucket *firstBucket;
    firstBucket = NULL;
    char line[100];
    while (fgets(line, 100, fptr) != NULL)
    {
        sscanf(line, "%d,%d,%d,%d,%d,%d,%d", &tempFlightID, &(tempDepartureTime.Hour), &(tempDepartureTime.Min), &(tempDepartureTime.Sec), &(tempETA.Hour), &(tempETA.Min), &(tempETA.Sec));
        planeNode = createNode(tempFlightID, tempDepartureTime, tempETA);
        firstBucket = insert(firstBucket, planeNode);
    }
    showMenu(firstBucket);
    return 0;
}