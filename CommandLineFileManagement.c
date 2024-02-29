#include<stdio.h> 
#include<stdlib.h> //needed for exit(0);
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//This is a method of type const char*, as we want to return the string filename, to be used further on in the program
//Objective of this method is to allow the user to create a new file, and if a existing file exists with that name, then the new file should not be created, and I developed my program to be more advanced by not allowing files to be overwritten.

const char* createFile() { //previously, limitation was that overwrite existing files, now a new file will not be created. 
    
    FILE* fp; // The file pointer called 'fp' is pointing to a memory location holding a FILE structure. Holds reference to our file 
    char filename[255]; //Creates an array called 'filename' of data type 'char'. The size of this array is 255 bytes.
    
    
    printf("Enter Name of File For Creation \n"); //Prompts user to input the name they want to assign for thier file. 'printf' sends this formatted command .to the screen
    scanf("%[^\n]s", filename); //scanf used to take input from the user. Reads formatted input from standard inputs, in this case a keyboard.
                        //The %s format specifier means only string input can be taken, and this input stored in the array 'filename'
                        //The [^\n is important as it allows for data to be carry on being read when whitespace is encountered
    fp = fopen(filename, "r"); //Need to open in read mode first, as else without it, when user enters name of a new file to be created, a segmentation fault occurs, as program thinks file with this name already exists.
                                //... and only allows the names of actual existing files to be inputted.
    
    if (fp != NULL) {  //If file already exists, no file will be created.
        printf("File Not Created. Existing File With Name : %s \n", filename); //Outputs that file cannot be created
    } 
    
    else{ //else statement if conditional if statement above is not met
        fp = fopen(filename, "w");  //new empty file created. Normally if a file with the same name already exists, it's contents is erased, and this file is a new empty file
                                    //...but due to the code opening the file in read first, the new file will not be overwritten. Very important, as means user doesn't lose important content due to poor program structure
        printf("File Created \n");
    }
    fclose(fp); //have to close the file, returns 0 on success

return "filename"; //as function type is a const char*, have to return the string 'filename'. else error.
}


//Method to remove a specific file the user enters. Of type 'int', as the remove function returns an integer

int removeFile() {
    char filename[255]; //Creates an array called 'filename' of data type 'char'. The size of this array is 255 bytes.
    int delete; //have to initiaise variable name, that removal will be assigned to.
    
    printf("Enter Name of File you want to Remove "); 
    scanf("%s", filename);
    delete = remove(filename); // remove() function used to delete a file, so that it is no longer accessible
    
    if(delete == 0) {  // On successfull deletion, 0 is returned
      printf("File has been successfully deleted");
   } else { //on error, -1 is returned, but I used an else statement as it is more concise and cleaner than an else if.
      printf("Error: unable to delete the file");
   }
}

//method to allow user to enter the name of a existing file, and it's contents are copied to a new file, with a new name
//method is of type FILE*, as file poninter is returned. 
FILE* copyFile() {
    FILE *fp1, *fp2; // Two different file pointers needed rather than one, to point to different locations holding file structures
    char filename[255], content; //Array of filename of type char, but also creates a variable called content of type char.
                                //Initialised in same line for effiency.
  
    printf("Enter Name of File you want to Copy Contents From \n"); 
    scanf("%s", filename); 
  
    
    fp1 = fopen(filename, "r"); //first file opened for reading contents
    if (fp1 == NULL) { //Error handling, if file doe not exist, cannot be opened and contents cannot be read from
        printf("File Cannot be Opened %s \n", filename); 
        exit(0); //Program terminated if file cannot be found, as it is needless for the user to carry on and enter the second file, because contents from first file can clearly not be copied, as first file does not exist.
    } 
  
    printf("Enter Name of File you want to Copy Contents To \n"); 
    scanf("%s", filename); 
  
    //Code is written in a way so that regardless of user entering a completely new file name, or a file that already exists, content from file 1 will always be copied to it, meaning I have allowed overwriting, 
    //This is because the user has already decided to copy the original code, so in my opinion for my program , it is more important that the content is copied from file1 , rather than possible content from file2 being overwritten.
    fp2 = fopen(filename, "w"); //Opens second file to write to, in write mode to ensure it is blank initially.
    if (fp2 == NULL) { 
        printf("File Cannot be Opened %s \n", filename); 
        exit(0); 
    } 
  
    //Used to read contents of the file
     //fgetc() will get the character from the inputted filename referenced by fp1. The internal file position indicator is then moved on to the next character
    //Setting every character obtained from file1 using fgetc() into one variable called content

    while ((content = fgetc(fp1))!= EOF) {  //while loop, condition is that as long the end of the file has not been rearched, or in case of any error,  (EOF)
        fputc(content, fp2); //Will write the content to the output stream, which is referenced by fp2.
         //repeated line as there is a while loop, which will run until while condition is no longer met.
    } 
  

   
    printf("Content has been Copied to %s \n ", filename); 
    fclose(fp1); //files are closed.
    fclose(fp2); 

return fp1, fp2; 
}

