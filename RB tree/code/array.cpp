#include "array.h"

void ARRAY_Insert(ARRAY_node data_ARRAY[], ARRAY_node data_node, int ARRAY_length) {
	if(ARRAY_length == 0){
	data_ARRAY[0].index = data_node.index;
	data_ARRAY[0].mac = data_node.mac;
	return ;}
	for(int i=0; i<ARRAY_length; i++){
		if(data_node.index < data_ARRAY[i].index){
			for(int j=ARRAY_length; j>i;j--){
				data_ARRAY[j].index = data_ARRAY[j-1].index;
				data_ARRAY[j].mac = data_ARRAY[j-1].mac;
			}
			data_ARRAY[i].index = data_node.index;
			data_ARRAY[i].mac = data_node.mac;
			return ;
		}
	}
	data_ARRAY[ARRAY_length].index = data_node.index;
	data_ARRAY[ARRAY_length].mac = data_node.mac;
}

void ARRAY_Query(ARRAY_node data_ARRAY[], int* Query_index, int ARRAY_length) {
	for(int i=0;i<10;i++){
		int front = 0;
		int end = ARRAY_length-1;
		int mid = (front+end)/2;
		while(front<end&&data_ARRAY[mid].index!=Query_index[i]){
			if(data_ARRAY[mid].index < Query_index[i]) front=mid+1;
			if(data_ARRAY[mid].index > Query_index[i]) end=mid-1;
			mid=(front+end)/2;
		}
		cout<<data_ARRAY[mid].index<<" : "<<data_ARRAY[mid].mac<<endl;
	}
}

void ARRAY_Delete(ARRAY_node data_ARRAY[], int* Delete_index, int& ARRAY_length) {
	for(int j=0;j<10;j++){
		int front = 0;
		int end = ARRAY_length-1;
		int mid = (front+end)/2;
		while(front<end&&data_ARRAY[mid].index!=Delete_index[j]){
			if(data_ARRAY[mid].index < Delete_index[j]) front=mid+1;
			if(data_ARRAY[mid].index > Delete_index[j]) end=mid-1;
			mid=(front+end)/2;
		}
		if(Delete_index[j]== data_ARRAY[mid].index){
			cout << data_ARRAY[mid].index<<' '<<data_ARRAY[mid].mac<<endl;
			for(int k=mid; k<ARRAY_length-1;k++){
				data_ARRAY[k].index = data_ARRAY[k+1].index;
				data_ARRAY[k].mac = data_ARRAY[k+1].mac;
			}
		}
	}
}