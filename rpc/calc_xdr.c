/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calc.h"

bool_t
xdr_inputs (XDR *xdrs, inputs *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->n1))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->n2))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->operator))
		 return FALSE;
	return TRUE;
}
