#include<stdio.h>
#include<stdlib.h>
#include<veda.h>
#include<time.h>
#include<math.h>

#define NOW() ({ struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); ts; })
#define TIME(start, end) ((double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_nsec - start.tv_nsec) / 1000000.0)

#define VEDA(err) check(err, __FILE__, __LINE__)

void check(VEDAresult err, const char* file, const int line) {
	if(err != VEDA_SUCCESS) {
		const char *name, *str;
		vedaGetErrorName	(err, &name);
		vedaGetErrorString	(err, &str);
		printf("%s: %s @ %s:%i\n", name, str, file, line);
		exit(1);
	}
}

double* get2DMatrixForAddition(int,int);
void print2DMatrix(double*,int,int);

int main(int argc,char *argv[]){
    int t=atoi(argv[1]);

        struct timespec start, end;
        start = NOW();

        VEDA(vedaInit(0));

        srand(time(NULL));
        
        int row=t,col=t;

        double *matrix_a = get2DMatrixForAddition(row,col);
        double *matrix_b = get2DMatrixForAddition(row,col);

        size_t matrix_size = (row*col)*sizeof(double);

        VEDAcontext ctx;
        VEDAdevice vd;
        VEDA(vedaDeviceGet(&vd,2));
        VEDA(vedaCtxCreate(&ctx,1,vd));
        VEDA(vedaCtxPushCurrent(ctx));

        VEDAmodule mod;
        VEDA(vedaModuleLoad(&mod,"lib_device.vso"));

        VEDAfunction func;
        VEDA(vedaModuleGetFunction(&func,mod,"addMatrix"));

        VEDAdeviceptr a;
        VEDAdeviceptr b;
        VEDA(vedaMemAllocAsync(&a,matrix_size,0));
        VEDA(vedaMemAllocAsync(&b,matrix_size,1));

        VEDA(vedaMemcpyHtoDAsync(a,matrix_a,matrix_size,0));
        VEDA(vedaMemcpyHtoDAsync(b,matrix_b,matrix_size,1));

        VEDAargs args;
        VEDA(vedaArgsCreate(&args));
        VEDA(vedaArgsSetVPtr(args,0,a));
        VEDA(vedaArgsSetVPtr(args,1,b));
        VEDA(vedaArgsSetI64(args,2,row));
        VEDA(vedaArgsSetI64(args,3,col));

        VEDA(vedaLaunchKernelEx(func,0,args,1,0));

        VEDA(vedaCtxSynchronize());

        VEDA(vedaMemcpyDtoHAsync(matrix_a,a,matrix_size,0));

        VEDA(vedaMemFreeAsync(a,0));
        VEDA(vedaMemFreeAsync(b,1));

        VEDA(vedaExit());

        //print2DMatrix(matrix_a,row,col);
        end = NOW();
        double elapsed = TIME(start, end);
        //printf("Total Host Elapsed Time: %f ms\n", elapsed);

    return 0;
}

double* get2DMatrixForAddition(int row,int col){

    double *m1 = (double*)malloc(row*col*sizeof(double));

    for(int i=0;i<row*col;i++){
            *(m1+i) = (double)rand()/RAND_MAX;
    }

    return m1;

}

void print2DMatrix(double *matrix,int row,int col){

    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
                printf(" %lf ",*(matrix+(i*col)+j));
        }
        printf("\n");
    }

}