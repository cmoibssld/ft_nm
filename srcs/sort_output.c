#include "../includes/include.h"

int	nm_strcmp(const char *a, const char *b) {

	int i = 0;
	int j = 0;
	
	while (a[i] != '\0' || b[j] != '\0') {
        
        unsigned char c1 = (unsigned char)a[i];
        unsigned char c2 = (unsigned char)b[j];

        while (a[i] && (a[i] < 'A' || a[i] > 'z'))
		    i++;
	    while (b[j] && (b[j] < 'A' || b[j] > 'z'))
		    j++;
        
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        if (a[i] != '\0') i++;
        if (b[j] != '\0') j++;
    }
	return (strcmp(a, b));
}

int compare_symbols(const void *a, const void *b) {

	t_nm_sym *sym_a = (t_nm_sym *)a;
    t_nm_sym *sym_b = (t_nm_sym *)b;

	
    int cmp = nm_strcmp(sym_a->name, sym_b->name);
    
    if (cmp != 0) {
        return cmp;
    }

    if (sym_a->value < sym_b->value) 
        return -1;
    if (sym_a->value > sym_b->value)
        return 1;
    
    return 0;
}