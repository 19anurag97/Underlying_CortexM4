#include "fault_handlers.h"

void M_Sort(uint8_t l, uint8_t h, uint8_t a[])
{
  uint8_t mid = (l+h)/2;
  uint8_t len1 = mid-l+1;
  uint8_t len2 = h-mid;

  uint8_t tempa[len1];
  uint8_t tempb[len2];

  uint8_t k=l, i=0,j=0; //main array idx.

  //Separate in 2 arrays.(Divide)
  for( i=0; i<len1; i++)
  {
    tempa[i] = a[l+i];
  }
  for( j=0;j<len2;j++)
  {
    tempb[j] = a[mid+1+j];
  }

  i=0,j=0;
  //Compare & Merge
  while (i<len1 && j<len2)
  {
    if (tempa[i] < tempb[j])
    {
      a[k] = tempa[i++];
    }
    else
    {
      a[k] = tempb[j++];
    }
    k++;
  }

  while (i != len1)
  {
    a[k++] = tempa[i++];
  }
  while (j != len2)
  {
    a[k++] = tempb[j++];
  }
}

void MergeSort(uint8_t l, uint8_t h, uint8_t a[])
{
  if (l <= h)		//WRONG BASE CONDITION
  {
	  uint8_t mid = (l+h)/2;
    MergeSort(l, mid, a);
    MergeSort(mid+1, h, a);
    M_Sort(l, h, a);
  }
}