//end of each operation, make call to the changelog function, which 
//changelog function also takes parameter of filename, so it can find the number of lines, all this written to change log


//method to display the contents of a file. The method is of type FIle*. 
//Entirety of Code has been recycled from above functions; no new code to explain here
FILE* showFile() {
    FILE *fp; 
    char filename[255], content; 
    printf("Enter Name of File you want to Display \n"); 
    scanf("%s", filename); 
  
    fp = fopen(filename, "r"); // Open file 
    if (fp == NULL) { 
        printf("File Cannot be Found \n"); 
    } 

    //read and print file contents to screen
      //difference between fgetc and getc is that fgetc can be implemnted as a macro, getc cannot.
        //Effect os this is that fgetc() will always be a function, so it's address can be taken. This address can be used as the argument if needed to passed to another function, which is useful
        //But calling fgetc() is longer than getc(), as calling a function takes more time
    while ((content = fgetc(fp)) != EOF) { 
        printf ("%c", content); //print the contents to screen, %c for type char
    } 
    fclose(fp); 

return fp;
}

//method that allows the user to enter a number for how many lines they want to append to any specific file, and content for each line is appended to the file

FILE* appendLine() { // Originally, the appended line not showing in terminal, only one word allowed, characters after spaces not considered

    FILE * fp;
    //initialsing the variables
    char filename[255], content;
    char newData[255]; //array for the new content that will be appended to the file
    int i,noOfLines;
  
    printf("Enter Name of File you want to Append To");
	scanf("%s",filename);	

    //repeated error handling
    fp = fopen(filename, "r"); 
    if (fp == NULL) { 
        printf("File Cannot be Found \n"); 
        exit(0); //plan was to take program back to main menu here but could not find a way without implicit declaration of function errors, so I decided to make program exit instead

    } 
	
    fp = fopen(filename, "a"); //Using fopen in append mode instead of write, so the file is not overwritten 
    
    printf(" Enter the Number of Lines you want to Append");
    scanf("%d", &noOfLines); //scanf used to take input from user for noOfLines. The %d is for type integer, as the user should only be able to append a whole integer number of lines.
                            //The & is needed before the variable, to ensure that the value of this variable can be changed
                            //It means taking the place in memory where the variable is stored (address), rather than the actual variable
    
    for(i = 0; i < noOfLines+1;i++) { // for loop that runs x times if the user wants to add x number of lines. 
        fgets(newData, sizeof newData, stdin); 
        // fgets() used to read text from keyboard. newData is the char array. sizeof newData is the amount of text to enter, which has to be the same size as the char array
        // The standard input device is called stdin, as defined in the header file #include<stdio.h> 
        
        fputs(newData, fp); // Prints a string to the file, in this case it is each line. fputs() accepts two arguments, pointer to string and file pointer
    }
    fclose (fp); //closes file

    
    //recycled code that prints the content to screen
    //content = fgetc(fp);  
    //while (content != EOF) { 
       // printf ("%c", content); 
       // content = fgetc(fp); 
   // } 
   // fclose(fp); 

    while ((content = fgetc(fp)) != EOF) { 
        printf ("%c", content); //print the contents to screen, %c for type char
    } 
    fclose(fp); 
	

 return fp;

}

//A method to delete a specific line from a specific file. Function is of type FILE* as file pointers are returned
//Limitation is that the specific file must have a blank line as the first line for method to work as intended
//If first line is not a blank line, the first line will also be deleted, along with the specific line number

