#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int errorFlag = 0; 
typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
}pixel; 

MPI_Datatype Pixel;

void handle_comments(FILE *in){
	unsigned char c = getc(in);
	while(c == '#' ){
        while (getc(in) != '\n'){
            c = getc(in);
        }
    }
	ungetc(c, in);
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
            for(int x = -1; x <=1; x++){
                for(int y = -1; y <= 1; y++){
                    if(i-x >= 0 && i+x < height && j+y >= 0 && j+y < width){ // ignore if out of range 
                    r += src[(i+x)*width+j+y].R;
                    g += src[(i+x)*width+j+y].G;
                    b += src[(i+x)*width+j+y].B;
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
	ch = fgetc(in);
	pNum = fgetc(in);
    if(ch != 'P' || pNum != '6'){
        printf("Wrong image format!\n");
        errorFlag = 1; 
        return; 
    }
    handle_comments(in);
    fscanf(in,"\n%d %d\n", w, h);
    handle_comments(in);  
    fscanf(in,"%d\n", &mx); 
    if(mx != 255){
        printf("Wrong image format.\n");
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
    int option;
    char filename[50] = {"/0"};
    char * name; 
    int size, rank;
    double start,finish;
    int len[3] = {1,1,1};
	MPI_Aint displ[3] = {0, sizeof(unsigned char) , 2*sizeof(unsigned char)}; 
    MPI_Datatype types[3] = {MPI_UNSIGNED_CHAR,MPI_UNSIGNED_CHAR,MPI_UNSIGNED_CHAR};
    pixel * src, *dest; 
    FILE * in; 
    int c,h,w;
    if(argc != 3 ){
        printf("FORMAT ERROR Run ./program ppmfile 1,2 or 3\n");
        return 0;
    }
    MPI_Init(&argc, &argv);
    start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Type_create_struct(3,len,displ,types,&Pixel);
	MPI_Type_commit(&Pixel);
    if(rank == 0){
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
    }

  
  MPI_Bcast(&w, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&option,1,MPI_INT,0,MPI_COMM_WORLD);
  int indHeight = (h/size);
  dest = (pixel*)malloc(sizeof(pixel)*w*h);
  pixel * rbuf = (pixel*)malloc(w * indHeight * sizeof(pixel));
  MPI_Scatter(src, w*indHeight, Pixel,
      rbuf, w*indHeight, Pixel, 0, MPI_COMM_WORLD);

  switch(option){
    case 1:
      src = flip(w, indHeight, rbuf);
      break;
    case 2:
      src = grayScale(w, indHeight, rbuf);
      break;
    case 3:
      src = smooth(w, indHeight, rbuf);
      break;
    default:
      printf("Invalid command!\n");
      break;
  }
  MPI_Gather(src, w*indHeight, Pixel, dest, w*indHeight, Pixel, 0, MPI_COMM_WORLD);

  if(rank == 0){

  switch(option){
    case 1:
      name = "flip.ppm";
      printf("Done flipping the image.\n");
      break;
    case 2:
      name = "grayscale.ppm";
      printf("Done grayscaling the image.\n");
      break;
    case 3:
      name="smooth.ppm";
      printf("Done smoothing the image.\n");
      break;
      }
     writeImage(w,h,dest,name); 
   }

  finish = MPI_Wtime();
  MPI_Finalize();

  printf("PROCESS %i Execution Time: %lf\n", rank, finish - start);
  return 0;
}