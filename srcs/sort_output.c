#include "../includes/include.h"

int	nm_strcmp(const char *a, const char *b) {

	int i = 0;
	int j = 0;
	
	while (a && (a[i] != '\0' || b[j] != '\0')) {
        
        

        while (a[i] && !ft_isalpha(a[i]))
            i++;
        while (b[j] && !ft_isalpha(b[j]))
            j++;
        
        unsigned char c1 = (unsigned char)a[i];
        unsigned char c2 = (unsigned char)b[j];

        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        if (a[i] != '\0') i++;
        if (b[j] != '\0') j++;
    }
	return (a[i] - b[j]);
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
    
    if (sym_a->c < sym_b->c)
        return (-1);
    if (sym_a->c > sym_b->c)
        return (1);
    
    return 0;
}

int rev_compare_symbols(const void *a, const void *b) {

	t_nm_sym *sym_a = (t_nm_sym *)a;
    t_nm_sym *sym_b = (t_nm_sym *)b;

    int cmp = nm_strcmp(sym_a->name, sym_b->name);
    
    if (cmp != 0) {
        return -cmp;
    }

    if (sym_a->value < sym_b->value) 
        return 1;
    if (sym_a->value > sym_b->value)
        return -1;
    
    if (sym_a->c < sym_b->c)
        return (1);
    if (sym_a->c > sym_b->c)
        return (-1);
    
    return 0;
}