FILE* deleteLine() {
    FILE *fp1, *fp2;
    //initialising variables
    char filename[255], content;
    int a = 1, lineNumber;
 
    printf("Enter Name of File");
    scanf("%s", filename);
    
    //Error handling. program is exited if file name entered does not match to a existing file
    fp1 = fopen(filename, "r");
    if (fp1 == NULL)  { 
        printf("File Cannot be Found"); 
        exit(0);
    } 
    
    //code to print content to the screen
    while ((content = fgetc(fp1))!= EOF) {
        printf("%c", content);
    }
    
    rewind(fp1); // The rewind function sets the file pointer to the beginning of the file
    
    printf(" \n Enter the Line Number to be Deleted");
    scanf("%d", &lineNumber); //scans for the line Number
    
    fp2 = fopen("temporary.c", "w"); //creates a new temporary file, where the contents will be copied
    content = fgetc(fp1);
    while (content != EOF) {
        content = fgetc(fp1);
        if (content == '\n') 
            a++; //increment a when a new line is reached, Funtil it reaches the line numbet that the user wanted to delete
            if (a != lineNumber) {
                // All lines in the temporary file will be copied except for the line that is ordered to be deleted.
                putc(content, fp2); // putc() writes the content to the file.
            }
    }
    
    fclose(fp1);
    fclose(fp2);
    remove(filename); //remove function used again to delete the old original file
    
    rename("temporary.c", filename);  //rename the temporary to the old original file, to ensure consistency between start and final file name.
    printf("\n Contents of File After Deletion\n");
    
    //prints the new updated contents of the file to screen
    fp1 = fopen(filename, "r");
     while ((content = fgetc(fp1)) != EOF) { 
        printf ("%c", content); //print the contents to screen, %c for type char
    } 
    fclose(fp1); 
	
    
return fp1,fp2;
}

//A method to read and print a specific line to screen of any file, bsed on the users line Number input
//type FILE* again, as file pointer returned

FILE* readLine() {
    
FILE *fp;
    //initialising variables
    int lineNumber, i, last ; 
    //variable last stores whether end of file has been reached or not.
    //lineNumber used for user to input line number to be read
    //i used as a loop, is incremented

    char content[255], filename[255]; //Creating the char arrays for file name input, and the line content from the file

    printf("Enter Name Of File");
    scanf("%s", filename);
    fp = fopen(filename, "r"); 
    if (fp == NULL) {
        printf("File Not Found \n");
    }

    printf("Enter Line Number you want Read");
    scanf("%d", &lineNumber);

    //iterate through every line, as long as last = 0 , meaning the end of file hasn't been reached. Start from i = 0, and increment i until the entered line number is reached
    for(last = i = 0;i<lineNumber;i++){
        if(0==fgets(content, sizeof(content), fp)) { // fgets() used to read text from the file pointer when the specific line is reached, and prints it to screen. Content is the char array. sizeof content is the amount of text to be read, which has to be the same size as the char array
                                                        //fp also taken as argument, is the file pointer
            last = 1; 
            // Set last = 1 if end of file has been reached, (EOF = 1)
            break; //break to end the for loop.
        }
    }
     
    // if no errors and end of file has not been reached and string is found from line, then print it to screen
    if(!last) {
        printf("\n Line %d: %s\n", lineNumber, content);
        fclose(fp);
    }

    return fp;
}

//This function allows the user to insert content anywhere in a given file, given the exact line number and the index position
//I decided to function my prorgram in this way because it adds a completely new feature to the list of options. The normal feature of inserting a new line of content onto a blank like has very similar outcomes to appending a new line of content
//But In my method, the user cannot insert content onto a blank line, but however has the ability to locate the exact location, including index number let alone line number.
//ALso cannot insert on the first line, line1, which is a potential issue the user should be aware of
//This way of pinpointing exactly where data goes is highly useful, and it promotes precision
//method is of type FILE*

