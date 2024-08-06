#define SS //static inline

SS void h(int i){
    for(int j = 0; j < i; j++);
}

SS void g(int i){
    for(int j = 0; j < i; j++) h(j);
}

SS void f(int i){
    for(int j = 0; j < i; j++) g(j);
}


int main(){
    for(int i=0; i < 300; i++) f(i);
}