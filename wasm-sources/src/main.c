#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define WASM_EXPORT __attribute__((visibility("default")))

typedef long int i32;
typedef unsigned short bignum_digit_t;

/* беззнаковое целое, имеющее в два раза больше бит, чем bignum_digit_t
для 32-х разрядных машин - unsiged int, для 64-х разрядных - unsigned long */
typedef unsigned int bignum_double_t;

/* беззнаковое целое, имеющее в два раза больше бит, чем bignum_digit_t
для 32-х разрядных машин - siged int, для 64-х разрядных - signed long */
typedef signed int bignum_signed_double_t;

/* кол-во бит в одном разряде числа */
#define BIGNUM_DIGIT_BITS (sizeof(bignum_digit_t) * 8)

/* маска для выделения разрядов числа */
#define BIGNUM_DIGIT_MASK (((bignum_double_t)1 << BIGNUM_DIGIT_BITS) - 1)

/* макрос, вычисляющий кол-во разрядов в числе по его размеру */
#define BIGNUM_DIGITS(x) (( x ) / sizeof(bignum_digit_t) )

/* макрос, вычисляющий размер числа по кол-ву разрядов в нем */
#define BIGNUM_SIZE(x) (( x ) * sizeof(bignum_digit_t) )

/* максимальный размер числа в байтах */
#define BIGNUM_MAX_SIZE 32

/* максимально допустимое кол-во разрядов в числе */
#define BIGNUM_MAX_DIGITS  (BIGNUM_MAX_SIZE / sizeof(bignum_digit_t))

/* преобразование числа в строку */
void bignum_tohex(bignum_digit_t* num, char* buff, int buffsize, int digits);

/* преобразование строки в число */
void bignum_fromhex(bignum_digit_t* num, char* str, int digits);

/* является ли число нулем */
int bignum_iszero(bignum_digit_t* num, int digits);

/* присвоить числу значение ноль */
void bignum_setzero(bignum_digit_t* num, int digits);