FILE* insertLine(){
    FILE *fp1,*fp2;
    long location = 0; //Used to store file's current position from the beginning of the file as a difference of certain number of bytes 
    int lineNumber,index,nl=0;
    //line Number to store the specific line number the user wants to insert at. 
    //index is the specific horizontal position where content is inserted in that line
    //n1 used as a value to be incremented
    int lineLength = 0; //variable to store the number of characters on a line
    
    char *filename = malloc(100), *text = malloc(100), content, content2;
    //mallocs used as a way of allocating memory manually at run time aka dynamic memory.
    //Two block of memorys each of 100 bytes are reserved. The pointers filename and text each hold the address of the first byte in both respective mallocs
    //Decided to test using mallocs for this function only as is one of the more complex functions
    //content1 stores the contents from file 1 originally, and content2 stores the new file contents with the inserted content.
    

    printf("Enter File Name for Insertion");
    scanf("%s",filename);
  
    if (access(filename,F_OK)!=-1) { //had to use access() method rather than the standard is file pointer null code, as I encountered several issues trying to implement the latter.
        
        //Prints current file contents to screen
        fp1 = fopen(filename, "r");
        printf("\n File Before Insertion \n");
        
        content = fgetc(fp1);
        while (content != EOF) {
            printf("%c", content);
            content = fgetc(fp1);
        }
            
        fclose(fp1);
        
        fp1 = fopen(filename, "r"); //have to reopen else will not be printed to screen.
        fp2 = fopen("temporary.txt", "w"); //creating a secondary temporary file where the contents will be copied, will make edits there.


        printf("\n Enter Text\n");
        scanf("%s",text); //Scanning for the text the user intends to input
        printf("Enter the Line Number you would like to Insert on (except line 1 or empty lines) ");
        scanf("%d", &lineNumber); //scanning for the specific line number

        while((content=fgetc(fp1))!=EOF) { //file1 contents copied to the temporary file2
            fputc(content,fp2); 
        
            if(content == '\n' && ++nl == lineNumber-1){ //lineNumber - 1 used as without it I have found the line Number scanned will be one above the intended, so the -1 corrects this
                location = ftell (fp1);//position of file pointer is found with respect to beginning of the file
                while (( content = fgetc (fp1)) != '\n' && content != EOF) {
                    lineLength++; //the count of number of characters on the line is incremented
                }
                
                if (lineLength == 0){ // purpose of this was so if content was to be inserted onto a empty line, do not allow it 
                     exit(0);        //However, program is not actually exited, this is a flaw
                }   //but this design structure is still useful, as it means it avoids an infinite loop of program printing out index has to be less than 0, and the user never being able to enter anything that allows them to leave.
                    //This would occur, as on a new line there is no content, so linelength is 0, and of couse index is always 0 on a new line, so if these 2 are equal content cannot be inserted, and the program would be stuck in an infinite while loop, but because of this code it isn't
                
                else{
                    fseek ( fp1,location,SEEK_SET );//seek  back to start of line
                    //get index where to insert text

                
                do { 
                    printf("\n State Index Number (Has to be Smaller Than %d) :\n", lineLength); //asking user to enter a index number smaller than the line length
                    if ( ( scanf("%d", &index)) != 1) {
                        scanf ("%*[^\n]");// Clears buffer if input entered is not a integer
                        index = lineLength; //set index = line length
                    }
                } while ( index >= lineLength || index < 0); //do while loop to run if the position where the user wants the character to be inserted is more than the actual number of characters in the whole line, or if index is a negative number (impossible to insert in a negative position)
                
                while (index) { //while (variable) means loop is exited when the null character found at end    
                            content = fgetc ( fp1);
                            fputc(content,fp2);
                            index--; //decrement index after copying contents
                }
                fprintf(fp2, "%s ", text);// Inserted text added
            }
            }
        }
                
        printf("\n Content after Insertion \n");
        fclose(fp1);
        fclose(fp2);

        fp1 = fopen(filename, "w");
        fp2 = fopen("temporary.txt", "r");
        
        while((content2=fgetc(fp2))!=EOF){
            fputc(content2,fp1); //recycled code to copy content
        }   
                
        fclose(fp2); //order of closing these files matter, as else file won't be written to change log
        fclose(fp1);
        remove("temporary.txt"); //temporary file now deleted

        //new file contents with inserted content printed to the screen
        fp1 = fopen(filename, "r");
        while ((content = getc(fp1))  != EOF) {
            printf("%c", content); //
        }
        fclose(fp1);
        
        free(filename); //Must deallocate returned pointer, to avoid a memory leak. 
        free(text);
        }

    else {
        printf("Error. File Not Found");
        exit(0);
    }

    return fp1,fp2;
}

