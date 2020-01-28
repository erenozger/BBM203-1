#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void print_matrix(int **,int,int);
int** found_subMatrix(int **,int,int,int);
int multipleMatrix(int **,int **,int);
void findtreasure(int **,int **,int **,int,int,int,int,int,FILE *);
int calculateC(int **,int **,int);
int main(int argc , char *argv[]){
   

    char *sizeOf_MM_String= argv[1];   
    int keyMatrix_size;
    int *mapMatrix_size[2]={0,0};

    int i,j;
    int row,column;
    /*Create files and token size from command line*/
    FILE *output_file = fopen(argv[5],"w");
    FILE *map_file = fopen(argv[3], "r");
    FILE *key_file = fopen(argv[4], "r");
    keyMatrix_size=atoi(argv[2]);   
    /*Control codes for files*/
    if(NULL == key_file)
    {
        fprintf(stderr, "Cannot open file: %s\n", key_file);
        return 1;
    }
    if(NULL == map_file)
    {
        fprintf(stderr, "Cannot open file: %s\n", map_file);
        return 1;
    }    
    /*find row and column count with atoi*/
    const char s[2] = "x";
    char *token = sizeOf_MM_String;
        
        row= atoi(strtok(token,s));
        column=atoi(strtok(NULL, s));
       
   
    /*create key matrix with 2d dynamic arrays*/
    int **key_matrix=(int **)malloc(keyMatrix_size*sizeof(int *));
    for(i=0;i<keyMatrix_size;i++){
        key_matrix[i]=malloc(keyMatrix_size*sizeof(int *));
    }
    for(i = 0; i < keyMatrix_size; i++){
          
          for(j = 0; j < keyMatrix_size; j++){     
              fscanf(key_file, "%d", &key_matrix[i][j]);  
          }    
    }   
       

    /*create map matrix with 2d dynamic arrays*/
    int **map_matrix=(int **)malloc(row*sizeof(int *));
    for(i=0;i<row;i++){
        map_matrix[i]=malloc(column*sizeof(int *));
    }
    for(i = 0; i <row; i++){        
          for(j = 0; j < column; j++){     
              fscanf( map_file, "%d", &map_matrix[i][j]);
          }   
    }
     

    /*create sub matrix with 2d dynamic arrays*/
    int **sub_matrix=(int **)malloc(keyMatrix_size*sizeof(int *));
    for(i=0;i<keyMatrix_size;i++){
        sub_matrix[i]=malloc(keyMatrix_size*sizeof(int *));
    }
    for(i = 0; i <keyMatrix_size; i++){
          
          for(j = 0; j < keyMatrix_size; j++){
              sub_matrix[i][j]=map_matrix[i][j];   
          }    
        
    }   
    int max_columnCount;
    int max_rowCount;
    /*Calculate max counts for row and column (if map matrix is 18x18 then max row :15)*/
    max_rowCount=(row-keyMatrix_size);
    max_columnCount=(column-keyMatrix_size);

    /*Create first sub matrix and w'll use it on findtreasure func.*/
    sub_matrix=found_subMatrix(map_matrix,0,0,keyMatrix_size);
    findtreasure(map_matrix,key_matrix,sub_matrix,keyMatrix_size,0,0,max_columnCount,max_rowCount,output_file);
    

    
    free(sub_matrix);
    free(key_matrix);
    free(map_matrix);
    fclose(key_file);
    fclose(map_file);
    fclose(output_file);
    
    return(0);

}

void print_matrix(int **matrix,int r,int c)
{
   /* i used this func when i was testing prog.*/
   int i,j;
   
   for(i=0;i<r;i++)
   {
      for(j=0;j<c;j++) 
      { if(j==0) printf("|%5d",matrix[i][j]);
        else if(j==c-1) printf("%5d|",matrix[i][j]);
        else printf("%5d",matrix[i][j]);
      }
	  printf("\n");
   }
   printf("\n\n");
}


int** found_subMatrix(int **mapMatrix,int rowCount,int columnCount,int size){
    
    /*it's creating to new subMatrix with next center of move*/
    
    int **result;
    /*result is the temp matrix */
    int i,j;
    result=calloc(size,sizeof(int *));
    for(i=0;i<size;i++){
        result[i]=calloc(size,sizeof(int *));
    }
    
    
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            result[i][j]=mapMatrix[i+rowCount][j+columnCount];
        }
    }
    return result;
    
}

int calculateC(int **keyM,int **subM,int size){
    /*Calculating c with 2 matrix and their size*/
    int c=0,i,j;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            c=c+(keyM[i][j]*subM[i][j]);
        }

    }
    return c;
}


void findtreasure(int **mapM,int **keyM,int **subM,int size,int row,int column,int max_columnCount,int max_rowCount,FILE *file){
    
    /*it's the real func in this program*/
    /*and it's recursive function*/
    /*it takes 3 matrixs and 5 int value and file(output.txt)*/

    int c,d,centerrow,centercolumn;
    centerrow=(row+((size-1)/2));
    centercolumn=(column+((size-1)/2));
    /*find new c every move */
    c=calculateC(keyM,subM,size);
    d=c%5;

    /* chanching row and column with value of new d */
    if(d==0){
        fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
        
    }  
    
    if(d==1){
        if(row==0){
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            row=(row+size);
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }
        else{
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            row=row-size;
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }   
    }
    if(d==2){
        if(row==max_rowCount){
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            row=(row-size);
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }    
    
    else{
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            row=row+size;
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }
    }
    if(d==3){
        if(column==max_columnCount){
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            column=column-size;
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }
    else{
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            column=column+size;
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }
    }
    if(d==4){
        if(column==0){
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            column=(column+size); 
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }
    else
        {
            fprintf(file,"%d,%d:%d\n",centerrow,centercolumn,c);
            column=column-size;
            subM=found_subMatrix(mapM,row,column,size);
            findtreasure(mapM,keyM,subM,size,row,column,max_columnCount,max_rowCount,file);
        }
    }
}
















