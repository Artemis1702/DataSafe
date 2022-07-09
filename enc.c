#include<stdio.h>
#include<string.h>
#include<sys\stat.h>
#include<sys\types.h>
#include<stdlib.h>
#include<direct.h>

#define SIZE 10000



struct user
{
    char passwd[30];
    char username[100];
    char path[60];    
    // FILE* details;
};

typedef struct user user;

void create(user*);
int existing(user*);
char * base64Encoding(char[]);
char * base64Decoding(char[]);
void base64Encoder(char[], user*);
void base64Decoder(char[], user*);
void input(user*);
void output(user*);


char cwd[500];

void create(user* u)
{
    FILE *temp;
    //qwerty.txt contains the username and password of the user 
    temp = fopen("qwerty.txt","w");
    char un[100], pw[100];

    int check;
    printf("ENTER THE USERNAME : ");
    scanf("%s", &u->username);

    printf("ENTER THE PASSWORD YOU WOULD LIKE TO USE : ");
    scanf("%s", &u->passwd);

    //creates a folder
    check = mkdir(u->username);

    if (check == 0)
    {
        printf("created\n");
        //copying the encoded username and password to be stored
        strcpy(un,base64Encoding(u->username));
        strcpy(pw,base64Encoding(u->passwd));
        
        fprintf(temp, "%s\n%s\n",un,pw);
        fclose(temp);

        snprintf(u->path, sizeof(u->path),"%s\\%s\\", cwd, u->username);
    }
    else
    {
        printf("Folder not created\n");
    }
}

int existing(user *u)
{
    FILE *temp;
    
    char un[100],pw[100],un1[100],pw1[100];
    temp = fopen("qwerty.txt","r");

    //checking for qwerty.txt
    if(temp == NULL)
    {
        
        printf("User doesn't exist.\nCreating a new one.......\n");
        create(u);

        return 1;
    }
    else
    {
        
        printf("ENTER YOUR USERNAME : ");
        scanf("%s",un);

        printf("\nENTER YOUR PASSWORD : ");
        scanf("%s",pw);

        fscanf(temp,"%s\n%s",un1,pw1);
        strcpy(un1,base64Decoding(un1));
        strcpy(pw1,base64Decoding(pw1));
        strcpy(u->username, un1);
        strcpy(u->passwd, pw1);
        snprintf(u->path, sizeof(u->path),"%s\\%s\\", cwd, u->username);

        if(strcmpi(un, u->username) == 0 && strcmpi(pw, u->passwd) == 0)
        {
            fclose(temp);
            return 1;
        }

        fclose(temp);
    }

    return 0;

}

void input(user* u)
{
    FILE *inp;
    char name[100], temp[50], buff;

    printf("ENTER THE NAME OF THE FILE : ");
    scanf("%s",temp);
    snprintf(name, sizeof(name), "%s\\%s\\%s.txt", cwd, u->username,temp);
    


    inp = fopen(name, "w");

    printf("ENTER \\ TO STOP WRITING INTO THE FILE WHEN DONE\n");
    
    do{
        scanf("%c", &buff);
        if (buff == '\\') break;
        fputc(buff, inp);

    }while (buff != '\\');

    fclose(inp);

    base64Encoder(name, u);
}


void output(user *u)
{
    char n[100];
    
    printf("ENTER THE NAME OF THE FILE (without extension) : ");
    scanf("%s",&n);

    char fulln[100];
    snprintf(fulln, sizeof(fulln), "%s%s.txt", u->path, n);
    
    FILE* out;
    
    out = fopen(fulln,"r");
    if(out == NULL)
    {
        printf("\nFILE DOES NOT EXIST\nEXITING...................");
        return;
    }

    base64Decoder(fulln, u);

}

char * base64Encoding(char samp[]){

    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

    // Resultant string 
    char *res_str = (char *) malloc(SIZE * sizeof(char)); 
      
    int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp; 
    int i, j, k = 0;

    int len_str = strlen(samp);
      
    // Loop takes 3 characters at a time from  
    // samp and stores it in val 
    for (i = 0; i < len_str; i += 3) 
        { 
            val = 0, count = 0, no_of_bits = 0; 
  
            for (j = i; j < len_str && j <= i + 2; j++) 
            { 
                // binary data of samp is stored in val 
                val = val << 8;  
                  
                // (A + 0 = A) stores character in val 
                val = val | samp[j];
                  
                // calculates how many time loop  
                // ran if "MEN" -> 3 otherwise "ON" -> 2 
                count++; 
              
            } 
  
            no_of_bits = count * 8;  
  
            // calculates how many "=" to append after res_str. 
            padding = no_of_bits % 3;  
  
            // extracts all bits from val (6 at a time)  
            // and find the value of each block 
            while (no_of_bits != 0)  
            { 
                // retrieve the value of each block 
                if (no_of_bits >= 6) 
                { 
                    temp = no_of_bits - 6; 
                      
                    // binary of 63 is (111111) f 
                    index = (val >> temp) & 63;  
                    no_of_bits -= 6;          
                } 
                else
                { 
                    temp = 6 - no_of_bits; 
                      
                    // append zeros to right if bits are less than 6 
                    index = (val << temp) & 63;  
                    no_of_bits = 0; 
                } 
                res_str[k++] = char_set[index]; 
            } 
    } 
  
    // padding is done here 
    for (i = 1; i <= padding; i++)  
    { 
        res_str[k++] = '='; 
    }
    res_str[k++] = '\n';
    res_str[k] = '\0'; 
  
    return res_str;
}

