#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

/* Monster structure and helper functions - DO NOT MODIFY THESE. */

typedef struct monster {
	int id;
	char name[64];
	char element[64];
	int population;
	double weight;
} monster;

monster *make_some_monsters(int n)
{
  monster *monsters = malloc(sizeof(monster) * n);

  time_t t;

  srand((unsigned) time(&t));

  for(int i = 0; i < n; i++)
  {
    monsters[i].id = i;
    sprintf(monsters[i].name, "Monster #%d", rand());
    sprintf(monsters[i].element, "Element #%d", rand());
    monsters[i].population = rand();
    monsters[i].weight = 500.0 * ((double) rand() / (double) RAND_MAX);
  }

  return monsters;
}

void output_monster_list(monster *list, int n, char *title) {
  printf("List %s:\n", title);
  for(int i = 0; i < n; i++) {
    printf("  Monster %d: %s %s %d %lf\n", i, list[i].name, list[i].element, list[i].population, list[i].weight);
  }
  printf("\n");
}

void print_clocks(clock_t clocks) {
  printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}

void swap_monsters(monster *list, int i, int j)
{
  monster temp;

  memcpy(&temp, list + i, sizeof(monster));
  memcpy(list + i, list + j, sizeof(monster));
  memcpy(list + j, &temp, sizeof(monster));
}

/* The core comparison function. */

int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
  	if(use_name == 1)
	{
    	return strcmp(m1-> name, m2-> name);
	}
  	if(use_weight == 1) 
	{
    	if(m1-> weight < m2-> weight)
		{
      		return 1;
		}
    	else if(m1-> weight > m2-> weight)
		{
      		return 0;
		}
  	}
	return 1000000000;
}

void check_monster_sort(monster *list, int n, int use_name, int use_weight) //moved down beacuse was throwing error when above
{
  for(int i = 1; i < n; i++) {
    if(compare_monsters(list + i - 1, list + i, use_name, use_weight) == 0) {
      printf("** The list is NOT sorted.\n\n");
      return;
    }
  }
  printf("The list is sorted.\n\n");
}

/* Implement ascending quick sort. */

int repartition(monster *list, int low_index, int high_index, int *comparisons, int *swaps, int use_name, int use_weight)
{
	monster pivot_value = list[high_index];
	int temp = low_index;
	int j = 0;

  	for(int j = low_index; j < high_index; j++)
  	{
    	if(compare_monsters(&list[j], &pivot_value, use_name, use_weight) == 1) 
		{
      		(*swaps)++;
      		swap_monsters(list, temp, j);
      		temp++;
    	}
		(*comparisons)++;
  	}
  	swap_monsters(list, temp, high_index);
	swaps++;
  	return temp;
}

/* Recursive function for quick sort. */

void quick_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *swaps, int use_name, int use_weight)
{
  	int pivot_index = 0; 
	pivot_index = repartition(list, low_index, high_index, comparisons, swaps, use_name, use_weight);

	//if(low_index < (pivot_index - 1)) error if used this way  	
	if ((pivot_index - 1) > low_index)
	{
    	quick_sort_recursive(list, low_index, (pivot_index - 1), comparisons, swaps, use_name, use_weight);
	}
	if(high_index > (pivot_index + 1))
	{
    	quick_sort_recursive(list, (pivot_index + 1), high_index, comparisons, swaps, use_name, use_weight);
	}
}

/* Shell function for quick sort. */