//This is a method to merge the contents of 2 files, and store this in a 3rd file for use, and delete the existing old files
//returns file pointers, so function is of type FILE*
//Justification for Usefulness covered in seperate write up.
//Improvements, error handling for if 2 files are equal. 
FILE* mergeFiles(){ 
    FILE *fp1, *fp2, *fp3; //3 different file pointers needed for 3 different memory locations

    char filename1[255], filename2[255], filename3[255], content;
    //initialsing the variable content and the arrays to store filename

    //Two files opened in read mode
    printf("Enter One Name of File you want to Merge \n"); 
    scanf("%s", filename1); 
    fp1 = fopen(filename1, "r"); 

    //Error Handling to check files can be found
    if (fp1 == NULL) {
        printf("File Cannot be Opened \n");
        exit(0);
    }
    
    printf("Enter Second Name of File for Merging \n"); 
    scanf("%s", filename2); 
    fp2 = fopen(filename2, "r");

    if (fp2 == NULL) {
        printf("File Cannot be Opened \n");
    }
    

    printf("Enter Name for new Merged File \n"); 
    scanf("%s", filename3); 
    fp3 = fopen(filename3, "w"); //third file will be opened in write mode to ensure it is completely blank
    
    if (fp3 == NULL) {
        printf("File Cannot be Opened \n");
    }


   while ((content = fgetc(fp1)) != EOF){ // FIle 1's contents read character by character and copied to the 3rd file
      fputc(content, fp3); 
   }
    
    while ((content = fgetc(fp2)) != EOF){ // FIle 2's contents read character by character and copied to the 3rd file
      fputc(content, fp3);
    }
    
    printf("Files have been Merged\n"); 

    remove(filename1);
    remove(filename2); //deletes the old files after contents have been merged, no longer needed
   
    fclose(fp1); 
    fclose(fp2); 
    fclose(fp3); 


   return fp1,fp2,fp3;
}

//A function that compares 2 files, and returns the exact number of differences, and the position of each difference between files
//Function of type int, as the number of differences returned is a integer value
int compareFile(){
    FILE *fp1, *fp2;
    char filename1[255], filename2[255], content1, content2;
  
    int differences = 0, location = 0, lineNumber = 1; 
    //lineNumber stores the current line being error checked
    //differences stores the actual number of differences between the two files
    //location stores the index position of errors on the line

    printf("Enter Name of File for Comparison \n"); 
    scanf("%s", filename1); 
    fp1 = fopen(filename1, "r");
    //Error Handling
    if (fp1 == NULL) {
        printf("File Cannot be Opened for Comparison \n");
        exit(0);
    }

    printf("Enter another Name of File for Comparison \n"); 
    scanf("%s", filename2); 
    fp2 = fopen(filename2, "r");

    if (fp2 == NULL) {
        printf("File Cannot be Opened for Comparison \n"); 
        exit(0); //if  file 2 is not found
    }

    content1 = fgetc(fp1);  //Content fetched until EOF
    content2 = fgetc(fp2); 
  
    while (content1 != EOF && content2 != EOF) { // iteratative loop that runs until the end of file (or an unxpected error)
        location++; //increment the location variable, so keep on moving the location index to the next index to check for error on adjacent index.
  
        
        if (content1 == '\n' && content2 == '\n') { //If there is no more content on a line, and content moves to a new line on both content1 and content2, then increment lineNumber value to new line, and reset location index to 0.
            lineNumber++; 
            location = 0; 
        } 
  
        if (content1 != content2) {   // If at any time the contents from both files are not equal, then increment the difference variable
            differences++; 
            printf("Line Number : %d \tDifference Position : %d \n", lineNumber, location); //output every difference, with the location index and line Number so it can be found easily
        } 
        
        
        content1 = fgetc(fp1); 
        content2 = fgetc(fp2); 
    } 
  
    printf("Final Count of Differences. %d ", differences); 
    fclose(fp1); 
    fclose(fp2);

return differences;
}

//method is called after every option, and user prompted to input filename again, in order to record the resultant Number of lines after each operation is complete in ChangeLog.txt
int noOfLines() { 
    FILE *fp; 
    int lineNumber = 0;  //initialise lineNumber as equal to 0
    char filename[255], content;
    // To store a character read from file 
  
    // Get file name from user. The file should be 
    // either in current folder or complete path should be provided 
    printf("\n You Must Enter File Name (Again) to Store Line Count in ChangeLog.txt \n"); 
    scanf("%s", filename); 
    
    //Error Handling
    fp = fopen(filename, "r"); // Open the file 
    if (fp == NULL) { // Check if file exists 
        printf("File Does not Exist \n "); 
        exit(0); //exit program if file does not exist. Needed, as without it segmentation fault occurs
        
    } 
    
    for (content = getc(fp); content != EOF; content = getc(fp)) { // Content extracted from file and stored in variable char content
        if (content == '\n') {   
            lineNumber++; // Count incremented if content is on newline 
        }
    }
    
    fclose(fp); // Close the file 
    printf("File %s has %d lines \n ", filename, lineNumber); 
  
  return lineNumber;
}

