
//Criptografia de curva elíptica
//Integrantes: Lucas da Rocha Pereira - 379948
//			   Thiago Martins de Jesus - 380385

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct{
	unsigned int x;
	unsigned int y;
}Ponto;

double dn,da,db,dp,dq;
unsigned long qpp, rp;
long r;

int mod(int a, int b){
	if(b < 0) //you can check for b == 0 separately and do what you want
     	return mod(-a, -b);   
   	int ret = a%b;
   	if(ret < 0)
     	ret+=b;
   	return ret;
}

long modlamda (int a, int b, int n)
{
	dn = n; da = a; db = b;
	dp = da*db;
	dq = dp/dn;
	qpp = dq+0.5; rp = a*b-qpp*n;
	r = (rp&0x80000000)?(rp+n):rp;
	return r;
}

int mdc(int  a, int b, int *x, int *y) {
  int xx, yy, d;
  if(b==0) {
    *x=1; *y=0;
    return a;
  }

  d = mdc(b, a%b, &xx, &yy);
  *x = yy;
  *y = xx - a/b*yy;
  return d;
}


int inv5(int a, int primo){
  int x,y,d;
  d = mdc(a,primo,&x,&y);

  if(x<0){
    x = x+primo;
  }

  return x;
}

int compara(Ponto a, Ponto b, int primo){
	if(a.x == 0 && a.y == 0){
		return 1;
	}else{
		if(b.x == 0 && b.y == 0){
			return 2;
		}else{
			if((a.x == b.x && a.y == mod((primo-b.y),primo)) || (b.x == a.x && b.y == mod((primo-a.y),primo))){
				return 3;
			}else{
				if((a.x == b.x && a.y == b.y) && (a.y != 0)){
					return 4;
				}else{
					return 5;
				}
			}
		}
	}
}

Ponto soma1(Ponto Q, Ponto G, int primo, int parametro){
	Ponto R;
	long int X,Y, lambda;
	X = mod((3*pow(Q.x,2)+parametro),primo);
	Y = mod((2*Q.y),primo);
	lambda = (modlamda(X,inv5(Y,primo),primo));
	R.x = mod(pow(lambda,2)-Q.x-G.x,primo);
	R.y = mod(lambda*(Q.x-R.x)-Q.y,primo);
	//printf("lambda = %d R.x = %d R.y = %d\n",lambda, R.x,R.y );
	return R;
}

Ponto soma2(Ponto Q, Ponto G, int primo, int parametro){
	Ponto R;
	long int X,Y,lambda;
	X = mod((G.y-Q.y),primo);
	Y = mod((G.x-Q.x),primo);
	lambda = modlamda(X,inv5(Y,primo),primo);
	R.x = mod(pow(lambda,2)-Q.x-G.x,primo);
	R.y = mod(lambda*(Q.x-R.x)-Q.y,primo);
	//printf("R.x = %d R.y = %d\n",R.x,R.y );
	return R;
}


int main(){
	Ponto G,Q;
	Q.x = Q.y = 0;
	long int primo, parametro, escalar,i, resp; 
	scanf("%ld", &escalar);
	while(escalar != 0){
		scanf("%ld %ld %d %d",&parametro, &primo, &G.x, &G.y);
		if(escalar == 1){
			Q.x = G.x;
			Q.y = G.y;
		}else{
			if(G.y !=0){
				Q = soma1(G,G,primo,parametro);
			}else{
				Q = soma2(G,G,primo,parametro);
			}
			
			for(i=2;i<escalar;i++){
				//printf("I: %ld\n",i );
				resp = compara(G,Q,primo);
				//printf("%ld\n",resp );
				if(resp == 1){
					Q.x = G.x;
					Q.y = G.y;
				}else{
					if(resp == 2){
						Q.x = G.x;
						Q.y = G.y;
					}else{
						if(resp == 3){
							Q.x = 0;
							Q.y = 0;
						}else{
							if(resp == 4){
								Q = soma1(Q,G,primo,parametro);
							}else{
								if(resp == 5){
									Q = soma2(Q,G,primo,parametro);
								}
							}
						}
					}
				}	
			}
		}
		printf("%d %d\n",Q.x,Q.y);
		Q.x = Q.y = 0;
		scanf("%ld",&escalar);
	}
}