/* сравнение двух чисел: 0 => равны, 1 => a > b, -1 => a < b*/
int bignum_cmp(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* скопировать число */
void bignum_cpy(bignum_digit_t* to, bignum_digit_t* from,
    int to_digits, int from_digits);

/* прибавить b к a. digits - кол-во разрядов в числах */
void bignum_add(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* вычесть b из a. digits - кол-во разрядов в числах */
void bignum_sub(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* умножить a на b */
void bignum_mul(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* выполнить деление a на b, найти частное и (опционально) остаток.
детальное описание алгоритма приведено у Кнута, Уоррена и Вельшенбаха */
void bignum_div(bignum_digit_t* a, /* делимое */
    bignum_digit_t* b, /* делитель */
    bignum_digit_t* q, /* частное, может быть a, b или NULL */
    bignum_digit_t* r, /* частное, может быть a, b или NULL */
    int digits); /* кол-во разрядов в числах */

/* найти сумму a и b в поле вычетов по модулю m
использовать двойной размер буфера */
void bignum_madd(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* m, int digits);

/* найти разность a и b в поле вычетов по модулю m
использовать двойной размер буфера */
void bignum_msub(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* m, int digits);

/* найти произведение a на b в поле вычетов по модулю m
использовать двойной размер буфера */
void bignum_mmul(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* m, int digits);

/* найти элемент, обратный num в поле вычетов по модулю m */
void bignum_inv(bignum_digit_t* num, bignum_digit_t* m, int digits);

/* найти частное a и b в поле вычетов по модулю m
использовать двойной размер буфера */
void bignum_mdiv(bignum_digit_t* a, /* делимое */
    bignum_digit_t* b, /* делитель */
    bignum_digit_t* m, /* модуль */
    int digits); /* кол-во разрядов в числах */

#define ECCRYPT_BIGNUM_DIGITS BIGNUM_MAX_DIGITS

/* точка на эллиптической кривой */
struct eccrypt_point_t {
    bignum_digit_t x[ECCRYPT_BIGNUM_DIGITS]; /* координата x */
    bignum_digit_t y[ECCRYPT_BIGNUM_DIGITS]; /* координата y */
    int is_inf; /* является ли точка несобственной */
};

/* параметры кривой */
struct eccrypt_curve_t {
    bignum_digit_t a[ECCRYPT_BIGNUM_DIGITS]; /* коэффициенты уравнения     */
    bignum_digit_t b[ECCRYPT_BIGNUM_DIGITS]; /*     y^2 = x^3 + a*x + b    */
    bignum_digit_t m[ECCRYPT_BIGNUM_DIGITS]; /* в поле вычетов по модулю m */
    struct eccrypt_point_t g; /* генерирующая точка */
};

/* пара ключей */
struct eccrypt_keypair_t {
    bignum_digit_t priv[ECCRYPT_BIGNUM_DIGITS]; /* закрытый ключ */
    struct eccrypt_point_t pub; /* открытый ключ */
};

/* копирование точки */
void eccrypt_point_cpy(struct eccrypt_point_t* to, /* куда копируем */
    struct eccrypt_point_t* from); /* откуда */

/* сложение точек эллиптической кривой */
void eccrypt_point_add(struct eccrypt_point_t* rslt, /* сумма */
    struct eccrypt_point_t* p, /* первое слогаемое */
    struct eccrypt_point_t* q, /* второе слогаемое */
    struct eccrypt_curve_t* curve); /* параметры кривой */

/* умножение точек эллиптической кривой */
void eccrypt_point_mul(struct eccrypt_point_t* rslt, /* результат */
    struct eccrypt_point_t* p, /* точка */
    bignum_digit_t* k, /* множитель */
    struct eccrypt_curve_t* curve); /* параметры кривой */

/*
 *  (c) Alexandr A Alexeev 2010 | http://eax.me/
 */

 // TODO: использовать флаги знака для коэффииентов a и b в eccrypt_curve_t

 /* копирование точки */
void eccrypt_point_cpy(struct eccrypt_point_t* to, /* куда копируем */
    struct eccrypt_point_t* from) { /* откуда */
    if (to == from) return;
    if (to->is_inf = from->is_inf)
        return;

    memcpy(to->x, from->x, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS));
    memcpy(to->y, from->y, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS));
}

/* сложение точек эллиптической кривой */
void eccrypt_point_add(struct eccrypt_point_t* s, /* сумма */
    struct eccrypt_point_t* p, /* первое слогаемое */
    struct eccrypt_point_t* q, /* второе слогаемое */
    struct eccrypt_curve_t* curve) { /* параметры кривой */
    struct eccrypt_point_t rslt; /* вполне возможна ситуация s = p = q */
    bignum_digit_t lambda[ECCRYPT_BIGNUM_DIGITS]; /* коэффициент лямбда */
    struct eccrypt_point_t* tp; /* временный указатель */
    int equal = 1; /* предполагаем, что точки равны */

    /* проверка на бесконечность: P + O = O + P = P для любой точки поля */
    if (p->is_inf) { tp = p; p = q; q = tp; }
    if (q->is_inf) {
        eccrypt_point_cpy(s, p);
        return;
    }

    if ((p != q) && (equal = !bignum_cmp(p->x, q->x, ECCRYPT_BIGNUM_DIGITS)))
        /* адреса точек различаются, но координаты по x равны */
        if (!(equal = !bignum_cmp(p->y, q->y, ECCRYPT_BIGNUM_DIGITS))) {
            /* проверяем случай Q = -P */
            memcpy(rslt.x, p->y, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS));
            bignum_madd(rslt.x, q->y, curve->m, ECCRYPT_BIGNUM_DIGITS);
            if (bignum_iszero(rslt.x, ECCRYPT_BIGNUM_DIGITS)) {
                s->is_inf = 1;
                return;
            }
        }

    /* вычисляем лямбда */
    if (equal) {
        /* lambda = (3*x*x + a) / (2*y)  (mod m) */
        bignum_setzero(rslt.x, ECCRYPT_BIGNUM_DIGITS);   /* t := 2 */
        rslt.x[0] = 2;
        bignum_mmul(rslt.x, p->y, curve->m, ECCRYPT_BIGNUM_DIGITS); /* t *= y */
        bignum_setzero(lambda, ECCRYPT_BIGNUM_DIGITS);   /* l := 3 */
        lambda[0] = 3;
        bignum_mmul(lambda, p->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l *= x */
        bignum_mmul(lambda, p->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l *= x */
        bignum_madd(lambda, curve->a, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l += a */
    }
    else {
        /* lambda = (y1 - y2) / (x1 - x2)  (mod m) */
        memcpy(rslt.x, p->x, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* t := x1 */
        bignum_msub(rslt.x, q->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* t -= x2 */
        memcpy(lambda, p->y, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* l := y1 */
        bignum_msub(lambda, q->y, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l -= y2 */
    }
    bignum_mdiv(lambda, rslt.x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l /= t */

    /* x3 = lambda*lambda - x1 - x2  (mod m) */
    memcpy(rslt.x, lambda, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* x3 := l */
    bignum_mmul(rslt.x, rslt.x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* x3 *= l */
    bignum_msub(rslt.x, p->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* x3 -= x1 */
    bignum_msub(rslt.x, q->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* x3 -= x2 */

    /* y3 = lambda*(x1 - x3) - y1  (mod m) */
    memcpy(rslt.y, p->x, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* y3 := x1 */
    bignum_msub(rslt.y, rslt.x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* y3 -= x3 */
    bignum_mmul(rslt.y, lambda, curve->m, ECCRYPT_BIGNUM_DIGITS); /* y3 *= l */
    bignum_msub(rslt.y, p->y, curve->m, ECCRYPT_BIGNUM_DIGITS); /* y3 -= y1 */

    rslt.is_inf = 0; /* получили собственную точку */
    eccrypt_point_cpy(s, &rslt);
}

/* умножение точек эллиптической кривой */
void eccrypt_point_mul(struct eccrypt_point_t* rslt, /* результат */
    struct eccrypt_point_t* p, /* точка */
    bignum_digit_t* k, /* множитель */
    struct eccrypt_curve_t* curve) { /* параметры кривой */
    struct eccrypt_point_t point; /* копия точки */
    bignum_digit_t digit; /* значение текущего разряда множителя */
    int digit_num = 0; /* номер разряда */
    int bits = 0; /* кол-во необработанных бит в разряде */
    int n = ECCRYPT_BIGNUM_DIGITS; /* число значащих разрядов */

    if (p->is_inf) {
        rslt->is_inf = 1;
        return; /* n * O = O */
    }

    while ((n > 0) && !k[n - 1]) n--; /* определяем старший значащий разряд */
    if (n) eccrypt_point_cpy(&point, p); /* создаем копию точки */

    /* несобственная точка, раньше мы не могли менять rslt,
    так как возможна ситуация rslt == p */
    rslt->is_inf = 1;

    /* пока есть необработанные биты */
    while ((digit_num < n) || digit) {
        if (digit_num) /* это итерация не первая по счету */
            eccrypt_point_add(&point, &point, &point, curve); /* point *= 2 */

        if (!bits) { /* текущий разряд уже обработан или еще не инициализирован */
            digit = k[digit_num++];
            bits = sizeof(bignum_digit_t) * 8;
        }

        if (digit & 1)
            eccrypt_point_add(rslt, rslt, &point, curve); /* rslt += point */

        digit >>= 1;
        bits--;
    }
}

/*
 *  (c) Alexandr A Alexeev 2010 | http://eax.me/
 */

 /* преобразование числа в строку */
void bignum_tohex(bignum_digit_t* num, char* buff, int buffsize, int digits) {
    static char hextbl[] = "0123456789ABCDEF";
    int pos, offset = (sizeof(bignum_digit_t) << 1) - 1;

    while ((digits > 0) && !num[digits - 1]) /* ищем первый значащий разряд */
        digits--;

    if (!digits) {
        if (buffsize >= 2) {
            buff[0] = '0';
            buff[1] = 0;
        }
        return;
    }

    pos = digits - 1;

    while (!((num[pos] >> (offset << 2)) & 0xF))
        offset--;

    while ((pos >= 0) && (buffsize > 1)) {
        *buff++ = hextbl[(num[pos] >> (offset-- << 2)) & 0xF];
        buffsize--;
        if (offset < 0) {
            offset = sizeof(bignum_digit_t) * 2 - 1;
            pos--;
        }
    }

    *buff = 0;
}

/* преобразование строки в число */
void bignum_fromhex(bignum_digit_t* num, char* str, int digits) {
    int pos = 0, offset = 0, len = strlen(str);
    char c;

    memset(num, 0, BIGNUM_SIZE(digits));
    //num = (bignum_digit_t*) malloc(BIGNUM_SIZE(digits));

    while (len && (pos < digits)) {
        c = str[--len];
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'F')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'f')
            c -= 'a' - 10;
        else return;

        num[pos] = num[pos] | (c << (offset++ << 2));

        if (offset == sizeof(bignum_digit_t) * 2) {
            offset = 0;
            pos++;
        }
    }
}

/* является ли число нулем */
int bignum_iszero(bignum_digit_t* num, int digits) {
    while (digits--)
        if (num[digits])
            return 0; /* не ноль */
    return 1;
}

/* присвоить числу значение ноль */
void bignum_setzero(bignum_digit_t* num, int digits) {
    memset(num, 0, BIGNUM_SIZE(digits));
}

/* сравнение двух чисел: 0 => равны, 1 => a > b, -1 => a < b*/
int bignum_cmp(bignum_digit_t* a, bignum_digit_t* b, int digits) {
    while (digits--)
        if (a[digits] > b[digits])
            return 1;
        else if (a[digits] < b[digits])
            return -1;
    return 0;
}

/* скопировать число */
void bignum_cpy(bignum_digit_t* to, bignum_digit_t* from,
    int to_digits, int from_digits) {
    int i = 0;
    while ((i < to_digits) && (i < from_digits)) {
        to[i] = from[i];
        i++;
    }

    while (i < to_digits)
        to[i++] = 0;
}

/* прибавить b к a. digits - кол-во разрядов в числах */
void bignum_add(bignum_digit_t* a, bignum_digit_t* b, int digits) {
    int i;
    bignum_double_t carry = 0;

    for (i = 0; i < digits; i++)
        a[i] = carry = ((bignum_double_t)a[i] + b[i] + (carry >> BIGNUM_DIGIT_BITS));
}

/* вычесть b из a. digits - кол-во разрядов в числах */
void bignum_sub(bignum_digit_t* a, bignum_digit_t* b, int digits) {
    bignum_double_t borrow = 0;
    int i;

    for (i = 0; i < digits; i++)
        a[i] = borrow = ((bignum_double_t)a[i] - b[i] - (borrow >> (BIGNUM_DIGIT_BITS * 2 - 1)));
}

/* найти произведение a на b и записать результат в result
result должен состоять из 2*digits разрядов
эта функция используется только внутри модуля */
void bignum_mul_raw(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* result, int digits) {
    bignum_double_t carry;
    int i, j;

    bignum_setzero(result, BIGNUM_MAX_DIGITS * 2);

    for (i = 0; i < digits; i++) {
        carry = 0;
        for (j = 0; j < digits; j++)
            /* без приведения типа a[i] умножение не будет работать на amd64 */
            result[i + j] = carry = (result[i + j] + (bignum_double_t)a[i] * b[j] +
                (carry >> BIGNUM_DIGIT_BITS));

        result[i + digits] = carry >> BIGNUM_DIGIT_BITS;
    }
}

/* умножить a на b */
void bignum_mul(bignum_digit_t* a, bignum_digit_t* b, int digits) {
    bignum_digit_t result[BIGNUM_MAX_DIGITS * 2]; /* произведение */
    bignum_mul_raw(a, b, result, digits);
    bignum_cpy(a, result, digits, BIGNUM_MAX_DIGITS * 2);
}

/* выполнить деление a на b, найти частное и остаток
для корректной работы функции mmul мы вынуждены поддерживать числа,
содержащие BIGNUM_MAX_DIGITS*2 разрядов */
void bignum_div(bignum_digit_t* a, /* делимое */
    bignum_digit_t* b, /* делитель */
    bignum_digit_t* q, /* частное, может быть a, b или NULL */
    bignum_digit_t* r, /* остаток, может быть a, b или NULL */
    int digits) { /* кол-во разрядов в числах */
    bignum_digit_t td[BIGNUM_MAX_DIGITS * 2]; /* нормализованный делитель */
    bignum_digit_t tq[BIGNUM_MAX_DIGITS * 2]; /* частное */
    bignum_digit_t tr[BIGNUM_MAX_DIGITS * 2 + 1]; /* нормализованный остаток */
    bignum_double_t qhat, rhat, product, carry; /* произведение двух чисел */
    bignum_signed_double_t temp;
    int i, j, n, shift = 0;

    bignum_setzero(tq, BIGNUM_MAX_DIGITS * 2);

    /* определяем старший ненулевой разряд делителя */
    n = digits;
    while ((n > 1) && !b[n - 1]) n--;

    if (n == 1) {
        /* делитель имеет всего один разряд - производим деление простым способом.
        это не оптимизация - оставшаяся часть алгоритма требует, чтобы делитель имел
        хотябы два разряда */
        carry = 0;
        for (j = digits - 1; j >= 0; j--) {
            tq[j] = ((carry << BIGNUM_DIGIT_BITS) | a[j]) / b[0];
            carry = ((carry << BIGNUM_DIGIT_BITS) | a[j]) - tq[j] * b[0];
        }

        if (q) /* сохраняем частное */
            bignum_cpy(q, tq, digits, BIGNUM_MAX_DIGITS * 2);

        if (r) {
            bignum_setzero(r, digits);
            r[0] = carry; /* сохраняем остаток */
        }
        return;
    }

    /* определяем shift - на сколько бит влево нужно сдвиднуть
    делитель, чтобы старший разряд стал равен единице */
    while (!((b[n - 1] << shift) & (1 << (BIGNUM_DIGIT_BITS - 1))))
        shift++;

    bignum_setzero(td, BIGNUM_MAX_DIGITS * 2);
    bignum_setzero(tr, BIGNUM_MAX_DIGITS * 2 + 1);

    /* на amd64 тип bignum_digit_t представляет собой int. при shift = 0
    a[digits - 1] >> 32 == a[digits - 1], вот почему необходимо приведение типа */
    tr[digits] = (bignum_double_t)a[digits - 1] >> (BIGNUM_DIGIT_BITS - shift);
    for (i = digits - 1; i > 0; i--)
        tr[i] = (a[i] << shift) | ((bignum_double_t)a[i - 1] >> (BIGNUM_DIGIT_BITS - shift));
    tr[0] = a[0] << shift;

    /* производим нормализацию делителя */
    for (i = n - 1; i > 0; i--)
        td[i] = (b[i] << shift) | ((bignum_double_t)b[i - 1] >> (BIGNUM_DIGIT_BITS - shift));
    td[0] = b[0] << shift;

    for (j = digits - n; j >= 0; j--) { /* главный цикл */
      /* вычисляем оценку j-го разряда частного и соответсвующего остатка */
        qhat = (((bignum_double_t)tr[j + n] << BIGNUM_DIGIT_BITS) | tr[j + n - 1]) / td[n - 1];
        rhat = (((bignum_double_t)tr[j + n] << BIGNUM_DIGIT_BITS) | tr[j + n - 1]) - qhat * td[n - 1];

        while ((qhat >= ((bignum_double_t)1 << BIGNUM_DIGIT_BITS)) ||
            (qhat * td[n - 2] > ((rhat << BIGNUM_DIGIT_BITS) | tr[j + n - 2]))) {
            qhat--;
            rhat += td[n - 1];
            if (rhat >= ((bignum_double_t)1 << BIGNUM_DIGIT_BITS))
                break;
        }

        carry = 0; /* умножение и вычитание */
        for (i = 0; i < n; i++) {
            tr[i + j] = temp = tr[i + j] - carry - ((product = qhat * td[i]) & BIGNUM_DIGIT_MASK);
            carry = (product >> BIGNUM_DIGIT_BITS) - (temp >> BIGNUM_DIGIT_BITS);
        }

        tr[j + n] = temp = tr[j + n] - carry;
        tq[j] = qhat; /* сохраняем разряд частного */
        if (temp < 0) {
            /* вычли слишком много - возвращаем назад. из-за этого сравнения t должен
            иметь знаковый тип. данное условие выполняеся крайне редко -
            пример чисел есть в Вельшенбахе */
            tq[j]--; carry = 0;
            for (i = 0; i < n; i++) {
                /* преобразование типов здесь необходимо для amd64 */
                tr[i + j] = temp = (bignum_double_t)tr[i + j] + td[i] + carry;
                carry = temp >> BIGNUM_DIGIT_BITS;
            }
            tr[j + n] += carry;
        }
    } /* конец главного цикла */

    if (q) /* сохраняем частное */
        bignum_cpy(q, tq, digits, BIGNUM_MAX_DIGITS * 2);

    if (r) { /* денормализуем остаток и сохраняем его */
        bignum_setzero(r, digits);
        for (i = 0; i < n; i++)
            r[i] = (tr[i] >> shift) | ((bignum_double_t)tr[i + 1] << (BIGNUM_DIGIT_BITS - shift));
    }
}

/* найти сумму a и b в поле вычетов по модулю m */
void bignum_madd(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* m, int digits) {
    bignum_digit_t ta[BIGNUM_MAX_DIGITS + 1];
    bignum_digit_t tb[BIGNUM_MAX_DIGITS + 1];
    bignum_digit_t tm[BIGNUM_MAX_DIGITS + 1];

    bignum_cpy(ta, a, digits + 1, digits);
    bignum_cpy(tb, b, digits + 1, digits);
    bignum_cpy(tm, m, digits + 1, digits);

    bignum_add(ta, tb, digits + 1);
    if (bignum_cmp(ta, tm, digits + 1) >= 0) /* a >= m */
        bignum_sub(ta, tm, digits + 1);

    bignum_cpy(a, ta, digits, digits + 1);
}

/* найти разность a и b в поле вычетов по модулю m */
void bignum_msub(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* m, int digits) {
    /* здесь может возникнуть целочисленное переполнение,
    но нас это нисколько не беспкоит */
    if (bignum_cmp(a, b, digits) < 0) /* a < b */
        bignum_add(a, m, digits);
    bignum_sub(a, b, digits);
}

/* найти произведение a на b в поле вычетов по модулю m */
void bignum_mmul(bignum_digit_t* a, bignum_digit_t* b,
    bignum_digit_t* m, int digits) {
    bignum_digit_t ta[BIGNUM_MAX_DIGITS * 2];
    bignum_digit_t tm[BIGNUM_MAX_DIGITS * 2];

    bignum_cpy(tm, m, digits << 1, digits);
    bignum_mul_raw(a, b, ta, digits);

    if (bignum_cmp(ta, tm, digits << 1) >= 0) /* a >= m */
        bignum_div(ta, tm, 0, ta, digits << 1);

    bignum_cpy(a, ta, digits, digits << 1);
}

/* найти элемент, мультипликативно обратный к num в поле вычетов по модулю m */
void bignum_inv(bignum_digit_t* num, bignum_digit_t* m, int digits) {
    /* r, q, d, d1 = 1, d2 = 0, u = m, v = num; */
    bignum_digit_t r[BIGNUM_MAX_DIGITS]; bignum_digit_t q[BIGNUM_MAX_DIGITS];
    bignum_digit_t d[BIGNUM_MAX_DIGITS]; bignum_digit_t d1[BIGNUM_MAX_DIGITS];
    bignum_digit_t d2[BIGNUM_MAX_DIGITS]; bignum_digit_t u[BIGNUM_MAX_DIGITS];
    bignum_digit_t* pu = u, * pv = num, * pr = r, * pd = d, * pd1 = d1, * pd2 = d2, * pt;

    bignum_cpy(u, m, digits, digits);
    bignum_setzero(d2, digits);
    bignum_setzero(d1, digits);
    d1[0]++;

    while (!bignum_iszero(pv, digits)) { /* while(v != 0) */
      /* r = u % v; q = (u - r) / v; */
        bignum_div(pu, pv, q, pr, digits);

        /* d = d2 - q*d1 (mod m) */
        bignum_mmul(q, pd1, m, digits);
        bignum_cpy(pd, pd2, digits, digits);
        bignum_msub(pd, q, m, digits);

        /* u = v; v = r; d2 = d1; d1 = d; */
        pt = pu; pu = pv; pv = pr; pr = pt;
        pt = pd2; pd2 = pd1; pd1 = pd; pd = pt;
    }

    /* если u = 1, то d2 - число, обратное num в кольце вычетов по модулю m
    иначе - обратного элемента не сущетсвует */
    if (pd2 != num) bignum_cpy(num, pd2, digits, digits);
}

/* найти частное a и b в поле вычетов по модулю m */
void bignum_mdiv(bignum_digit_t* a, /* делимое */
    bignum_digit_t* b, /* делитель */
    bignum_digit_t* m, /* модуль */
    int digits) { /* кол-во разрядов в числах */
    bignum_digit_t binv[BIGNUM_MAX_DIGITS];

    /* находим b ^ -1 - элемент, мультипликативно обратный к b */
    bignum_cpy(binv, b, digits, digits);
    bignum_inv(binv, m, digits);

    /* умнажаем a на (b ^ -1) */
    bignum_mmul(a, binv, m, digits);
}

WASM_EXPORT
void print(i32 *plaintext,const i32 plaintextLength){
char* cparam_a = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF40", //а
         buff[128],
         buff1[128],
        * cparam_b = "77CE6C1515F3A8EDD2C13AABE4D8FBBE4CF55069978B9253B22E7D6BD69C03F1", //b
        * cparam_p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF43", //module
        * cparam_gx= "0000000000000000000000000000000000000000000000000000000000000000", //x of gen point
        * cparam_gy= "6BF7FC3CFB16D69F5CE4C9A351D6835D78913966C408F6521E29CF1804516A93", //y of gen point
        //* cpoint_x = "bd1a5650179d79e03fcee49d4c2bd5ddf54ce46d0cf11e4ff87bf7a890857fd0", //x of point
        //* cpoint_y = "7ac6a60361e8c8173491686d461b2826190c2eda5909054a9ab84d2ab9d99a90",
        * alphabet = "0123456789ABCDEF:", //y of point
        numk[plaintextLength]; //module (порядок)

    for (int i = 0; i < plaintextLength; i++) {
      if (alphabet[plaintext[i]] == ':'){
        break;
      }

      numk[i] = alphabet[plaintext[i]];
    }

    struct eccrypt_curve_t curve;
    struct eccrypt_point_t p1, rslt;
    bignum_digit_t k[ECCRYPT_BIGNUM_DIGITS];

    bignum_fromhex(curve.a, cparam_a, ECCRYPT_BIGNUM_DIGITS);
    bignum_fromhex(curve.b, cparam_b, ECCRYPT_BIGNUM_DIGITS);
    bignum_fromhex(curve.m, cparam_p, ECCRYPT_BIGNUM_DIGITS);

   // bignum_fromhex(curve.g.x, cparam_gx, ECCRYPT_BIGNUM_DIGITS);
   // bignum_fromhex(curve.g.y, cparam_gy, ECCRYPT_BIGNUM_DIGITS);
    curve.g.is_inf = 0;
    bignum_fromhex(p1.x, cparam_gx, ECCRYPT_BIGNUM_DIGITS);
    bignum_fromhex(p1.y, cparam_gy, ECCRYPT_BIGNUM_DIGITS);
    p1.is_inf = bignum_iszero(p1.x, ECCRYPT_BIGNUM_DIGITS) &&
        bignum_iszero(p1.y, ECCRYPT_BIGNUM_DIGITS);

    bignum_fromhex(k, numk, ECCRYPT_BIGNUM_DIGITS);
    eccrypt_point_mul(&rslt, &p1, k, &curve);
    
    if (rslt.is_inf) {
    }
    else {
        int count = sizeof(buff);
        bignum_tohex(rslt.x, buff, sizeof(buff), ECCRYPT_BIGNUM_DIGITS);
        
        for (int i = 0; i<64; i++){
          for (int j = 0; j<strlen(alphabet); j++){
            if (buff[i] == alphabet[j]){
              plaintext[i] = j;
            }
          }
        }

        bignum_tohex(rslt.y, buff1, sizeof(buff1), ECCRYPT_BIGNUM_DIGITS);

        for (int i = 65; i<129; i++){
          for (int j = 0; j<strlen(alphabet); j++){
              if (buff1[i-65] == alphabet[j]){
                plaintext[i] = j;
                break;
              }
          }
        }
    }
}