//basic function that asks for the filename for the user. 
//This is needed as we want to write the specific filename to Changelog.txt, so the user can check exactly what specific files the operations were operated on. (also check the number of lines)
const char* fileName(){
    FILE *fp; 
    char *filename;
    printf("\n Please Enter File Name (Again) to Store File Name in Changelog.txt \n"); 
    scanf("%s", filename); 
  
    fp = fopen(filename, "r"); 
    if (fp == NULL) { 
        printf(" Invalid File Name\n");
        exit(0); //exit needed as else segmentation fault occurs here
        
}
    
return filename; //as function is const char*, must return a string. Variable filename has to be returned for later use
}

int main() {
    
    FILE *fp = NULL;
    fp = fopen("ChangeLog.txt" ,"a"); // Creates / open the Change Log file, in append mode
                                    //Append used so that new data can be added to the end. Originally write was used, which is a problem as data is wiped. 
                                    //Thought write was more useful initially, as then the user could see all exact operations completed in one aingular boot of the program, and then reset upon another boot
                                    //But if the same file is operated on in multiple boot ups, then a whole history of it's operations since creation is much more beneficial than data being wiped, so 'a' is more useful thsn 'w'
    if (fp == NULL) {
        printf("Error opening file!\n");
    }
    int option, content; 
    //option used for the user to choose the different operations they want to carry out
    //content is for the program to be able to output the contents of ChangeLog.txt. Has to be defined here, cannot define variable in switch case
    char term; //used in the menu system below

  //Created a menu system. Do while loop ensures that as long as the user doesn't enter the option to exit the program, the program will constantly carry on.
    // Very important feature I added, as it means the program will not be exited after each operation, causing inconvienience, but in fact the user will be taken back to menu after each successful operation, to choose another option
    do{
        printf("\n Enter an option. \n 1 = Create file \n 2 = Delete file \n 3 = Copy File \n 4 = Show File \n 5 = Append Line \n 6 = Delete Line \n 7 = Read Line \n 8 = Insert Line \n 9 = Count Number of Lines \n 10 = Merge Files \n 11 = Compare Files \n 12 = Print ChangeLog.txt \n 13 = Quit Program \n");
        //Use of \n after each option so each choice is on a new line, creates a cleaner interface

        if(scanf("%d%c", &option, &term) != 2 || term != '\n')
            exit(0); //However, due to the do while loop, the program was infintely looping without stop, so had to make program force exit if the input was not an integer.
                    //If an an integer greater than 13 entered, the user is prompted to enter again, until a correct input is registered
                    // previously, if nothing was entered, then infinite while loop was encountered, not good but this if statement fixed it
        else{
            printf("You chose option %d", option); //initially tried to use fgets, error of expected char restrict, so switched to scanf    
            printf("\n");
        }
        
        //switch case evaluates the option, and compares it with the values of each case label
        //decided not to include default case, as nested in a do while loop, so if incorrect option is entered, program will naturally carry on (and loop)
        switch(option) { 
            case 1 :
                createFile(); //calls the main function for operation
                int a = {noOfLines()}; //initialsing a variable to call the noOfLines method, and the returned output of noOflines function will be assigned to variable 'a'.
                                        //int as noOflines returns a integer. 
                                        //has to go after the createFile() method, as else get error of label can only be part of a statement and a declaration is not a statement
                const char* filea = fileName(); //initialise variable to call filename method, and store the returned output of this into the const char* variable
                
                //Calling these functions everytime is needed in order for the specific file name and specific no of lines to be written to ChangeLog.txt, as else a generic 'File is Created' would have to be writted, which is not detailed and informative in the slightest
                //I acknowledge this is potentially a inefficient way to write to Change log, but it does give full information.
                
                //There is error handling within the above functions, if a non existant file name is entered, program will exit
                //But if a name of another existing file is entered when fileName() is called , which is different to the name the file created, the file written to ChangeLog will be incorrect and show a different file to the one you created, and will have a different number of Lines. 
                //This is a flaw of this design, as if the user enters a different filename to the one they created/appended to etc. ,but this filename also exists, the ChangeLog will be incorrect. 

                fprintf(fp,"File Called %s\n",filea); 
                fprintf(fp,"---- Created with Number of lines %d\n",a);
                break;
                
            case 2 : {
                const char* fileb = fileName();//had a problem where I did case '2' instead of 2, so program thought I was using char, so switch case statement did not run
                removeFile();
                fprintf(fp,"File Called %s\n",fileb);
                fprintf(fp,"---- Was Deleted\n");
                //With my system, calling fileName() to input filename to store in changelog, does not work here, because after deletion, file nolonger exists, so filename cannot find the existing file to write to ChangeLog.txt
                //so have to call filename function first, which can cause confusion to user, and is inefficient, but does work
               
                break; //break statement needed as else code for each case will not run 
            }
            case 3:
                copyFile();
                int c = {noOfLines()}; //Same structure used for most cases in order to write to ChangeLog.txt
                const char* filec = fileName();
                fprintf(fp,"File Called %s\n",filec); 
                fprintf(fp,"---- Copied, has Number Of Lines %d\n",c); //problem is you can add either the file read or new file to the changelog, so ideally want ony the new file in change log to reflect on new changes can be seen as a problem, but gives the user choice
                break;
            
            case 4:
                showFile();
                int d = {noOfLines()}; 
                const char* filed = fileName();
                fprintf(fp,"File Called %s\n",filed);
                fprintf(fp,"---- Displayed, has Number Of Lines %d\n",d);
                break;
            
            case 5:
                appendLine();
                int e = {noOfLines()};
                const char* filee = fileName();
                fprintf(fp,"File Called %s\n",filee);
                fprintf(fp,"---- was Appended to , now has Number of lines %d\n",e);
                break;

            case 6:
                deleteLine();
                int f = {noOfLines()};
                const char* filef = fileName();
                fprintf(fp,"File Called %s\n",filef);
                fprintf(fp,"---- had a Line Deleted, now has Number of lines %d\n",f);
                break;


            case 7:
                readLine();
                int g = {noOfLines()};
                const char* fileg = fileName();
                fprintf(fp,"File Called %s\n",fileg);
                fprintf(fp,"---- had Line Read, has Number of lines %d\n",g);
                break;
            
            case 8:
                insertLine();
                int h = {noOfLines()};
                const char* fileh = fileName();
                fprintf(fp,"File Called %s\n",fileh);
                fprintf(fp,"---- had Content Inserted, File now has Number of lines %d\n",h);
                break;
            
            case 9: { //brackets are needed here, as else encounter error of label can only be part of a statement and a declaration is not a statement
                int j = {noOfLines()};
                const char* filej = fileName();
                fprintf(fp,"File Called %s\n",filej);
                fprintf(fp,"---- had Number of Lines counted. Count =  %d\n",j);
                break;
            }

            case 10: { //brackets are needed here, as else encounter error of label can only be part of a statement and a declaration is not a statement
                mergeFiles();
                int k = {noOfLines()};
                const char* filek = fileName(); //Enter name of Merged File here, not original files, as they will be deleted
                fprintf(fp,"File Called %s\n",filek);;
                fprintf(fp,"---- was created by Merging. New Merged File has Number of Lines =  %d\n",k);
                break;
            }

            case 11: { //
                int n = compareFile(); 
                fprintf(fp,"Files were Compared. Number of Differences = %d \n ",n); //Could not find a way to write both files to ChangeLog, and it was aimless to only include one, therefore there is a complete lack of filenames here
                break;
            }

            case 12:
                //gives option to print Change Log to screen
                printf("CHANGE LOG.TXT \n ");
                fp = fopen("ChangeLog.txt" ,"r");
                while ((content = fgetc(fp)) != EOF) { 
                    printf ("%c", content); 
                } 
                fclose(fp); 
            
            //A flaw with the way Change log is designed is that if the user fails to enter the same name as the one they entered when forming the original operation, then nothing will be written to ChangeLog.txt, even if the operation was succesfuly completed.
            //This can be seen as a positive if the user is aware of this beforehand however, because if the user does not want to track the file changes briefly for some instance, they can enter an invalid file name in order to not write to Change log.
            
            case 13:
                exit(0); //if option 13 is entered, then program will exit
                        //program also exit if non integer is entered

                
        }
            
    } while (option != 13); //iterate until user chooses option on menu to leave program


}