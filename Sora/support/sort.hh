/* The MIT License

   Copyright (c) 2008, by Attractive Chaos <attractivechaos@aol.co.uk>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
/*
  This C++ header file implements bottom-up mergesort, introsort and
  heapsort. All functions are template functions.

  - Attractive Chaos
 */

#ifndef AC_SORT_HH
#define AC_SORT_HH

// Bottom-up iterative mergesort. Additional O(N) temporary array is
// required.

template<class TYPE>
void algo_mergesort(size_t n, TYPE array[], TYPE *temp = 0)
{
	TYPE *a2[2], *a, *b;
	int curr, shift;

	a2[0] = array;
	a2[1] = temp? temp : new TYPE[n];
	for (curr = 0, shift = 0; (1ul<<shift) < n; ++shift) {
		a = a2[curr]; b = a2[1-curr];
		if (shift == 0) {
			TYPE *p = b, *i, *eb = a + n;
			for (i = a; i < eb; i += 2) {
				if (i == eb - 1) *p++ = *i;
				else {
					if (*(i+1) < *i) {
						*p++ = *(i+1); *p++ = *i;
					} else {
						*p++ = *i; *p++ = *(i+1);
					}
				}
			}
		} else {
			size_t i, step = 1ul<<shift;
			for (i = 0; i < n; i += step<<1) {
				TYPE *p, *j, *k, *ea, *eb;
				if (n < i + step) {
					ea = a + n; eb = a;
				} else {
					ea = a + i + step;
					eb = a + (n < i + (step<<1)? n : i + (step<<1));
				}
				j = a + i; k = a + i + step; p = b + i;
				while (j < ea && k < eb) {
					if (*j < *k) *p++ = *j++;
					else *p++ = *k++;
				}
				while (j < ea) *p++ = *j++;
				while (k < eb) *p++ = *k++;
			}
		}
		curr = 1 - curr;
	}
	if (curr == 1) {
		TYPE *p = a2[0], *i = a2[1], *eb = array + n;
		for (; p < eb; ++i) *p++ = *i;
	}
	if (temp == 0) delete[] a2[1];
}

// Heap related functions (binary heap)

template<class TYPE>
inline void algo_heap_adjust(TYPE l[], int i, int n)
{
	int k;
	TYPE tmp = l[i];
	for (;;) {
		k = (i << 1) + 1; // the left child
		if (k >= n) { l[i] = tmp; return; }
		if (k < n - 1 && l[k] < l[k+1]) ++k;
		if (tmp < l[k]) { l[i] = l[k]; i = k; }
		else { l[i] = tmp; return; }
	}
}
template<class TYPE>
void algo_heap_make(TYPE l[], int lsize)
{
	for (int i = (lsize >> 1) - 1; i >= 0; --i)
		algo_heap_adjust(l, i, lsize);
}
template<class TYPE>
void algo_heap_sort(TYPE l[], int lsize)
{
	TYPE swap_tmp;
	for (int i = lsize - 1; i > 0; --i) {
		TYPE swap_tmp = l[0];
		l[0] = l[i]; l[i] = swap_tmp;
		algo_heap_adjust(l, 0, i);
	}
}

typedef struct {
	void *left, *right;
	int depth;
} algo_isort_stack_t;

template<class TYPE>
void algo_sort(size_t n, TYPE a[])
{
	int d;
	algo_isort_stack_t *top, *stack;
	TYPE rp, swap_tmp;
	TYPE *s, *t, *i, *j, *k;

	if (n == 0) return;
	for (d = 2; 1ul<<d < n; ++d);
	stack = (algo_isort_stack_t*)malloc(sizeof(algo_isort_stack_t) * ((sizeof(size_t)*d)+2));
	top = stack; s = a; t = a + (n-1); d <<= 1;
	while (1) {
		if (s < t) {
			if (--d == 0) { // trigger heapsort to avoid worst case
				algo_heap_make(s, t - s + 1);
				algo_heap_sort(s, t - s + 1);
				t = s;
				continue;
			}
			// choose pivot
			i = s; j = t; k = i + ((j-i)>>1) + 1;
			if (*k < *i) {
				if (*k < *j) k = j;
			} else k = *j < *i? i : j;
			rp = *k;
			// partition
			swap_tmp = *k; *k = *t; *t = swap_tmp;
			do {
				do { ++i; } while (*i < rp);
				do { --j; } while (j >= a && rp < *j);
				swap_tmp = *i; *i = *j; *j = swap_tmp;
			} while (i < j);
			swap_tmp = *i; *i = *j; *j = swap_tmp;
			swap_tmp = *i; *i = *t; *t = swap_tmp;
			// push to stack
			if (i-s > t-i) {
				if (i-s > 16) { top->left = s; top->right = i-1; top->depth = d; ++top; }
				s = t-i > 16? i+1 : t;
			} else {
				if (t-i > 16) { top->left = i+1; top->right = t; top->depth = d; ++top; }
				t = i-s > 16? i-1 : s;
			}
		} else {
			if (top == stack) { // insertion sort
				free(stack);
				for (i = a+1; i < a+n; ++i)
					for (j = i; j > a && *j < *(j-1); --j) {
						swap_tmp = *j; *j = *(j-1); *(j-1) = swap_tmp;
					}
				return;
			} else { --top; s = (TYPE*)top->left; t = (TYPE*)top->right; d = top->depth; }
		}
	}
}

