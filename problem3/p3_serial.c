#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int errorFlag = 0; 
typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
}pixel; 

void handle_comments(FILE *in){
    char c = fgetc(in);
    char buf[1024]; 
    if(c=='#'){
        fgets(buf, 1024, in); 
    }else{
        ungetc(c,in);
    }
}

pixel * flip(int width, int height, pixel * src){
    pixel * dest = (malloc)(sizeof(pixel) * width* height);
    for(int i = 0; i < height; i++){
        for(int j = 0 ; j < width; j++){
            int index = width - 1 - j; 
            dest[i*width+j].R = src[i*width+index].R;
            dest[i*width+j].G = src[i*width+index].G;
            dest[i*width+j].B = src[i*width+index].B;
        }
    } 
    return dest; 
}

pixel * smooth (int width, int height, pixel * src){
    pixel * dest = (malloc)(sizeof(pixel) * width* height);
    double r,g,b;
    for(int i = 0; i < height; i++){
        for(int j = 0 ; j < width; j++){
            b = 0; g = 0; r = 0; 
            for(int x = 0; x <3; x++){
                for(int y = 0; y <3; y++){
                    if(i-x-1 >= 0 && i+x-1 < height && j+y-1 >= 0 && j+y-1 < width){ // ignore if out of range 
                    r += src[(i+x-1)*width+j+y-1].R;
                    g += src[(i+x-1)*width+j+y-1].G;
                    b += src[(i+x-1)*width+j+y-1].B;
                    }
                }
            }
            dest[i*width+j].R = (unsigned char) (r/9);
            dest[i*width+j].G = (unsigned char) (g/9);
            dest[i*width+j].B = (unsigned char) (b/9);
        }
    } 
    return dest; 
}
pixel * grayScale(int width, int height, pixel * src){
    pixel * dest = (malloc)(sizeof(pixel) * width* height);
    double avg = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0 ; j < width; j++){
            avg = src[i*width+j].R + src[i*width+j].G + src[i*width+j].B; 
            dest[i*width+j].R = (unsigned char)(avg/3);
            dest[i*width+j].B = (unsigned char)(avg/3);
            dest[i*width+j].G = (unsigned char)(avg/3);
        }
    } 
    return dest; 
}

void readHeader(int *w, int *h, FILE *in){
	int mx;
	char ch;
	char pNum;
    handle_comments(in);
    fscanf(in,"%c %c\n", &ch,&pNum);
    if(ch != 'P' || pNum != '6'){
        printf("Wrong image format!\n");
        errorFlag = 1; 
        return; 
    }
    handle_comments(in);
    fscanf(in,"%d %d\n", w, h); 
    handle_comments(in);
    fscanf(in,"%d\n", &mx);  
    if(mx != 255){
        printf("Wrong image format. %d\n",mx);
        errorFlag = 1;
        return; 
    }
    
}
void writeImage(int width, int height, pixel * img, char * filename){
    FILE * out;
    out = fopen(filename, "w");
    fprintf(out,"P6\n");
	fprintf(out,"%d %d\n",width,height);
	fprintf(out,"%d\n",255); 
	for(int i=0;i<height;i++){
		for(int j=0;j<width ; j++){
			fprintf(out,"%c%c%c",img[i*width+j].R, img[i*width+j].G, img[i*width+j].B);
		}
	}
	fclose(out);
}
pixel * readImage(int width, int height, FILE *in){
    pixel * src = (malloc)(sizeof(pixel*) * height*width);  
    for(int i = 0 ; i < height*width; i++){
        fscanf(in,"%c%c%c", &(src[i].R), &(src[i].G), &(src[i].B)); // read r,g,b values 
    }
    fclose(in);
    return src; 
}

int main(int argc, char** argv){
    int option,c,h,w;
    char filename[50] = {"/0"};
    char * name; 
    pixel * src, *dest; 
    FILE * in; 
    clock_t start,end; 
    if(argc != 3 ){
        printf("FORMAT ERROR Run ./program ppmfile 1,2 or 3\n");
        return 0;
    }
    start = clock();
    strcpy(filename ,argv[1]);
    option = atoi(argv[2]);
    in = fopen(filename,"r");
    if(in==0){
        printf("File cannot be opened.\n");
        return 0;
    }
    readHeader(&w,&h,in);
    if(errorFlag)
        return 0;
    src = readImage(w,h,in); 
    switch(option){
        case 1 : dest = grayScale(w,h,src);
                 writeImage(w,h,dest,"grayscale.ppm"); 
                 printf("Done converting it to gray scale!\n");
                 break;
        case 2 : dest = flip(w,h,src); 
                 writeImage(w,h,dest,"flip.ppm"); 
                 printf("Done flipping the image!\n");
                 break;
        case 3 : dest = smooth(w,h,src); 
                 writeImage(w,h,dest,"smooth.ppm");
                 printf("Done smoothing the image!\n");
                 break;
        default : printf("Option should be 1~3\n");
                    return 0; 
    }
    end = clock(); 
    printf("Serial program execution time : %lf\n", (float)(end - start) / CLOCKS_PER_SEC);
}