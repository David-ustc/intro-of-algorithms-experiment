#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
using namespace std;
#define RANGE 10000000
#define MAXINT 2147483647
typedef long ElemType;
typedef long KeyType;
typedef struct {
    ElemType *r;
    long len;
} SqTable;
class array{
    public:
        SqTable L;
        string classname;
        array(long len){
            L.r = (ElemType*)malloc((len+1)*sizeof(ElemType));
            L.len = len; 
            BuildArray();
        }
        /**array(array &oldarray){
            L.r = (ElemType*)malloc((L.len+1)*sizeof(ElemType));
            L.len = oldarray.L.len;
            memcpy(L.r, oldarray.L.r, (oldarray.L.len+1)*sizeof(ElemType));
        }**/
        array(const array &obj);
        void BuildArray(){
            long i;
            srand((unsigned) time(NULL));
            for (i=0;i<L.len+1;i++){
                L.r[i]= (rand()*(32767)+rand())% RANGE +1;
            }
        }
        ~array(){
            free(L.r);
        }
        void show(){
            for (int i=0;i<0;i++) std::cout<<L.r[i]<<'\t';
            string filename = "./result/"+ classname + ".txt";
            FILE*fp=fopen(filename.c_str(), "a+");
	        for(int i=0;i<100;i++)
            fprintf(fp,"%ld ", L.r[i]);
            fprintf(fp, "\n");
            fclose(fp);
        }
};
class Qsort: public array{
    public:
        Qsort(long len):array(len){
            classname = "Qsort";
        };
        long Partition(long low, long high) {
            long pivotloc = low; 
            ElemType tmp = L.r[pivotloc];
            KeyType pivotkey = tmp;
            while (low<high) {
                while (low<high && L.r[high]>=pivotkey) --high;
                L.r[low] = L.r[high];
                while (low<high && L.r[low]<=pivotkey) ++low;
                L.r[high] = L.r[low];
            }
            L.r[low] = tmp;
            return low;
        }
        void Quicksort(long low, long high) {
            if (low < high) {
            long pivotloc = Partition(low, high);
            Quicksort(low, pivotloc-1);
            Quicksort(pivotloc+1, high);}
        }
        void QuickSort() {
            clock_t begin = clock(); // 计时器开始
            Quicksort(0, L.len-1); 
            clock_t end = clock(); // 计时器结束
            show();
            double timerecord = (double)(end-begin)/CLOCKS_PER_SEC;
            for(int i=0;i<10;i++){
                if(TimeRecord[i]==0){
                TimeRecord[i] = timerecord;
                break;}
            }
        }
        
