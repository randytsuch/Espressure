/*
Utilities for files
From SD Test sample program

Contains these routines:

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){

void createDir(fs::FS &fs, const char * path){

void removeDir(fs::FS &fs, const char * path){

void readFile(fs::FS &fs, const char * path){

void readCSVFile(fs::FS &fs, const char * path){

void writeFile(fs::FS &fs, const char * path, const char * message){

void appendFile(fs::FS &fs, const char * path, const char * message){

void renameFile(fs::FS &fs, const char * path1, const char * path2){

void deleteFile(fs::FS &fs, const char * path){

void testFileIO(fs::FS &fs, const char * path){



Usage

    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    listDir(SD, "/", 0);
    createDir(SD, "/mydir");
    listDir(SD, "/", 0);
    removeDir(SD, "/mydir");
    listDir(SD, "/", 2);
    writeFile(SD, "/hello.txt", "Hello ");
    appendFile(SD, "/hello.txt", "World!\n");
    readFile(SD, "/hello.txt");
    deleteFile(SD, "/foo.txt");
    renameFile(SD, "/hello.txt", "/foo.txt");
    readFile(SD, "/foo.txt");
    testFileIO(SD, "/test.txt");
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

From SPIFFS Test\
\
void setup()\{\
    Serial.begin(115200);\
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))\{\
        Serial.println("SPIFFS Mount Failed");\
        return;\
    \}\
    
    listDir(SPIFFS, "/", 0);
    writeFile(SPIFFS, "/hello.txt", "Hello ");
    appendFile(SPIFFS, "/hello.txt", "World!\\r\\n");
    readFile(SPIFFS, "/hello.txt");
    renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    readFile(SPIFFS, "/foo.txt");
    deleteFile(SPIFFS, "/foo.txt");
    testFileIO(SPIFFS, "/test.txt");
    deleteFile(SPIFFS, "/test.txt");
    Serial.println( "Test complete" );
*/

//*************************************************************
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed open dir");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}


//*************************************************************
void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}


//*************************************************************
void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}


//*************************************************************
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Read open file err");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

//*****************************************************************************

  size_t n;      // Length of returned field with delimiter.
  char str[10];   // Must hold longest field with delimiter and zero byte.
  char *ptr;     // Test for valid field.


void parseData() {

//first copy the row of data from 2 dimensional array into its own array for processing
  int i = 1;     // row index.
  int j = 1;     // column index
  int k = 1;
  int localint = 0;
  int tempint = 0;
  int localcnt=0;
  
 // Serial.print("Row Array ");
  while (j<= SHOT_PARAM_SIZE) {
    ReadRow[j] = ShotFilearray [i][j];
 //   Serial.print(ReadRow[j]);
    j++;
  }
  
  Serial.println("Row Array ");
  j=1;
  while (j<= SHOT_PARAM_SIZE) {
    char ch=ReadRow[j++];
    //Serial.print(ch); 
    //Serial.print(" ");

    if (ch == ',') {  //look for commas, they seperate the parameters
      tempint = 0;
      Serial.print("*");
      Serial.println(localint,DEC); 
      switch (localcnt) {
        case 1:
          ShotIndex = localint;
          break;
        case 2:
          break;
        default:
          break;  
      }    
      localcnt++;  //counter for parameters found
    }
     else {
      localint = ch - '0';
      tempint = tempint*10;
      localint = tempint+localint;
      tempint=localint;
      Serial.print("/");
      Serial.println(localint,DEC); 
    }
    //else str[k++]=ch;
  }    
  
 /* char *localstrings[SHOT_PARAM_SIZE];
  char *localptr = NULL;

  byte index = 0;
  Serial.print("Row Array ");
  localptr = strtok(ReadRow, ",");  // 
  localint = atoi(ReadRow);
  Serial.print(localint);
    while(localptr != NULL)
    {
        localstrings[index] = localptr;
        index++;
        localptr = strtok(NULL, ",");  // 
//        localint = atoi(localstrings);
        Serial.print(localint); 
    }*/

}

//*************************************************************
void readShotFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Read Shot open file err");
        return;
    }
    
    int i = 0;     // row index.
    int j = 0;     // column index
    Serial.print("Shot read from file: ");
    while(file.available()){
        int c = file.read();
        ShotFilearray[i][j]=c;
        Serial.print(ShotFilearray[i][j]);
        if (ShotFilearray[i][j]== '\n') {   //check for new line, if new line then go to next line
          i++;
          j=0;
          }
        else if (j > SHOT_PARAM_SIZE) {   //check to see if column index exceeds line size before new line, if exceeds return
          return;
          }
        j++;
      }
    file.close();
}


//*************************************************************
void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Write open file err");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}


//*************************************************************
void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}


//*************************************************************
void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}


//*************************************************************
void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}


//*************************************************************
void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}