void quick_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  start_cpu = clock();
  quick_sort_recursive(list, 0, n-1, &comparisons, &swaps, use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Implement ascending bubble sort. */

void bubble_sort(monster *list, int n, int use_name, int use_weight) 
{
  int i = 0;
  int j = 0;
  int temp = 0;
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Bubble sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  start_cpu = clock();

  	for(i = (n - 1); i >= 0; i--)
  	{
    	for(j = 0; j < i; j++)
    	{
      		if(compare_monsters(&list[j], &list[j + 1], use_name, use_weight) == 0) 
      		{
        		swap_monsters(list, j, (j + 1));
				swaps++;
      		}
      		comparisons++;
    	}
  	}

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Highest-value finder for selection sort. */

int find_highest(monster *list, int n, int *comparisons, int use_name, int use_weight)
{
  	int i = 0;
  	int location = 0;
  	monster* value = &list[0];
  
  	for(i = 0; i <= n; i++)
  	{
    	if(compare_monsters(&list[i], value, use_name, use_weight) == 0) 
    	{
      		location = i;
      		value = &list[i];
    	}
    	(*comparisons)++;
  	}
  	return location;
}

/* Implement ascending selection sort. */

void selection_sort(monster *list, int n, int use_name, int use_weight)
{
  int i = 0;
  int highest = 0;
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Selection sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  start_cpu = clock();

  /* Iterate through the count for our monsters, use find_highest to compare the last
  highest value to our list starting from the highest index given to the function. If
  the new highest location does not equal the current index in our iteration, swap
  the monsters to place them in ascending order. */
  	for(i = (n - 1); i > 0; i--)
  	{
    	highest = find_highest(list, i, &comparisons, use_name, use_weight);
    	if(highest != i)
    	{
      		swap_monsters(list, highest, i);
      		swaps++;
    	}
  	}
  
  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int *comparisons, int use_name, int use_weight)
{
	int i = low_index; //needs declared before the for loop

  	for(int i = low_index; i < high_index; i++)
  	{
    	if(compare_monsters(k, &list[i], use_name, use_weight) == 1) 
		{
      		return i;
    	}
    	(*comparisons)++;
  	}
  	return high_index;
}

/* Implement insertion sort. */



void insertion_sort_internal(monster *list, int n, int *comparisons, int *copies, int *block_copies, int use_name, int use_weight)
{
	int i = 0;
	int j = 0;
  	for (int i = 0; i < n; i++) 
	{
    	monster temp = list[i];
		int h_index = i;
		int l_index = 0;
		int position = insertion_sort_find_position(list, l_index, h_index, &list[i], comparisons, use_name, use_weight);

    	for(int j = i; j > position; j--) 
		{
      		list[j] = list[j - 1];
			(*block_copies)++;
    	}
    	list[position] = temp;
		(*copies)++;
  	}
}

/* Shell for insertion sort. */

void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int block_copies = 0;
  int total_copies = 0;
  clock_t start_cpu, end_cpu;
  printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  start_cpu = clock();

  insertion_sort_internal(list, n, &comparisons, &total_copies, &block_copies, use_name, use_weight);

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d block copies (%d total copies).\n", comparisons, block_copies, total_copies);
  print_clocks(end_cpu - start_cpu);
  return;
}

/* Merge function for merge sort.  Merges sub-arrays [l1..h1] and [l2..h2]. */

void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2, int *comparisons, int *copies, int *block_copies, int *mallocs, int use_name, int use_weight)
{
  	int i = l1;
	int j = i;
   	monster* temp = malloc(sizeof(monster) * (h2 - i + 1));
   	memcpy(temp, list + i, (h2 - i + 1) * sizeof(monster));
   	(*block_copies)++;
   	(*mallocs)++;
   	
	while(l1 <= h1 && l2 <= h2) 
	{
    	if(compare_monsters(&temp[l1 - i], &temp[l2 - i], use_name, use_weight) == 1) 
		{
      		list[j] = temp[l1 - i];
      		l1++;
			j++;
			(*copies)++;
    	} 
		else
		{
      		list[j] = temp[l2 - i];
      		l2++;
			j++;
			(*copies)++;
    	}
    	(*comparisons)++;
   	}
   	while(l1 <= h1) 
	{
      	list[j] = temp[l1 - i];
      	l1++;
      	j++;
		(*copies)++;
   	}
   	while(l2 <= h2) 
	{
      	list[j] = temp[l2 - i];
      	l2++;
      	j++;
		(*copies)++;
   	}
   	free(temp);
}

/* Recursive function for merge sort. */

void merge_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *copies, int *block_copies, int *mallocs, int use_name, int use_weight)
{
    if(low_index >= high_index) //error early on if this check is not used
    {
        return;
    }
    int half = (low_index + (high_index - low_index) / 2);
   
    merge_sort_recursive(list, low_index, half, comparisons, copies,  block_copies,  mallocs,use_name, use_weight);
    merge_sort_recursive(list, (half + 1),  high_index, comparisons, copies,  block_copies,  mallocs, use_name,  use_weight);
    merge_sort_merge(list, low_index, half, (half + 1), high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
}

/* Implement merge sort. */

void merge_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int copies = 0;
  int block_copies = 0;
  int mallocs = 0;
  clock_t start_cpu, end_cpu;

  printf("Merge sort %d monsters...\n", n);

  start_cpu = clock();
  merge_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs,
                       use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
  print_clocks(end_cpu - start_cpu);
}

/* Recursive function for merge-insertion sort. */

void merge_insertion_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *copies, int *block_copies, int *mallocs, int use_name, int use_weight)
{
  	int i = (high_index - low_index + 1);
  	int half = (i / 2);
  	if(i <= 25) 
	{
    	insertion_sort_internal((list + low_index), i, comparisons, copies, block_copies, use_name, use_weight);
  	} 
	else 
	{
    	int mid = (high_index - half);

    	merge_insertion_sort_recursive(list, low_index, mid, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    	merge_insertion_sort_recursive(list, (mid + 1), high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    	merge_sort_merge(list, low_index, mid, (mid + 1), high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
  	}
}

/* Implement merge sort. */

void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int copies = 0;
  int block_copies = 0;
  int mallocs = 0;
  clock_t start_cpu, end_cpu;

  printf("Merge-insertion sort %d monsters...\n", n);

  start_cpu = clock();
  merge_insertion_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs,
                                 use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
  print_clocks(end_cpu - start_cpu);
}

/* Main program. */

void run_all_sorts(int n, int only_fast, int use_name, int use_weight) {
  monster *our_list = make_some_monsters(n);
  monster *our_unsorted_list = malloc(sizeof(monster) * n);

  printf("SORT SET: n = %d, %s, by %s\n\n", n, only_fast ? "fast sorts only" : "all sorts", use_name ? "name" : "weight");

  if(only_fast == 0) {
    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    bubble_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    selection_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    insertion_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);
  }

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  quick_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  merge_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  merge_insertion_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  printf("SORT SET COMPLETE\n\n");

  free(our_list);
  free(our_unsorted_list);
}

int main(void) {
  run_all_sorts(50, 0, 0, 1);
  run_all_sorts(50, 0, 1, 0);
  run_all_sorts(500, 0, 0, 1);
  run_all_sorts(500, 0, 1, 0);
  run_all_sorts(5000, 0, 0, 1);
  run_all_sorts(5000, 0, 1, 0);
  run_all_sorts(50000, 1, 0, 1);
  run_all_sorts(50000, 1, 1, 0);
  run_all_sorts(500000, 1, 0, 1);
  run_all_sorts(500000, 1, 1, 0);
}