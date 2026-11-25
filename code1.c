/*Venetsanos Ioannis
  AM:2022202300141
  dit23141@uop.gr
  
  Karanika Argyrw
  AM:2022202300144
  dit23144@uop.gr*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct
{
    int year;
    int month;
    int day;
} mydate;

typedef struct
{
    int hour;
    int minutes;
} mytime;

typedef struct bilList
{
    char name[256];
    char city_country[256];
    char main_activity[256];
    char country[256];
    char *gender;
    mydate Birthdate;
    mytime Birthtime;
    char *net;
    char account_number[14];
    struct bilList *next;
    struct bilList *prev;
} billionaire;

billionaire *insert(billionaire *strptr, billionaire *enptr, billionaire *c);
billionaire *createbil(char *str);
void capitalize_name(char *name);
void RpUnknown(char *str);
int Validac(const char *accountNumber);
void Findbil(char *str, billionaire *head);
void Printbil(char *str, billionaire *head);
void sum_wealth(billionaire *head);
void DeleteN(billionaire **head);
void DeleteO(billionaire **head);
int calculateCheckDigit(const char *accountNumber);
void printList(billionaire *head);
void str_change(char *str);
void generateAccount(char *accountNumber);
void file_save(billionaire *head, char *filename);
void printlis(billionaire *temp);
void load_file(billionaire **head, billionaire **tail, char *filename);
void free_list(billionaire *head, char *tok);

int main()
{
    srand(time(NULL));
    billionaire *head = NULL;
    billionaire *tail = NULL;
    char *tok=malloc(256);
    if (tok==NULL)
    {
        printf("fail to allocate memory");
    }
    int i = 0;
    printf("$> ");
    do
    {
        billionaire *c = (billionaire *)malloc(sizeof(billionaire));
        if (c == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }
        int len = 0;
        char str[256];
        fgets(str, sizeof(str), stdin);
        len = strlen(str);
        if (len > 0 && str[len - 1] == '\n')
        {
            str[len - 1] = '\0';
        }
        if (strncmp(str, "newbil", 6) == 0)
        {
            c = createbil(str + 7);
            if (c != NULL)
            {
                head = insert(head, tail, c);
                if (tail == NULL)
                {
                    tail = head;
                }
                else
                {
                    tail = c;
                }
            }
            printf("Billionaire added successfully\n");
        }
        else if (strncmp(str, "deleteN", 8) == 0)
        {
            DeleteN(&head);
        }
        else if (strncmp(str, "deleteO", 8) == 0)
        {
            DeleteO(&head);
        }
        else if (strncmp(str, "sum", 4) == 0)
        {
            sum_wealth(head);
        }
        else if (strncmp(str, "find", 4) == 0)
        {
            Findbil(str, head);
        }
        else if (strncmp(str, "print", 5) == 0)
        {
            Printbil(str, head);
        }
        else if (strncmp(str, "save", 4) == 0)
        {

            strcpy(tok, str + 5);
            file_save(head, tok);
        }
        else if (strncmp(str, "load",4) == 0)
        {
            strcpy(tok, str + 5);
            load_file(&head, &tail, tok);
        }
        else if (strncmp(str, "exit", 5) == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command\n");
        }
        printf("$> ");
    } while (1);
    free_list(head,tok);
    printf("End of program\n");
    return 0;
}

billionaire *insert(billionaire *strptr, billionaire *enptr, billionaire *c)
{
    if (strptr == NULL)
    {
        c->next = c;
        c->prev = c;
        return c;
    }
    else
    {
        enptr->next = c;
        c->prev = enptr;
        c->next = strptr;
        strptr->prev = c;
        return strptr;
    }
}

billionaire *createbil(char str[256])
{
    billionaire *c = (billionaire *)malloc(sizeof(billionaire));
    if (c == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    c->gender = (char *)malloc(10 * sizeof(char));
    if (c->gender == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    c->net = (char*)malloc(20 * sizeof(float));
    if (c->net == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    mydate a;
    mytime b;

    int i = 0;
    char *token, *token2, *token3;
    token = strtok(str, ";");
    capitalize_name(token);
    strcpy(c->name,token); 
    token = strtok(NULL, ";"); 
    RpUnknown(token);
    strcpy(c->city_country,token);  
    token = strtok(NULL, ";");
    RpUnknown(token);
    strcpy(c->main_activity,token);  
    token = strtok(NULL, ";");
    strcpy(c->country,token); 
    token = strtok(NULL, ";");
    strcpy(c->gender, token);
    token = strtok(NULL, "/");
    a.day=atoi(token);
    token=strtok(NULL,"/");
    a.month=atoi(token);
    token=strtok(NULL," ");
    a.year=atoi(token);
    token=strtok(NULL,":");
    b.hour=atoi(token);
    token=strtok(NULL,";");
    b.minutes=atoi(token);
    c->Birthdate=a;
    c->Birthtime=b;
    token=strtok(NULL,"\0");
    if (token != NULL) 
    {
        str_change(token);
        strcpy(c->net, token);
    }
    generateAccount(c->account_number);
    c->prev = c;
    c->next = c;
    return c;
}

void capitalize_name(char *name)
{
    int i = 0;
    int cp = 1;
    while (name[i] != '\0')
    {
        if (isspace(name[i]))
        {
            cp = 1;
        }
        else if (cp)
        {
            name[i] = toupper(name[i]);
            cp = 0;
        }
        else
        {
            name[i] = tolower(name[i]);
        }
        i++;
    }
}

void RpUnknown(char *str)
{
    if (strcmp(str, "?") == 0 || strcmp(str, "Unknown") == 0)
    {
        strcpy(str, "Unknown");
    }
}

int Validac(const char *accountNumber)
{
    int len = strlen(accountNumber);
    int sum = 0;
    int i;
    for (i = 0; i < len; i++)
    {
        int digit = accountNumber[len - 1 - i] - '0';
        if (i % 2 == 1)
        {
            digit *= 2;
            if (digit > 9)
            {
                digit = digit / 10 + digit % 10;
            }
        }
        sum += digit;
    }
    return sum % 10 == 0;
}

void sum_wealth(billionaire *head)
{
    double ttl_net = 0;
    billionaire *curptr = head;
    if (curptr == NULL)
    {
        printf("Total wealth: 0.0\n");
        return;
    }
    do
    {
        ttl_net += atof(curptr->net);
        curptr = curptr->next;
    } while (curptr != head);
    printf("Total wealth: %.2f\n", ttl_net);
}

void DeleteN(billionaire **head)
{
    if (*head == NULL)
    {
        printf("The list is already empty, nothing to delete.\n");
        return;
    }
    billionaire *tail = (*head)->prev;
    if (*head == tail)
    {
        free((*head)->gender);
        free((*head)->net);
        free(*head);
        *head = NULL;
        printf("You deleted the only entry.\n");
    }
    else
    {
        billionaire *new_tail = tail->prev;
        new_tail->next = *head;
        (*head)->prev = new_tail;
        free(tail->gender);
        free(tail->net);
        free(tail);
        printf("The most recent entry has been deleted.\n");
    }
}

void DeleteO(billionaire **head)
{
    if (*head == NULL)
    {
        printf("The list is already empty, nothing to delete.\n");
        return;
    }
    
    billionaire *tail = (*head)->prev;
    if (*head == tail)
    {
        free((*head)->gender);
        free((*head)->net);
        free(*head);
        *head = NULL;
        printf("You deleted the only entry.\n");
    }
    else
    {
        billionaire *new_head = (*head)->next;
        new_head->prev = tail;
        tail->next = new_head;
        
        free((*head)->gender);
        free((*head)->net);
        free(*head);
        
        *head = new_head;
        printf("The oldest entry has been deleted.\n");
    }
}

void generateAccount(char *accountNumber)
{
    int i;
    for (i = 0; i < 12; i++)
    {
        if (i==5 || i== 11)
        {
            accountNumber[i]='-';
        }
        else
        {
            accountNumber[i] = '0' + rand() % 10;
        }
    }
    int checkDigit = calculateCheckDigit(accountNumber);
    accountNumber[12] = '0' + checkDigit;
    accountNumber[13] = '\0';
}

int calculateCheckDigit(const char *accountNumber)
{
    int len = strlen(accountNumber);
    int sum = 0, checkd, i, digit;

    for (i = 0; i < len; i++)
    {
        digit = accountNumber[len - 1 - i] - '0';

        if (i % 2 == 0)
        {
            digit *= 2;
            if (digit > 9)
            {
                digit = digit / 10 + digit % 10;
            }
        }

        sum += digit;
    }
    checkd = (sum * 9) % 10;
    return checkd;
}

void printList(billionaire *head)
{
    if (head == NULL)
    {
        printf("The list is empty.\n");
        return;
    }
    billionaire *temp = head;
    do
    {
        printlis(temp);
        temp = temp->next;
    } while (temp != head);
}

void Findbil(char *str, billionaire *head)
{
    if (head == NULL) 
    {
        return;
    }

    billionaire *temp = head;
    if (strchr(str, '*') != NULL)
    {
        printList(head);
        return;
    }

    char *token = strtok(str, " ");
    if (token == NULL) 
    {
        return;
    }
    char *token2 = strtok(NULL, "\0");
    if (token2 == NULL) 
    {
        return;
    }

    do
    {
        if (strchr(token, 'N') || strchr(token, 'n'))
        {
            if (temp->name != NULL && strstr(temp->name, token2))
            {
                printlis(temp);
            }
        }
        if (strchr(token, 'C') || strchr(token, 'c')) 
        {
            if (temp->city_country != NULL && strstr(temp->city_country, token2))
            {
                printlis(temp);
            }
        }
        if (strchr(token, 'S') || strchr(token, 's'))
        {
            if (temp->main_activity != NULL && strstr(temp->main_activity, token2))
            {
                printlis(temp);
            }
        }
        temp = temp->next;
    } while (temp != head);
}



void file_save(billionaire *head, char *filename)
{
    if (head == NULL)
    {
        printf("The list is empty, nothing to save.\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    billionaire *temp = head;

    if (file == NULL)
    {
        printf("There was a problem with opening this file.\n");
        return;
    }
    else
    {
        do
        {
            fprintf(file, "%s;", temp->name);
            fprintf(file, "%s;", temp->city_country);
            fprintf(file, "%s;", temp->main_activity);
            fprintf(file, "%s;", temp->country);
            fprintf(file, "%s;", temp->gender);
            fprintf(file, "%02d/%02d/%04d ", temp->Birthdate.day, temp->Birthdate.month, temp->Birthdate.year);
            fprintf(file, "%02d:%02d;", temp->Birthtime.hour, temp->Birthtime.minutes);
            fprintf(file, "%s;", temp->account_number);
            fprintf(file, "%s\n", temp->net);
            temp = temp->next;
        } while (temp != head);
    }
    printf("Successful save\n");
    fclose(file);
}


void printlis(billionaire *temp)
{
    printf("Name: %s\n", temp->name);
    printf("City/Country: %s\n", temp->city_country);
    printf("Main Activity: %s\n", temp->main_activity);
    printf("Origin Country: %s\n", temp->country);
    printf("Gender: %s\n", temp->gender);
    printf("Birth Date: %02d/%02d/%04d\n", temp->Birthdate.day, temp->Birthdate.month, temp->Birthdate.year);
    printf("Birth Time: %02d:%02d\n", temp->Birthtime.hour, temp->Birthtime.minutes);
    printf("Account number: %s\n", temp->account_number);
    printf("Net Worth:%s billion USD\n\n", temp->net);
}

void load_file(billionaire **head, billionaire **tail, char *str) 
{
    
    if (str == NULL) 
    {
        printf("Invalid arguments\n");
        return;
    }

    char *tok = strtok(str, " ");
    if (tok == NULL) {
        printf("Invalid input string\n");
        return;
    }
    int limit = atoi(tok);

    char *filename = strtok(NULL, "\n");
    if (filename == NULL) 
    {
        printf("Invalid input string\n");
        return;
    }


    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Unable to open file: %s\n", filename);
        return;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < limit) 
    {
        billionaire *c = createbil(line);
        if (c != NULL) 
        {
            *head = insert(*head, *tail, c);
            if (*tail == NULL) {
                *tail = *head;
            } else {
                (*tail)->next = c;
                *tail = c;
            }
            count++;
        }
    }
    printf("Successful loading of the file\n");
    fclose(file);
}


void Printbil(char *str, billionaire *head)
{
    if (head == NULL) 
    {
        printf("Empty list\n");
        return;
    }

    billionaire *temp = head;
    char *token1 = NULL;
    char *token2 = NULL;
    char *token3 = NULL;
    int day, month, year, hour, minute;
    double netValue;
    char comparison = str[5];
    char *value = str + 6; 

    if (strchr(value, '/')) 
    {
        token1 = strtok(value, "/");
        token2 = strtok(NULL, "/");
        token3 = strtok(NULL, "\0");

        if (token1 == NULL || token2 == NULL || token3 == NULL)
        {
            printf("Invalid date format\n");
            return;
        }

        day = atoi(token1);
        month = atoi(token2);
        year = atoi(token3);

        do 
        {
            if (comparison == '+')
            {
                if ((temp->Birthdate.year > year) ||
                    (temp->Birthdate.year == year && temp->Birthdate.month > month) ||
                    (temp->Birthdate.year == year && temp->Birthdate.month == month && temp->Birthdate.day > day))
                {
                    printlis(temp);
                }
            }
            else if (comparison == '=')
            {
                if (temp->Birthdate.day == day && temp->Birthdate.month == month && temp->Birthdate.year == year)
                {
                    printlis(temp);
                }
            }
            else if (comparison == '-')
            {
                if ((temp->Birthdate.year < year) ||
                    (temp->Birthdate.year == year && temp->Birthdate.month < month) ||
                    (temp->Birthdate.year == year && temp->Birthdate.month == month && temp->Birthdate.day < day))
                {
                    printlis(temp);
                }
            }
            temp = temp->next;
        } while (temp != head);
    } 
    else if (strchr(value, ':')) 
    {
        token1 = strtok(value, ":");
        token2 = strtok(NULL, ":");

        if (token1 == NULL || token2 == NULL)
        {
            printf("Invalid time format\n");
            return;
        }

        hour = atoi(token1);
        minute = atoi(token2);

        do 
        {
            if (comparison == '+')
            {
                if ((temp->Birthtime.hour > hour) || 
                    (temp->Birthtime.hour == hour && temp->Birthtime.minutes > minute))
                {
                    printlis(temp);
                }
            }
            else if (comparison == '=')
            {
                if (temp->Birthtime.hour == hour && temp->Birthtime.minutes == minute)
                {
                    printlis(temp);
                }
            }
            else if (comparison == '-')
            {
                if ((temp->Birthtime.hour < hour) || 
                    (temp->Birthtime.hour == hour && temp->Birthtime.minutes < minute))
                {
                    printlis(temp);
                }
            }
            temp = temp->next;
        } while (temp != head);
    } 
    else 
    {
        netValue = atof(value);

        do 
        {
            if (comparison == '+')
            {
                if (atof(temp->net) > netValue)
                {
                    printlis(temp);
                }
            }
            else if (comparison == '=')
            {
                if (atof(temp->net) == netValue)
                {
                    printlis(temp);
                }
            }
            else if (comparison == '-')
            {
                if (atof(temp->net) < netValue)
                {
                    printlis(temp);
                }
            }
            temp = temp->next;
        } while (temp != head);
    }
}


void str_change(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ',')
        {
            str[i] = '.';
        }
    }
}


void free_list(billionaire *head,char *tok) 
{
    if (head == NULL)
    {
        return;
    } 
    billionaire *temp = head;
    do 
    {
        billionaire *next = temp->next;
        free(temp->gender);
        free(temp->net);
        free(temp);
        temp=next;
    } while (temp != head);
    free(tok);

}