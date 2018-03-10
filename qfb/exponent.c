/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 William Hart

******************************************************************************/

#include <stdlib.h>
#include <gmp.h>
#include "flint/flint.h"
#include "flint/ulong_extras.h"
#include "flint/fmpz.h"
#include "qfb.h"

int qfb_exponent(fmpz_t exponent, fmpz_t n, ulong B1, ulong B2_sqrt, slong c)
{
   fmpz_t p, exp, n2;
   slong i;
   qfb_t f;
   ulong pr, nmodpr, s;
   int ret = 1;
   n_primes_t iter;

   n_primes_init(iter);
   fmpz_init(p);
   fmpz_init(n2);
   fmpz_init(exp);
   qfb_init(f);

   fmpz_set_ui(exponent, 1);

   /* find odd prime such that n is a square mod p */
   pr = 0;
   for (i = 0; i < c + 2; )
   {
      i--;
      do
      {
         pr = n_primes_next(iter);
         nmodpr = fmpz_fdiv_ui(n, pr);
         i++;
      } while ((pr == 2 && ((s = fmpz_fdiv_ui(n, 8)) == 2 || s == 3 || s == 5))
         || (pr != 2 && nmodpr != 0 && n_jacobi(nmodpr, pr) < 0));

      if (i < c + 2)
      {
         fmpz_set_ui(p, pr);
      
         /* find prime form of discriminant n */
         qfb_prime_form(f, n, p);
         fmpz_set(n2, n);

         /* deal with non-fundamental discriminant */
         if (nmodpr == 0 && fmpz_fdiv_ui(f->c, pr) == 0)
         {   
            fmpz_fdiv_q_ui(f->a, f->a, pr);
            fmpz_fdiv_q_ui(f->b, f->b, pr);
            fmpz_fdiv_q_ui(f->c, f->c, pr);
            fmpz_fdiv_q_ui(n2, n2, pr*pr);
         }
         if (pr == 2 && fmpz_is_even(f->a) 
                     && fmpz_is_even(f->b) && fmpz_is_even(f->c))
         {
            fmpz_fdiv_q_2exp(f->a, f->a, 1);
            fmpz_fdiv_q_2exp(f->b, f->b, 1);
            fmpz_fdiv_q_2exp(f->c, f->c, 1);
            fmpz_fdiv_q_2exp(n2, n2, 2);
         }
         
         qfb_reduce(f, f, n2);
         
         if (!fmpz_is_one(exponent))
            qfb_pow(f, f, n2, exponent);

         if (!qfb_exponent_element(exp, f, n2, B1, B2_sqrt))
         {
            ret = 0;
            goto cleanup;
         }
      
         if (fmpz_is_one(exp))
            i++;
         else
            fmpz_mul(exponent, exponent, exp);
      }
   }

cleanup:
   qfb_clear(f);
   fmpz_clear(p);
   fmpz_clear(n2);
   fmpz_clear(exp);
   n_primes_clear(iter);

   return ret;
}
