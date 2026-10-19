#include "find_min_max.h"
#include <stddef.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
    struct MinMax result;
    result.min = 0;
    result.max = 0;
    
    // Проверка на корректность
    if (array == NULL || begin > end) {
        return result;
    }
    
    // Инициализация первым элементом
    result.min = array[begin];
    result.max = array[begin];
    
    // Поиск мин и макс
    for (unsigned int i = begin + 1; i <= end; i++) {
        if (array[i] < result.min) {
            result.min = array[i];
        }
        if (array[i] > result.max) {
            result.max = array[i];
        }
    }
    
    return result;
}