void base64Encoder(char name[], user *u)
{
    FILE *file;
    FILE *enc_file;

    file = fopen(name, "r");

    char enc_name[100];
    snprintf(enc_name, sizeof(enc_name), "%s\\%s\\enc.txt", cwd, u->username);
    enc_file = fopen(enc_name, "w");

    char samp[1000];
    fgets(samp,1000,file);

    char * res_str;

    while(!feof(file))
    {

        res_str = base64Encoding(samp);
        fputs(res_str, enc_file);
        fgets(samp, 1000, file);

        if(feof(file)){
            res_str = base64Encoding(samp);
            fputs(res_str, enc_file);
        }

    }

    fclose(file);
    fclose(enc_file);

    file = fopen(name, "w");
    enc_file = fopen(enc_name, "r");

    char c = fgetc(enc_file);
    while (c != EOF)
    { 
        fputc(c, file); 
        c = fgetc(enc_file);
    }

    fclose(file);
    fclose(enc_file);
    enc_file = fopen(enc_name, "w");
    fclose(enc_file);
}

char * base64Decoding(char samp[]){
    
    char* decoded_string; 
    decoded_string = (char*)malloc(sizeof(char) * SIZE); 

    int i, j, k = 0;
    int len_str = strlen(samp);

    // stores the bitstream. 
    int num = 0; 

    // count_bits stores current 
    // number of bits in num. 
    int count_bits = 0; 

    // selects 4 characters from 
    // encoded string at a time. 
    // find the position of each encoded 
    // character in char_set and stores in num. 
    for (i = 0; i < len_str; i += 4)  
    { 
        num = 0, count_bits = 0; 
        for (j = 0; j < 4; j++) 
        { 
            
            // make space for 6 bits. 
            if (samp[i + j] != '=')  
            { 
                num = num << 6; 
                count_bits += 6; 
            } 

            /* Finding the position of each samp  
            character in char_set  
            and storing in "num", use OR  
            '|' operator to store bits.*/

            // samp[i + j] = 'E', 'E' - 'A' = 5 
            // 'E' has 5th position in char_set. 
            if (samp[i + j] >= 'A' && samp[i + j] <= 'Z') 
                num = num | (samp[i + j] - 'A'); 

            // samp[i + j] = 'e', 'e' - 'a' = 5, 
            // 5 + 26 = 31, 'e' has 31st position in char_set. 
            else if (samp[i + j] >= 'a' && samp[i + j] <= 'z') 
                num = num | (samp[i + j] - 'a' + 26); 

            // samp[i + j] = '8', '8' - '0' = 8 
            // 8 + 52 = 60, '8' has 60th position in char_set. 
            else if (samp[i + j] >= '0' && samp[i + j] <= '9') 
                num = num | (samp[i + j] - '0' + 52); 

            // '+' occurs in 62nd position in char_set. 
            else if (samp[i + j] == '+') 
                num = num | 62; 

            // '/' occurs in 63rd position in char_set. 
            else if (samp[i + j] == '/') 
                num = num | 63; 

            // ( str[i + j] == '=' ) remove 2 bits 
            // to delete appended bits during encoding. 
            else { 
                num = num >> 2; 
                count_bits -= 2; 
            } 
        } 

        while (count_bits != 0) 
        { 
            count_bits -= 8; 

            // 255 in binary is 11111111 
            decoded_string[k++] = (num >> count_bits) & 255; 
        } 
    } 

    // place NULL character to mark end of string. 
    decoded_string[k] = '\0';
    return decoded_string;
}

void base64Decoder(char name[], user *u)
{

    FILE *enc_file;
    
    enc_file = fopen(name, "r");

    char samp[1000];
    fgets(samp, 1000, enc_file);

    char* decoded_string;

    while (!feof(enc_file))
    {
        decoded_string = base64Decoding(samp);
        printf("%s", decoded_string);
        fgets(samp, 1000, enc_file);
    }

    fclose(enc_file);
}




int main()
{

    user u;

    getcwd(cwd, sizeof(cwd));
    
    // Creates a new user or takes the existing one
    int created_or_creds_correct = existing(&u);

    if(!created_or_creds_correct){
        printf("The Creds are wrng\n");
        return 0;
    }

    int choice;


    do{

        printf("\n1)CREATE A FILE\n2)RECOVER A FILE\n3)QUIT\n");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                input(&u);
                break;

            case 2:
                output(&u);
                break;
        }

    }while(choice != 3);

} 