        static double TimeRecord[9];
};
double Qsort::TimeRecord[9]={0,0,0,0,0,0,0,0,0};
class Bsort: public array{
    public:
        Bsort(long len):array(len){classname = "Bsort";};
        void BubbleSort() {
            bool change = true;
            long n=L.len;
            for (long i=n; i>=2 && change; --i) {
                change = false;
                for (long j=1; j<=i-1; ++j) {
                    if (L.r[j] > L.r[j+1]) {
                        ElemType tmp = L.r[j]; L.r[j] = L.r[j+1]; L.r[j+1] = tmp;
                        change = true;}
                }
            }
        }
};
class Isort: public array{
    public:
        Isort(long len):array(len){classname = "Isort";};
        void InsertSortSub(SqTable &L, long low, long high) {
        long j;
        for (long i=low+1; i<=high; ++i)
            if (L.r[i]< L.r[i-1]) {
            ElemType tmp = L.r[i];
            for (j=i-1; tmp< L.r[j]&& j>=low; --j)  L.r[j+1] = L.r[j]; // 元素后移
            L.r[j+1] = tmp;      // 插入到合适位置
            }
        }
        void InsertSort() {
            clock_t begin = clock(); // 计时器开始
            InsertSortSub(L, 0, L.len-1);
            clock_t end = clock(); // 计时器结束
            show();
            double timerecord = (double)(end-begin)/CLOCKS_PER_SEC;
            for(int i=0;i<10;i++){
                if(TimeRecord[i]==0){
                TimeRecord[i] = timerecord;
                break;}
            }
        }
        static double TimeRecord[9];
};
double Isort::TimeRecord[9]={0,0,0,0,0,0,0,0,0};
class Hsort: public array{
    public:
        Hsort(long len):array(len){classname = "Hsort";};
        void HeapAdjust(long s, long m) {
            ElemType tmp = L.r[s];
            for (long i=2*s; i<=m; i*=2) { 
                if (i<m && L.r[i]<L.r[i+1]) ++i;
                if (tmp>= L.r[i]) break; 
                L.r[s] = L.r[i]; s = i; 
            }
            L.r[s] = tmp;
        }
        void HeapSort() {
            long i; ElemType tmp;
            clock_t begin = clock(); // 计时器开始
            for (i=L.len/2; i>=0; --i) HeapAdjust(i, L.len-1); // 建堆
            for (i=L.len-1; i>0; --i) {
                tmp = L.r[i];
                L.r[i] = L.r[0];
                L.r[0] = tmp; // 
                HeapAdjust(0, i-1); 
            }
            clock_t end = clock(); // 计时器结束
            show();
            double timerecord = (double)(end-begin)/CLOCKS_PER_SEC;
            for(int i=0;i<10;i++){
                if(TimeRecord[i]==0){
                TimeRecord[i] = timerecord;
                break;}
            }
        }
        static double TimeRecord[9];
};
double Hsort::TimeRecord[9]={0,0,0,0,0,0,0,0,0};
class Msort: public array{
    public:
        Msort(long len):array(len){classname = "Msort";};
        void Merge(ElemType* Rs, ElemType* Rt, long s, long m, long t) {
            long i,j,k;
            for (i=s, j=m+1, k=s; i<=m && j<=t; ++k) {
                if (Rs[i]<= Rs[j]) Rt[k] = Rs[i++];
                else Rt[k] = Rs[j++];
            }
            for (; i<=m; ++i, ++k) Rt[k] = Rs[i];
            for (; j<=t; ++j, ++k) Rt[k] = Rs[j];
        }
        void Mergesort(ElemType* Rs, ElemType* Rt, long low, long high) {
            if (low < high) {
                long mid = (low+high)/2;
                Mergesort(Rs, Rt, low, mid); Mergesort(Rs, Rt, mid+1, high);
                Merge(Rs, Rt, low, mid, high);
                for (long i=low; i<=high; ++i) Rs[i] = Rt[i];}
        }
        void MergeSort() {
            clock_t begin = clock();//
            ElemType* tmp = new ElemType[L.len];
            Mergesort(L.r, tmp, 0, L.len-1);
            delete []tmp;
            clock_t end = clock(); // 计时器结束
            show();
            double timerecord = (double)(end-begin)/CLOCKS_PER_SEC;
            for(int i=0;i<10;i++){
                if(TimeRecord[i]==0){
                TimeRecord[i] = timerecord;
                break;}
            }
        }
        static double TimeRecord[9];
};
double Msort::TimeRecord[9]={0,0,0,0,0,0,0,0,0};
int main() {
    long N = 50000;
    for(int i=0;i<10;i++){
        N+=50000;
        Isort* ptr0= new Isort(N);
        //(*ptr0).InsertSort();
        delete ptr0;

        Qsort* ptr= new Qsort(N);
        (*ptr).QuickSort();
        delete ptr;

        Hsort* ptr2 = new Hsort(N);
        (*ptr2).HeapSort();
        delete ptr2;

        Msort* ptr3 = new Msort(N);
        (*ptr3).MergeSort();
        delete ptr3;
    }
    for ( int j = 0; j < 10; j++) std::cout<< Isort::TimeRecord[ j ] << "\t";
    std::cout<<'\n';
    for ( int j = 0; j < 10; j++) std::cout<< Qsort::TimeRecord[ j ] << "\t";
    std::cout<<'\n';
    for ( int j = 0; j < 10; j++) std::cout<< Hsort::TimeRecord[ j ] << "\t";
    std::cout<<'\n';
    for ( int j = 0; j < 10; j++) std::cout<< Msort::TimeRecord[ j ] << "\t";
    std::cout<<'\n';
    getchar();
}