//Dela Cruz, Simon Daniel M.    BSCS 2B M
//Tejerero, Andrew James S.     BSCS 2A-M
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define MAX 100
#define pwmaxlength 5
const int secret = 30;
char passw[pwmaxlength];

typedef struct data {
    char name[MAX];
    int id;
    char password[pwmaxlength];

}voter;

typedef struct cand{
    char pres[MAX];
    char vPres[MAX];
    char headCoA[MAX], headCoSRW[MAX],headCoI[MAX],headSec[MAX],headCoF[MAX],headCoSC[MAX], headCoSP[MAX];
    char govAF[MAX], vGovAF[MAX], govCE[MAX], vGovCE[MAX], govCS[MAX], vGovCS[MAX], govLA[MAX], vGovLA[MAX], govIE[MAX], vGovIE[MAX];
}  position;

struct receipt{
    char pres[MAX];
    char vPres[MAX];
    char headCoA[MAX], headCoSRW[MAX],headCoI[MAX],headSec[MAX],headCoF[MAX],headCoSC[MAX], headCoSP[MAX];
    char govAF[MAX], vGovAF[MAX], govCE[MAX], vGovCE[MAX], govCS[MAX], vGovCS[MAX], govLA[MAX], vGovLA[MAX], govIE[MAX], vGovIE[MAX];
} RECEIPT;

typedef struct list{
    voter Q[MAX];
    position P[2];
    int last;
}LIST;

LIST L;
struct receipt vr;

void makenull();
void insert(voter x);
void retrieve();
int isfull();
int isempty();
int locateName(char x[]);
int locateID(int x);
int menu();
void saveToUSB(voter x);
void saveToDatabase();
char pin();
char *decrypt(char *e);
void voting(int x);
void saveReceipt(int x);

int main()
{
    int index;

    FILE *fp1, *fp2;
/*----------------------------------------------------*/
/*   Note: USB Directory may vary                     */
/*----------------------------------------------------*/
    do{
        fp1 = fopen("F:\\temp.txt", "w");       //attempt to create a temporary file in a usb directory for initial scanning
        fclose(fp1);
        system("cls");                          //will only stop if the creation of temporary file is successful
        printf("Please Insert USB...");
    }while(fp1 = fopen("F:\\temp.txt","r")==NULL);

    voter vtr;
    char nameTemp[MAX], ans;
    int option;
    makenull();
    while(1){       //infinite loop - one entry, one exit
        option = menu();
        system("cls");
        switch (option){
        case 1 :        //Module 2 Voting System
            retrieve();//retrieves updated record.txt file if a new voter registers
            int compare;
            while(1){ //infinite loop - one entry, one exit
            system("cls");
            printf("=======================================================");
            printf("\nEnter studet id: TUPM-M-");
            scanf(" %d", &vtr.id);
            printf("\n=======================================================\n");
            index = locateID(vtr.id);  //Finding the id input by the user in the list
            if(index ==-1){
                system("cls");
                printf("\nSorry. TUP ID not registered.\n");
                getch();
                break;
            }
            else{


                int i;
                for(i=3; i>0; i--){       //Loop that limits the user's attempt to login
                    system("cls");
                    printf("=======================================================");
                    printf("\nEnter the following info to proceed: \nTUP ID: %d\n", vtr.id);
                    pin();
                    printf("=======================================================\n");
                    compare = strcmp(decrypt(passw), decrypt(L.Q[index].password));           //compares user input to the database for a password match and decrypting it

                    if(compare == 0){       // if else statement to check if the input password is the same with the one saved in the data base
                        system("cls");
                        printf("=======================================================");
                        printf("\nLOGIN SUCCESSFUL\n");
                        printf("ACCOUNT NAME:\t%s\n", L.Q[index].name);
                        printf("ACCOUNT ID:\tTUP-M-%d\n", L.Q[index].id);
                        printf("=======================================================\n");
                        system("pause");
                        voting(index); //Calls the voting system function
                        break;
                    }
                    else{
                        if(i==1){
                            printf("\nINVALID PASSWORD (%d)", i-1);
                            printf("\nAll attempts used. Returning to Main Menu");
                            getch();
                            break;
                        }
                        printf("\nINVALID PASSWORD. Try again. (%d)", i-1);
                        getch();
                    }
                }
                break;
            }
        }
        break;
        case 2 :    // Module 1 Registration
        if(fp2 = fopen("F:\\VotersInfo.txt","r")==NULL){    //checks the USB drive for the copy of login credentials
                                                            //to prevent the usb from multiple use as voting card
            retrieve();
            while(1){           //infinite loop - one entry, one exit
                system("cls");
                //Asks the user's informations
                printf("=======================================================\n");
                printf("\t\tWELCOME!\n\nPlease enter your name: ");
                printf("\n=======================================================\n");
                scanf(" %[^\n]", vtr.name);

                    printf("Enter TUP ID: TUP-M-");
                    scanf("%d", &vtr.id);
                    if(locateID(vtr.id)>=0){
                        printf("\nTUP ID already registered.");
                        getch();
                        break;
                    }
                    else{
                        pin();
                        strcpy(vtr.password, passw);
                        printf("\n=======================================================\n");
                        printf("Please check the information below before saving...\n\n");
                        printf("Name:\t\t%s\nTUP Number:\tTUP-M-%d\n", vtr.name, vtr.id);
                        printf("\n=======================================================\n");
                        printf("\nSubmit your registration? (Y/N) : ");
                        scanf(" %c", &ans);
                        if(ans=='y'||ans=='Y'){
                            saveToUSB(vtr);
                            insert(vtr);
                            saveToDatabase();
                            printf("\nRegistration Complete.");
                            getch();
                            break;
                        }
                        else{
                            printf("\nCancel registration? (Y/N) ");
                            scanf(" %c", &ans);
                            if(ans=='y'||ans=='Y'){
                                printf("\nReturning to Main Menu...");
                                getch();
                                break;
                            }
                            else
                                continue;
                        }
                    }
            }
        }
        else{       //Checks if the flash drive used was already registered
            printf("USB already registered. Insert new one.");
            getch();
        }
        break;
        case 3: //Exit
            exit(0);
        default:    //Error checking if the user picks a number aside from 1-3
            printf("1-3 only.");
            getch();
        }
    }
}


