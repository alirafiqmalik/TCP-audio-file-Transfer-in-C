#include <stdio.h>

int BUFFER_SIZE = 1024;
FILE *source;
FILE *destination;
int n;
int count = 0;
int written = 0;


int main() {

  unsigned char buffer[BUFFER_SIZE];

    source = fopen("audio/audio1.m4a", "rb");

    if (source) {
        destination = fopen("tmpout/tmp.m4a", "wb");
        int counti=0;
        while (!feof(source)) {
            n = fread(buffer, 1, BUFFER_SIZE, source);
            count += n;
            printf("n = %d\n", counti);
            fwrite(buffer, 1, n, destination);
            counti+=1;
        }
        printf("%d bytes read from library.\n", count);
    } else {
        printf("fail\n");
    }

    fclose(source);
    fclose(destination);

  return 0;
}