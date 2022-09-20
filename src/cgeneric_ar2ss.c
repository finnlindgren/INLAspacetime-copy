#include "cgeneric_defs.h"

double *inla_cgeneric_ar2ss_model(inla_cgeneric_cmd_tp cmd, double *theta, inla_cgeneric_data_tp * data)
{
  
  double *ret = NULL, a1, a2, a1s, sleng, lprec, prec, pcorrect;
  
  assert(!strcasecmp(data->ints[0]->name, "n"));
  int N = data->ints[0]->ints[0];
  assert(N > 0);
  
  assert(!strcasecmp(data->doubles[0]->name, "psigma"));
  inla_cgeneric_vec_tp *psigma = data->doubles[0];
  assert(psigma->len == 2);

  assert(!strcasecmp(data->doubles[1]->name, "pleng"));
  inla_cgeneric_vec_tp *pleng = data->doubles[1];
  assert(pleng->len == 2);

  assert(!strcasecmp(data->doubles[2]->name, "pcor"));
  inla_cgeneric_vec_tp *pcor = data->doubles[2];
  assert(pcor->len == 2);

  int npar=0;
  if(theta) {
    if (iszero(psigma->doubles[1])) {
      prec = 1/SQR(psigma->doubles[0]);
      lprec = log(prec);
    } else {
      lprec = -2 * theta[npar];
      prec = exp(lprec);
      npar++;
    }
    if (iszero(pleng->doubles[1])) {
      sleng = pleng->doubles[0];
    } else {
      sleng = exp(theta[npar]);
      npar++;
    }
    if (iszero(pcor->doubles[1])) {
      a2 = pcor->doubles[0];
    } else {
      a2 = (2.0 / ( 1.0 + exp(-theta[npar]) )) -1.0;
      npar++;
    }
    a1 = -2 * sqrt(a2) * cos(2 * M_PI / sleng);
    a1s = SQR(a1);
    pcorrect = (1 + a2) / ( (1.0-a2) * (1.0-a1+a2) * (1.0+a1+a2) ) ;
  } else {
    sleng = NAN;
    prec = lprec = NAN;
    a2 = NAN;
    pcorrect = a1s = a1 = NAN;
  }
  
  switch (cmd) {
  case INLA_CGENERIC_VOID:
    {
      assert(!(cmd == INLA_CGENERIC_VOID));
      break;
    }
    
  case INLA_CGENERIC_GRAPH:
    {
      int M = N + N - 1, i=0, k=0;
      if(N>2)
	M += N-2;
      //      printf("graph M: %d\n", M);
      ret = Calloc(2 + 2 * M, double);

      ret[k++] = N;				/* dimension */
      ret[k++] = M;				/* number of (i <= j) */
      //      printf("graph : N = %g, M = %g, k = %d\n", ret[0], ret[1], k);
      ret[k++] = 0;
      ret[k++] = 0;
      if(N==2) {
	ret[k++] = 1;
	ret[k++] = 0;
	ret[k++] = 1;
	ret[k++] = 1;
      }
      //      printf("G k = %d\n", k);
      if(N>2) {
	ret[k++] = 0;
	if(N>3) {
	  for (i = 1; i < (N-2); i++) {
	    ret[k++] = i;		       /* i */
	    ret[k++] = i;		       /* i */
	    ret[k++] = i;		       /* i */
	  }
	}
	//	printf("G k = %d\n", k);
	ret[k++] = i;
	ret[k++] = i;
	ret[k++] = i+1;
	for (i = 0; i < (N-2); i++) {
	  ret[k++] = i;                 /* j */
	  ret[k++] = i+1;                 /* j */
	  ret[k++] = i+2;                 /* j */
	}
	ret[k++] = i;
	ret[k++] = i+1;
	ret[k++] = i+1;
	//printf("G k = %d\n", k);
      }
      assert(N == (i+2));
      assert((2*M+2) == k);
      
      /*      if(N<20) {
	FILE *fp = fopen("gg2", "w");
	k=0;
	fprintf(fp, "%g ",ret[k++]);
	fprintf(fp, "%g\n",ret[k++]);
	for(int l=0; l<2; l++) {
	  for(i=1; i<(N-1); i++) {
	    fprintf(fp, "%d %g ", i, ret[k++]);
	    fprintf(fp, "%g ",ret[k++]);
	    fprintf(fp, "%g\n",ret[k++]);
	  }
	  fprintf(fp, "%d %g ", i, ret[k++]);
	  fprintf(fp, "%g\n", ret[k++]);
	  fprintf(fp, "%d %g\n",i+1,ret[k++]);
	}
	fclose(fp);
	} */

      break;
    }
  
  case INLA_CGENERIC_Q:
    {
      double q1, q2, q3, param = pcorrect * prec; 
      int M = N + N - 1, i, k;
      if(N>2)
	M += N-2;
      // printf("Q M: %d\n", M);
      ret = Calloc(2 + M, double);
      
      k=0;
      ret[k++] = -1;
      ret[k++] = M;
      //      printf("Q : ret = %g, M = %g, k = %d\n", ret[0], ret[1], k);
      ret[k++] = param;
      if(N==2) {
	ret[k++] = 0.5 * a1 * param;    // convention: 0.5 * rho * (1-rho^2)/s2innovation
      }
      if(N>2) {
	ret[k++] = a1 * param;
	ret[k++] = a2 * param;
      }
      if(N>3) {
	ret[k++] = (1.0 + a1s) * param;
	ret[k++] = a1 * (1 + a2) * param;
	ret[k++] = a2 * param;
      }
      //      printf("q123 = %g, %g, %g\n", q1, q2, q3);
      if(N>4) {
	q1 = ( 1.0 + a1s + SQR(a2) ) * param;
	q2 = (a1 * (1.0 + a2)) * param;
	q3 = a2 * param;
	//    printf("a = %g, %g\n", a1, a2);
	//    printf("q123 = %g, %g, %g\n", q1, q2, q3);
	for(i=3; i<(N-1); i++) {
	  ret[k++] = q1;
	  ret[k++] = q2;
	  ret[k++] = q3;
	}
      }
      //      printf("Q k = %d\n", k);
      if(N>2) {
	ret[k++] = (1.0 + a1s) * param;
	ret[k++] = a1 * param;
      }
      if(N>1) {
	ret[k++] = param;
      }
      /*
      if(N<20) {
	FILE *fp = fopen("qq2", "w");
	k=0;
	fprintf(fp, "%g ",ret[k++]);
	fprintf(fp, "%g\n",ret[k++]);
	for(i=1; i<(N-1); i++) {
	  fprintf(fp, "%d %g ", i, ret[k++]);
	  fprintf(fp, "%g ",ret[k++]);
	  fprintf(fp, "%g\n",ret[k++]);
	}
	fprintf(fp, "%d %g ", i, ret[k++]);
	fprintf(fp, "%g\n", ret[k++]);
	fprintf(fp, "%d %g\n",i+1,ret[k++]);
	fclose(fp);
      }
      */
      break;
      }
    
  case INLA_CGENERIC_MU:
    {
      ret = Calloc(1, double);
      ret[0] = 0;
      break;
    }
    
  case INLA_CGENERIC_INITIAL:
    {
      ret = Calloc(npar + 1, double);
      npar=0;
      if (!iszero(psigma->doubles[1])) {
	npar++;
	ret[npar] = 0.0; // log(psigma->doubles[0] - 1);
      }
      if (!iszero(pleng->doubles[1])) {
	npar++;
	ret[npar] = 10.0; // log(pleng->doubles[0] + 1);
      }
      if (!iszero(pcor->doubles[1])) {
	npar++;
	ret[npar] = 2.94445; // log((0.5+0.5*pcor->doubles[0])/(1-(0.5+0.5*pcor->doubles[0]))); 
      }
      ret[0] = npar;
      break;
    }
    
  case INLA_CGENERIC_LOG_NORM_CONST:
    {
      break;
    }
    
  case INLA_CGENERIC_LOG_PRIOR:
    {
      ret = Calloc(1, double);
      ret[0] = 0.0;
      double lam;
      npar=0;
      if (!iszero(psigma->doubles[1])) {
	lam = -log(psigma->doubles[1])/psigma->doubles[0];
	ret[0] += log(lam) + theta[npar] - lam * exp(theta[npar]);
	npar++;
      }
      if (!iszero(pleng->doubles[1])) {
	lam = -log(pleng->doubles[1])/pleng->doubles[0];
	ret[0] += log(lam * 0.5) - 0.5 * theta[npar] - lam * exp(-0.5 * theta[npar]);
	npar++;
      }
      if (!iszero(pcor->doubles[1])) {
	//	ret[0] += priorfunc_pc_cor1(&theta[npar], &pcor->doubles[0]);
	ret[0] += -0.5*SQR(theta[npar]);
	npar++;
      }
      break;
    }
    
  case INLA_CGENERIC_QUIT:
  default:
    break;
  }
  
  return (ret);
}