// heap functions and introsort with user-provided comparison function

template<class TYPE, class cmp_t>
inline void algo_heap_adjust(TYPE l[], int i, int n, const cmp_t &cmp)
{
	int k;
	TYPE tmp = l[i];
	for (;;) {
		k = (i << 1) + 1; // the left child
		if (k >= n) { l[i] = tmp; return; }
		if (k < n - 1 && cmp(l[k], l[k+1]) < 0) ++k;
		if (cmp(tmp, l[k]) < 0) { l[i] = l[k]; i = k; }
		else { l[i] = tmp; return; }
	}
}
template<class TYPE, class cmp_t>
void algo_heap_make(TYPE l[], int lsize, const cmp_t &cmp)
{
	for (int i = (lsize >> 1) - 1; i >= 0; --i)
		algo_heap_adjust(l, i, lsize, cmp);
}
template<class TYPE, class cmp_t>
void algo_heap_sort(TYPE l[], int lsize, const cmp_t &cmp)
{
	TYPE swap_tmp;
	for (int i = lsize - 1; i > 0; --i) {
		TYPE swap_tmp = l[0];
		l[0] = l[i]; l[i] = swap_tmp;
		algo_heap_adjust(l, 0, i, cmp);
	}
}

template<class TYPE, class cmp_t>
void algo_sort(size_t n, TYPE a[], const cmp_t &cmp)
{
	int d;
	algo_isort_stack_t *top, *stack;
	TYPE rp, swap_tmp;
	TYPE *s, *t, *i, *j, *k;

	if (n == 0) return;
	for (d = 2; 1ul<<d < n; ++d);
	stack = (algo_isort_stack_t*)malloc(sizeof(algo_isort_stack_t) * ((sizeof(size_t)*d)+2));
	top = stack; s = a; t = a + (n-1); d <<= 1;
	while (1) {
		if (s < t) {
			if (--d == 0) { // trigger heapsort to avoid worst case
				algo_heap_make(s, t - s + 1, cmp);
				algo_heap_sort(s, t - s + 1, cmp);
				t = s;
				continue;
			}
			// choose pivot
			i = s; j = t; k = i + ((j-i)>>1) + 1;
			if (cmp(*k, *i) < 0) {
				if (cmp(*k, *j) < 0) k = j;
			} else k = cmp(*j, *i) < 0? i : j;
			rp = *k;
			// partition
			swap_tmp = *k; *k = *t; *t = swap_tmp;
			do {
				do { ++i; } while (cmp(*i, rp) < 0);
				do { --j; } while (j >= a && cmp(rp, *j) < 0);
				swap_tmp = *i; *i = *j; *j = swap_tmp;
			} while (i < j);
			swap_tmp = *i; *i = *j; *j = swap_tmp;
			swap_tmp = *i; *i = *t; *t = swap_tmp;
			// push to stack
			if (i-s > t-i) {
				if (i-s > 16) { top->left = s; top->right = i-1; top->depth = d; ++top; }
				s = t-i > 16? i+1 : t;
			} else {
				if (t-i > 16) { top->left = i+1; top->right = t; top->depth = d; ++top; }
				t = i-s > 16? i-1 : s;
			}
		} else {
			if (top == stack) { // insertion sort
				free(stack);
				for (i = a+1; i < a+n; ++i)
					for (j = i; j > a && cmp(*j, *(j-1)) < 0; --j) {
						swap_tmp = *j; *j = *(j-1); *(j-1) = swap_tmp;
					}
				return;
			} else { --top; s = (TYPE*)top->left; t = (TYPE*)top->right; d = top->depth; }
		}
	}
}

#endif