char pin(){     //Pin function that accepts the 4 digit pin code from the user and hides it
    char ch;
    int chpos = 0;
    printf("Please enter 4-digit pin password for your voter's account: ");
    while(1){
        ch = getch();
        if ( ch == 13 ){    //Breaks the function if the user inputs enter
            break;
        }
        else if ( ch == 8 || ch == 127 ){   //Deletes previous inputs if the user input backspace or delete
            if ( chpos>0){
                chpos--;
                passw[chpos] = '\0';
                printf("\b \b");
            }
        }
        else if ( ch == 32 || ch == 9){     //ignores the tab and space input of user
            continue;
        }
        else{                              //Accepts all input that allowed by the function and encrypting it using the 'secret' constant
            if ( chpos<pwmaxlength-1 ){
                passw[chpos] = ch+secret;
                chpos++;
                printf("*");
            }
        }
    }
    passw[chpos]= '\0';                     // Adds null terminating character at the end to complete is as a string
    printf("\n");
    if (strlen(passw)==0){                  // Checks if the user immediately inputs enter
        printf("No Password Entered\n");
        pin();
    }else{
        return (passw);                     //returns the pincode entered by the user to the main
    }
}

char *decrypt(char *e){                     // decrypts the pincode input by the user and saved in the data base
    int l;
    for (l=0;l<pwmaxlength;l++){            //for loop that subtracts the value of 'secret' to every character that saved in the pincode
        e[l] = e[l]-secret;
    }
    return(e);                              //retuns the decrypted pincode to the main

}

