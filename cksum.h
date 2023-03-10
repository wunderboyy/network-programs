#include <sys/types.h>

static inline u_short
in_cksum(const u_short* addr, register int len, u_short csum)
{
  register int nleft = len;
  const u_short* w = addr;
  register u_short answer;
  register int sum = csum;

  /*
   *  Our algorithm is simple, using a 32 bit accumulator (sum),
   *  we add sequential 16 bit words to it, and at the end, fold
   *  back all the carry bits from the top 16 bits into the lower
   *  16 bits.
   */
  while (nleft > 1) {
    sum += *w++;
    nleft -= 2;
  }

#define ODDBYTE(v) (v)

  /* mop up an odd byte, if necessary */
  if (nleft == 1)
    sum +=
      ODDBYTE(*(u_char*)w); /* le16toh() may be unavailable on old systems */

  /*
   * add back carry outs from top 16 bits to low 16 bits
   */
  sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
  sum += (sum >> 16);                 /* add carry */
  answer = ~sum;                      /* truncate to 16 bits */
  return (answer);
}
