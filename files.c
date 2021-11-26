/**************************************************************************************
 * @file    files.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   program body containing write and read routines destined to store and 
 *          retrieve data from cipher sheets.
 **************************************************************************************
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 * ************************************************************************************
*/

const char* OUT_FORMAT = "{%s-%s},\n";
const char* IN_FORMAT = "{[^-]-[^-]}\n";

typedef struct ciphertext
{
    char tag[10];
    char text[64];  
          
}ciphertext;

static u8 writeFile(u8* dataOUT)
{
    system("cls");

    u8 file_name[20] = {0};
    ciphertext cipher_out;
    FILE* file;

    printf( "Type in file name with extension (e.g. .txt .dat... )\nInput >> ");
    scanf("%20s", &file_name[0]);

    fopen_s(&file, file_name, "w");

    if( file == NULL )
    {
        fclose(file);
        return EXIT_FAILURE;
    }
    else
    {
        system("cls");
        printf("Type in ciphertext tag \nInput >> ");
        scanf("%10s", &cipher_out.tag);
        
        for(u8 I = 0; I < MAX_SIZE; ++I) cipher_out.text[I] = dataOUT[I];

        fprintf_s(file, OUT_FORMAT, cipher_out.tag, cipher_out.text);
        fclose(file);

        return EXIT_SUCCESS;
    }

    return EXIT_UNKNWN;
}

static u8 readFile(u8* dataIN)
{
    system("cls");

    u8 file_name[20] = {0};
    ciphertext cipher_in;
    FILE* file;

    printf( "Type in file name with extension to retrive input (e.g. .txt .dat... )\nInput >> ");
    scanf("%20s", &file_name[0]);

    fopen_s(&file, file_name, "r");

    if( file == NULL )
    {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    else
    {
        fseek(file, 0, SEEK_SET);
        fscanf_s(file, IN_FORMAT, cipher_in.tag, cipher_in.text);

        for(u8 I = 0; I < MAX_SIZE; ++I) dataIN[I] = cipher_in.text[I];

        printf("Tag >> ");
        for(u8 I = 0; I < 10; ++I)
        {
            printf(" %c ", cipher_in.tag[I]);
        };

        printf("\nciphertext >> ");
        for(u8 I = 0; I < MAX_SIZE; ++I)
        {
            printf(" %c ", cipher_in.text[I]);
        };

        getch();
        fclose(file);
        return EXIT_SUCCESS;

    }
    return EXIT_UNKNWN;
}