void voting(int x){                        //Voting system fucntion
    system("cls");
    int index, id = x;
    makenull();
    candidates();
    printf("---------------VOTING MODE---------------\n");
    printf("\nPRESIDENT\n");
    for(int i=0; i<L.last+1;i++){
            printf("(%d)%s\t", i+1, L.P[i].pres);               //Prints the list of candidates for the position
        }   printf("\nYour vote: "); scanf("%d", &index);       //Scans the vote of the user
    strcpy(vr.pres,L.P[index-1].pres);                          //Stores the vote of the user to a list

    printf("\nVICE PRESIDENT\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].vPres);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.vPres,L.P[index-1].vPres);

    printf("\nHEAD, COMMITTEE ON APPOINTMENTS\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headCoA);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headCoA,L.P[index-1].headCoA);

    printf("\nHEAD, COMMITTEE ON STUDENT'S RIGHT AND WELFARE\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headCoSRW);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headCoSRW,L.P[index-1].headCoSRW);

    printf("\nHEAD, COMMITTEE ON INFORMATION\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headCoI);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headCoI,L.P[index-1].headCoI);

    printf("\nHEAD SECRETARIAT\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headSec);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headSec,L.P[index-1].headSec);

    printf("\nHEAD, COMMITTEE ON FINANCE\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headCoF);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headCoF,L.P[index-1].headCoF);

    printf("\nHEAD, COMMITTEE ON SPORTS AND CULTURE\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headCoSC);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headCoSC,L.P[index-1].headCoSC);

    printf("\nHEAD, COMMITTEE ON SPECIAL PROJECTS\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].headCoSP);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.headCoSP,L.P[index-1].headCoSP);

    printf("\nGOVERNOR: COLLEGE OF ARCHITECTURE AND FINE ARTS\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].govAF);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.govAF,L.P[index-1].govAF);

        printf("\nVICE-GOVERNOR: COLLEGE OF ARCHITECTURE AND FINE ARTS\n");
        for(int i=0; i<L.last+1;i++){
                printf("(%d)%s\t", i+1, L.P[i].vGovAF);
            }   printf("\nYour vote: "); scanf("%d", &index);
        strcpy(vr.vGovAF,L.P[index-1].vGovAF);

    printf("\nGOVERNOR: COLLEGE OF ENGINEERING\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].govCE);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.govCE,L.P[index-1].govCE);

        printf("\nVICE-GOVERNOR: COLLEGE OF ENGINEERING\n");
        for(int i=0; i<L.last+1;i++){
                printf("(%d)%s\t", i+1, L.P[i].vGovCE);
        }       printf("\nYour vote: "); scanf("%d", &index);
        strcpy(vr.vGovCE,L.P[index-1].vGovCE);

    printf("\nGOVERNOR: COLLEGE OF SCIENCE\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].govCS);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.govCS,L.P[index-1].govCS);

        printf("\nVICE-GOVERNOR: COLLEGE OF SCIENCE\n");
        for(int i=0; i<L.last+1;i++){
                printf("(%d)%s\t", i+1, L.P[i].vGovCS);
        }       printf("\nYour vote: "); scanf("%d", &index);
        strcpy(vr.vGovCS,L.P[index-1].vGovCS);

    printf("\nGOVERNOR: COLLEGE OF LIBERAL ARTS\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].govLA);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.govLA,L.P[index-1].govLA);

        printf("\nVICE-GOVERNOR: COLLEGE OF LIBERAL ARTS\n");
        for(int i=0; i<L.last+1;i++){
                printf("(%d)%s\t", i+1, L.P[i].vGovLA);
        }       printf("\nYour vote: "); scanf("%d", &index);
        strcpy(vr.vGovLA,L.P[index-1].vGovLA);

    printf("\nGOVERNOR: COLLEGE OF INDUSTRIAL EDUCATION\n");
    for(int i=0; i<L.last+1;i++){
        printf("(%d)%s\t", i+1, L.P[i].govIE);
    } printf("\nYour vote: "); scanf("%d", &index);
    strcpy(vr.govIE,L.P[index-1].govIE);

        printf("\nVICE-GOVERNOR: COLLEGE OF INDUSTRIAL EDUCATION\n");
        for(int i=0; i<L.last+1;i++){
                printf("(%d)%s\t", i+1, L.P[i].vGovIE);
            }   printf("\nYour vote: "); scanf("%d", &index);
        strcpy(vr.vGovIE,L.P[index-1].vGovIE);
    system("pause");
    saveReceipt(id);
}

int menu(){ //Displays initial MENU options
    int i, opt;
    char *m[] = {"YES", "NO", "EXIT"};
    system("cls");
    printf("\n=======================================================\n");
    printf("Welcome to Student Elections!\nAre you registered?\n\n");
    for(i=0;i<3;i++)
        printf("%d.) %s\n", i+1,m[i]);
    printf("\n Select (1-3): ");
    printf("\n=======================================================\n");
    scanf("%d", &opt);

    return(opt);
}

void makenull(){    //make the list empty
    L.last = -1;
}

void insert(voter x){  //takes values from parameters to store in the structure
    if(isfull()){
        printf("List is full.");
        getch();
    }
    else{
        L.last++;
        strcpy(L.Q[L.last].name, x.name);
        L.Q[L.last].id = x.id;
        strcpy(L.Q[L.last].password, x.password);
    }
}

int isempty(){
    return (L.last == -1); //tells if the list is empty
}

int isfull(){
    return(L.last == MAX-1); //tells if the list is full
}

int locateName(char x[]){ //checks the structure for match and returns the index if successful
    int a;
    for (int i = 0; i<= L.last; i++){
        a = strcmp(L.Q[i].name,x);
        if (a == 0)
            return(i);
    }
    return(-1);
}

int locateID(int x){    //checks the structure for match and returns the index if successful
    int a;
    for (int i = 0; i<= L.last; i++){
        if (x == L.Q[i].id)
            return(i);
    }
    return(-1);
}


void retrieve(){    //scans the values from a text file into the structure
    FILE *localFile;
    voter y;
    localFile = fopen("record.txt", "r+");
    if(localFile == NULL){//file not found
        localFile = fopen("record.txt","w+");
    }
    else{//file exists
        while(fscanf(localFile, "%[^\t]\t%d\t%s\n", y.name, &y.id, y.password) != EOF){
            insert(y);
        }
    fclose(localFile);
    }
}

void saveToUSB(voter x){        //saves a copy of user's login credentials
    FILE *usbFile;
    usbFile = fopen("F:\\VotersInfo.txt","w");
    fprintf(usbFile,"Name: %s\nID: %d\n",x.name, x.id);
    fclose(usbFile);
}

void saveToDatabase(){  //saves all the values from the data structure into external text file
    FILE *fp;
    fp = fopen("record.txt","w+");
    for(int i=0;i<=L.last;i++)
        fprintf(fp, "%s\t%d\t%s\n", L.Q[i].name, L.Q[i].id, L.Q[i].password);
    fclose(fp);
}

void candidates(){          //scans the names of the candidates from a local text file
    FILE *fp = fopen("candidates.txt", "r");
    for(int i=0; i<2; i++){
        fscanf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", L.P[i].pres,L.P[i].vPres,
               L.P[i].headCoA,L.P[i].headCoSRW,L.P[i].headCoI,L.P[i].headSec,L.P[i].headCoF,L.P[i].headCoSC,L.P[i].headCoSP,
               L.P[i].govAF,L.P[i].vGovAF,L.P[i].govCE,L.P[i].vGovCE,L.P[i].govCS,L.P[i].vGovCS,L.P[i].govLA,L.P[i].vGovLA,
               L.P[i].govIE,L.P[i].vGovIE);
        L.last++;
    }
    fclose(fp);
}

void saveReceipt(int x){            //takes the index of the registered ID used to login as a parameter
    FILE *local, *usb;
    usb = fopen("G:\\Vote Receipt.txt", "w");           //saves a copy of user's choices in the election to the USB drive
    fprintf(usb,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            vr.pres, vr.vPres, vr.headCoA, vr.headCoSRW, vr.headCoI, vr.headSec, vr.headCoF,
            vr.headCoSC, vr.headCoSP, vr.govAF, vr.vGovAF, vr.govCE, vr.vGovCE, vr.govCS,
            vr.vGovCS, vr.govLA, vr.vGovLA, vr.govIE, vr.vGovIE);
    fclose(usb);

    char filename[MAX] = "Vote Receipt_ID-";
    char tupID[MAX];
    char text[MAX] = ".txt";
    itoa(L.Q[x].id, tupID, 10);
    strcat(filename,tupID);          //concatenate strings to create receipt with unique filename based on the voter's ID
    strcat(filename,text);

    local = fopen(filename,"w");                        //saves a copy of user's choices in the election to a local folder
    fprintf(local,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            vr.pres, vr.vPres, vr.headCoA, vr.headCoSRW, vr.headCoI, vr.headSec, vr.headCoF,
            vr.headCoSC, vr.headCoSP, vr.govAF, vr.vGovAF, vr.govCE, vr.vGovCE, vr.govCS,
            vr.vGovCS, vr.govLA, vr.vGovLA, vr.govIE, vr.vGovIE);
    fclose(local);
}

