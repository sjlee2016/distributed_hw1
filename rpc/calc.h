/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CALC_H_RPCGEN
#define _CALC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct inputs {
	int n1;
	int n2;
	char operator;
};
typedef struct inputs inputs;

#define CALCULATOR_PROG 0x31111111
#define CALCULATOR_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define server_calculator 1
extern  int * server_calculator_1(inputs *, CLIENT *);
extern  int * server_calculator_1_svc(inputs *, struct svc_req *);
extern int calculator_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define server_calculator 1
extern  int * server_calculator_1();
extern  int * server_calculator_1_svc();
extern int calculator_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_inputs (XDR *, inputs*);

#else /* K&R C */
extern bool_t xdr_inputs ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CALC_H_RPCGEN */
