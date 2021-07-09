#include "linked.h"

LINKED_node* created_LINKED() {
	LINKED_node* HEAD = new LINKED_node;
	HEAD -> next = NULL;
	return HEAD;
}

void LINKED_Insert(LINKED_node* HEAD, LINKED_node* data_node) {
	LINKED_node* p = HEAD->next;
	data_node->next= p;
	//头插
	HEAD -> next = data_node;
}

void LINKED_Query(LINKED_node* HEAD, int* LINKED_Query) {
	for(int i=0;i<10;i++){
		LINKED_node* p = HEAD->next;
		LINKED_node* q= HEAD;
		for(;p!=NULL; q=p,p = p->next){
			if(p->index == LINKED_Query[i]){
				cout<<p->index<<" : "<<p->mac<<endl;
				break;
			}
		}
	}
}

void LINKED_Delete(LINKED_node* HEAD, int* LINKED_Delete) {
	for(int i=0;i<10;i++){
		LINKED_node* p = HEAD->next;
		LINKED_node* q= HEAD;
		for(;p!=NULL; q=p,p = p->next){
			if(p->index == LINKED_Delete[i]){
				cout<<p->index<<" : "<<p->mac<<endl;
				q->next=p->next;
				delete p;
				break;
			}
		}
	}
}