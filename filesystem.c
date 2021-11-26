/**************************************************************************************
 * @file    filesystem.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   program body containing write and read routines destined to store and 
 *          retrieve data from cipher sheets.
 **************************************************************************************
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 * ************************************************************************************
*/

const char* CIPHERTEXT_OUTPUT_FORMAT = "%s: %s\n\r";
const char* CIPHERTEXT_INPUT_FORMAT = "%[^:]: %[^]";

typedef struct ciphertext
{
    char filename[20];
    char tag[10];
    char text[64];  
          
}ciphertext;

static u8 writeFile(u8* dataOUT)
{
    FILE *filePtr;

    ciphertext CTO; 
    u8 I = 0;

    system("cls");
    printf("Enter filename.\nInput >> ");
    scanf("%s", CTO.filename);
   
    fopen_s(&filePtr, CTO.filename, "a+");

    if(filePtr == NULL)
    {
        fclose(filePtr);
        return EXIT_FAILURE;
    } 
    else
    {
        system("cls");
        printf("Enter cipher tag.\nInput >> ");
        scanf("%s", CTO.tag);

        while( I < 4 * N_KEY * N_BLOCKS )
        {
            CTO.text[I] = dataOUT[I];
            I++;
        }

        fprintf_s(filePtr, CIPHERTEXT_OUTPUT_FORMAT, CTO.tag, CTO.text);
        fclose(filePtr);
        return EXIT_SUCCESS;
    }

    return EXIT_UNKNWN;
}

static u8 readFile(u8* dataIN)
{
    FILE* filePtr;

    ciphertext CTI;

    system("cls");
    printf("Enter filename.\nInput >> ");
    scanf("%s", CTI.filename);

    fopen_s(&filePtr, CTI.filename, "r");

    if(filePtr == NULL)
    {
        fclose(filePtr);
        return EXIT_FAILURE;
    }
    else
    {
        system("cls");
        printf("Enter cipher tag.\nInput >> ");
        scanf("%s", CTI.tag);
                                        
        fseek(filePtr, 0, SEEK_SET);
        
        /* @todo implement a function to search for input tag inside ciphersheet and scan the written cipher text */
        return EXIT_SUCCESS;
    }
    
    return EXIT_UNKNWN;
}
