#include "bigint.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "../src-given/random.c"

int bigintToBinaryString(bigint *n, char *string);
int roundUp(int length, int multiple);

struct bigIntCalc {
    big_uint result;
    int carry;
};

struct bigIntCalcMult {
    big_uint result;
    big_uint carry;
};

void big_init(bigint *X) {
    X->signum = 0;
    X->num_limbs = 0;
    X->data = NULL;
}

void big_free(bigint *X) {
    if (X != NULL && X->data != NULL) {
        free(X->data);
    }
}

int big_copy(bigint *X, const bigint *Y) {
    X->signum = Y->signum;
    X->num_limbs = Y->num_limbs;
    X->data = (big_uint *) realloc(X->data, Y->num_limbs * sizeof(big_uint));

    if (X->data == NULL) {
        return ERR_BIGINT_ALLOC_FAILED;
    }
    memcpy(X->data, Y->data, sizeof(big_uint) * Y->num_limbs);

    return 0;
}

int toBinaryString(big_uint n, char *string) {
  int num_bits = sizeof(big_uint) * 8;
  if (!string) {
    return NULL;
  }
  for (int i = num_bits - 1; i >= 0; i--) {
    string[i] = (n & 1) + '0';
    n >>= 1;
  }
  string[num_bits] = '\0';
  return 0;
}

size_t big_bitlen(const bigint *X) {
    
    char output[X->num_limbs * 64 + 1];

    bigintToBinaryString(X, output);

    size_t bitlen = strlen(output);
    return (size_t) bitlen;
}

int print_bigint(const bigint *p) {
    char output[500];
    size_t size;
    big_write_string(p, output, 500, &size);
}

int print_bigint_all(const bigint *p) {
    for (size_t i=0; i < p->num_limbs; i++) {
        printf("Limb %zu is %llu\n", i, p->data[i]);
    }
    char output[500];
    size_t size;
    big_write_string(p, output, 500, &size);
    printf("%s\n", output);
    printf("Size of buffer is %zu\n", size);
    printf("Sign is %d\n", p->signum);
}

size_t big_size(const bigint *X) {
    size_t round = roundUp(big_bitlen(X), 8);
    return (size_t) (round / 8);
}

int big_set_nonzero(bigint *X, big_uint limb) {
    X->data = (big_uint *) realloc(X->data, sizeof(big_uint));

    if (X->data == NULL) {
        return ERR_BIGINT_ALLOC_FAILED;
    }

    X->signum = 1;
    X->num_limbs = 1;
    X->data[0] = limb;
    return 0;
}

int roundUp(int length, int multiple)
{
    if (multiple == 0)
        return length;

    int remainder = length % multiple;
    if (remainder == 0)
        return length;

    return length + multiple - remainder;
}

int big_read_string(bigint *X, const char *s) {
    if (s[0] == '-') {
        X->signum = -1;
        s += 1;
    }
    else {
        X->signum = 1;
    }
    int paddedLength = roundUp(strlen(s), 16);
    char newStr[paddedLength + 1];
    for (size_t i=0; i < paddedLength - strlen(s); i++) {
        newStr[i] = '0';
    }
    for (size_t j=0; j < strlen(s); j++) {
        if (!isxdigit(s[j])) {
            return ERR_BIGINT_INVALID_CHARACTER;
        }
        newStr[j + (paddedLength - strlen(s))] = s[j];
    }
    newStr[paddedLength] = '\0';

    size_t max_length = (size_t) paddedLength / 16;
    X->data = (big_uint *) realloc(X->data, sizeof(big_uint) * max_length);
    X->num_limbs = max_length;

    size_t curr_limb = 0;
    for (size_t i=max_length; i > 0; i--) {
        char tmp[17];
        strncpy(tmp, newStr + (i - 1) * 16, 16);
        tmp[16] = '\0';
        big_uint result = strtoull(tmp, NULL, 16);
        X->data[curr_limb] = (big_uint) result;
        curr_limb += 1;
    }

    while (X->data[X->num_limbs - 1] == 0 && X->num_limbs != 1) {
        X->data = (big_uint *) realloc(X->data, sizeof(big_uint) * (X->num_limbs - 1));
        X->num_limbs -= 1;
    }
    return 0;
}


int big_write_string(const bigint *X,
                     char *buf, size_t buflen, size_t *olen) {

    if (X->num_limbs == 1 && X->data[0] == 0) {
        *olen = 2;
        *buf++ = '0';
        *buf++ = '\0';
        return 0;
    }

    char hexString[16 * X->num_limbs + 1];
    
    size_t curr_size = X->num_limbs - 1;
    for (size_t i=0; i < X->num_limbs ; i++) {
        big_uint tmp = X->data[i];
        for (int j=15; j >= 0; j--) {
            int ch = tmp % 16;
            tmp = tmp / 16;
            
            if (ch < 10) 
                ch = ch + 48; 
            else
                ch = ch + 55; 
            hexString[(curr_size) * 16 + j] = (char) tolower(ch);
        }
        curr_size--;
    }

    hexString[16 * X->num_limbs] = '\0';

    char *start = hexString;
    bool msb = false;
    size_t actual_size = 16 * X->num_limbs;

    for (size_t i=0; i < 16 * X->num_limbs; i++) {
        if (!msb && hexString[i] == '0') {
            start += 1;
            actual_size--;
        }
        else if (!msb && hexString[i] != '0') {
            msb = true;
            break;
        }
    }

    size_t loop_size = actual_size;
    size_t bits_written = 0;
    if (X->signum == -1) {
        actual_size++;
        *buf++ = '-';
        bits_written += 1;
    }
    for (size_t i = 0; i < loop_size; i++) {
        if (bits_written + 1 >= buflen) {
            *olen = actual_size + 1;
            *buf = '\0';
            return ERR_BIGINT_BUFFER_TOO_SMALL;
        }
        *buf++ = *start++;
        bits_written += 1;
    }
    *buf = '\0';
    *olen = actual_size + 1;
    
    return 0;
}

int big_read_binary(bigint *X, const uint8_t *buf, size_t buflen) {
    size_t roundedLen = roundUp(buflen, 8);
    size_t bufferzero = roundedLen - buflen;
    uint8_t temp[roundedLen];

    for (size_t i=0; i < bufferzero; i++) {
        temp[i] = (uint8_t) 0;
    }

    for (size_t i=bufferzero; i<roundedLen; i++) {
        temp[i] = buf[i - bufferzero];
    }


    X->data = (big_uint *) realloc(X->data, sizeof(big_uint) * (roundedLen / 8));
    X->num_limbs = (roundedLen / 8);
    X->signum = 1;

    size_t curr_limb = (roundedLen / 8) - 1;
    for (size_t i=0; i < (roundedLen / 8); i++) {
        for (size_t j=0; j < 8; j++) {
            X->data[curr_limb] = (X->data[curr_limb] << 8) | temp[(i) * 8 + j];
        }
        curr_limb--;
    }

    return 0;
}

int big_write_binary(const bigint *X, uint8_t *buf, size_t buflen) {
    if (X->num_limbs == 0) {
        for (size_t i=0; i < buflen; i++) {
            buf[i] = 0;
        }
        return 0;
    }
    uint8_t temp[X->num_limbs * 8];

    size_t curr_limbs = X->num_limbs-1;
    for (size_t i=0; i < X->num_limbs; i++) {
        for (size_t j=8; j > 0; j--) {
            temp[i * 8 + (8 - j)] = (X->data[curr_limbs] >> (8 * (j - 1)));
        }
        curr_limbs--;
    }

    size_t start = 0;
    while (temp[start] == 0 && start < X->num_limbs * 8 - 1) {
        start += 1;
    }

    size_t tmp_length = X->num_limbs * 8 - start;

    if (buflen < tmp_length) {
        return ERR_BIGINT_BUFFER_TOO_SMALL;
    }
    else {
        size_t buffer_zeros = buflen - tmp_length;
        for (size_t i=0; i < buffer_zeros; i++) {
            buf[i] = (uint8_t) 0;
        }
        size_t buf_start = buffer_zeros;
        for (size_t i=start; i < X->num_limbs * 8; i++) {
            buf[buf_start] = temp[i];
            buf_start++;
        }
    }

    return 0;
}

big_uint max(big_uint A, big_uint B) {
    if (A > B) {
        return A;
    }
    else {
        return B;
    }
}

struct bigIntCalc big_add_1x1(big_uint a, big_uint b, int c) {
    int carry = 0;
    big_udbl base = (big_udbl) 1 << 64;
    big_udbl s1 = (big_udbl) ((a + b + c) % base);
    if (s1 < a || s1 < b) {
        carry = 1;
    }
    else {
        carry = 0;
    }
    return (struct bigIntCalc) {.result = s1, .carry = carry};
}

int big_add(bigint *X, const bigint *A, const bigint *B) {
    if (A->signum == 1 && B->signum == -1) {
        bigint B_pos;
        big_init(&B_pos);
        big_copy(&B_pos, B);
        (&B_pos)->signum = 1;
        big_sub(X, A, &B_pos);
        big_free(&B_pos);
    }
    else if (A->signum == -1 && B->signum == 1) {
        bigint A_pos;
        big_init(&A_pos);
        big_copy(&A_pos, A);
        (&A_pos)->signum = 1;
        big_sub(X, B, &A_pos);
        big_free(&A_pos);
    }
    else {
        if (A->signum == 1 && B->signum == 1) {
            X->signum = 1;
        }
        else if (A->signum == -1 && B->signum == -1) {
            X->signum = -1;
        }
        size_t new_num_limbs = max(A->num_limbs, B->num_limbs);

        X->data = realloc(X->data, sizeof(big_uint) * new_num_limbs);

        X->num_limbs = new_num_limbs;

        big_uint added_result[new_num_limbs];
        int c = 0;
        for (size_t i=0 ; i < new_num_limbs; i++) {
            struct bigIntCalc result;
            if (i < A->num_limbs && i < B->num_limbs) {
                result = big_add_1x1(A->data[i], B->data[i], c);
            }
            else if (i >= A->num_limbs) {
                result = big_add_1x1((big_uint) 0, B->data[i], c);
            }
            else if (i >= B->num_limbs) {
                result = big_add_1x1(A->data[i], (big_uint) 0, c);
            }
            c = result.carry;
            X->data[i] = result.result;
        }
        if (c != 0) {
            X->data = realloc(X->data, sizeof(big_uint) * (new_num_limbs + 1));
            X->data[new_num_limbs] = c;
            X->num_limbs = new_num_limbs + 1;
        }
    }
    return 0;
}

struct bigIntCalc big_sub_1x1(big_uint a, big_uint b, int c) {
    int carry = 0;
    big_uint s;

    if (b == 0 && c != 0) {
        s = a + c;
        if (a == 0) {
            carry = -1;
        }
        else {
            carry = 0;
        }
    }
    else if (a == b) {
        if (c == -1) {
            s = UINT64_MAX;
            carry = -1;
        }
        else {
            s = 0;
            carry = 0;
        }
    }
    else if (a > b + c) {
        s = a - b + c;
        carry = 0;
    }
    else {
        s = a + (UINT64_MAX - b) + c + 1;
        carry = -1;
    }
    return (struct bigIntCalc) {.result = s, .carry = carry};
}

int big_sub(bigint *X, const bigint *A, const bigint *B) {
    if (A->signum == 1 && B->signum == -1) {
        bigint B_pos;
        big_init(&B_pos);
        big_copy(&B_pos, B);
        (&B_pos)->signum = 1;
        big_add(X, A, &B_pos);
        big_free(&B_pos);
    }
    else if (A->signum == -1 && B->signum == 1) {
        bigint A_pos;
        big_init(&A_pos);
        big_copy(&A_pos, A);
        (&A_pos)->signum = 1;
        big_add(X, &A_pos, B);
        X->signum = -1;
        big_free(&A_pos);
    }
    else if (A->signum == -1 && B->signum == -1) {
        bigint A_pos;
        bigint B_pos;
        big_init(&A_pos);
        big_init(&B_pos);
        big_copy(&A_pos, A);
        big_copy(&B_pos, B);
        (&A_pos)->signum = 1;
        (&B_pos)->signum = 1;
        big_sub(X, &B_pos, &A_pos);
        big_free(&A_pos);
        big_free(&B_pos);
    }
    else {
        int cmp = big_cmp(A, B);
        if (cmp == 0) {
            X->data = realloc(X->data, sizeof(big_uint));
            X->signum = 1;
            X->data[0] = (big_uint) 0;
            X->num_limbs = 1;
        }
        else if (cmp == 1) {
            size_t new_num_limbs = max(A->num_limbs, B->num_limbs);

            X->data = realloc(X->data, sizeof(big_uint) * new_num_limbs);
            X->num_limbs = new_num_limbs;

            big_uint added_result[new_num_limbs];
            int c = 0;
            for (size_t i=0; i < new_num_limbs; i++) {
                struct bigIntCalc result;
                if (i < A->num_limbs && i < B->num_limbs) {
                    result = big_sub_1x1(A->data[i], B->data[i], c);
                }
                else if (i >= A->num_limbs) {
                    result = big_sub_1x1((big_uint) 0, B->data[i], c);
                }
                else if (i >= B->num_limbs) {
                    result = big_sub_1x1(A->data[i], (big_uint) 0, c);
                }
                c = result.carry;
                X->data[i] = result.result;
            }
            while (X->data[X->num_limbs - 1] == 0 && X->num_limbs != 1) {
                X->data = (big_uint *) realloc(X->data, sizeof(big_uint) * (X->num_limbs - 1));
                X->num_limbs -= 1;
            }
            X->signum = 1;
        }
        else {
            big_sub(X, B, A);
            X->signum = -1;
            if (X->num_limbs == 1 && X->data[0] == 0) {
                X->signum = 1;
            }
        }
    }
    return 0;
}

int big_cmp(const bigint *x, const bigint *y) {
    bigint x_copy;
    bigint y_copy;

    big_init(&x_copy);
    big_init(&y_copy);

    big_copy(&x_copy, x);
    big_copy(&y_copy, y);

    while ((&x_copy)->data[(&x_copy)->num_limbs - 1] == 0 && (&x_copy)->num_limbs != 1) {
        (&x_copy)->data = (big_uint *) realloc((&x_copy)->data, sizeof(big_uint) * ((&x_copy)->num_limbs - 1));
        (&x_copy)->num_limbs -= 1;
    }

    while ((&y_copy)->data[(&y_copy)->num_limbs - 1] == 0 && (&y_copy)->num_limbs != 1) {
        (&y_copy)->data = (big_uint *) realloc((&y_copy)->data, sizeof(big_uint) * ((&y_copy)->num_limbs - 1));
        (&y_copy)->num_limbs -= 1;
    }

    if ((&x_copy)->num_limbs == 1 && (&x_copy)->data[0] == 0 && (&y_copy)->num_limbs == 1 && (&y_copy)->data[0] == 0) {
        big_free(&x_copy);
        big_free(&y_copy);
        return 0;
    }

    if ((&x_copy)->signum == 1 && (&y_copy)->signum == -1) {
        big_free(&x_copy);
        big_free(&y_copy);
        return 1;
    }
    else if ((&x_copy)->signum == -1 && (&y_copy)->signum == 1) {
        big_free(&x_copy);
        big_free(&y_copy);
        return -1;
    }
    else if ((&x_copy)->signum == 1 && (&y_copy)->signum == 1) {
        if ((&x_copy)->num_limbs > (&y_copy)->num_limbs) {
            big_free(&x_copy);
            big_free(&y_copy);
            return 1;
        }
        else if ((&x_copy)->num_limbs < (&y_copy)->num_limbs) {
            big_free(&x_copy);
            big_free(&y_copy);
            return -1;
        }
        else {
            for (size_t i = (&x_copy)->num_limbs; i > 0; i -= 1) {
                if ((&x_copy)->data[i - 1] > (&y_copy)->data[i - 1]) {
                    big_free(&x_copy);
                    big_free(&y_copy);
                    return 1;
                }
                else if ((&x_copy)->data[i - 1] < (&y_copy)->data[i - 1]) {
                    big_free(&x_copy);
                    big_free(&y_copy);
                    return -1;
                }
            }
        }
    }
    else if ((&x_copy)->signum == -1 && (&x_copy)->signum == -1) {
        if ((&x_copy)->num_limbs > (&y_copy)->num_limbs) {
            big_free(&x_copy);
            big_free(&y_copy);
            return -1;
        }
        else if ((&x_copy)->num_limbs < (&y_copy)->num_limbs) {
            big_free(&x_copy);
            big_free(&y_copy);
            return 1;
        }
        else {
            for (size_t i = (&x_copy)->num_limbs; i > 0; i -= 1) {
                if ((&x_copy)->data[i - 1] > (&y_copy)->data[i - 1]) {
                    big_free(&x_copy);
                    big_free(&y_copy);
                    return -1;
                }
                else if ((&x_copy)->data[i - 1] < (&y_copy)->data[i - 1]) {
                    big_free(&x_copy);
                    big_free(&y_copy);
                    return 1;
                }
            }
        }
    }
    big_free(&x_copy);
    big_free(&y_copy);
    return 0;
}

struct bigIntCalcMult bigMult1x1(big_uint a, big_uint b, big_uint carry, big_uint prev) {
    big_udbl a_copy = (big_udbl) a;
    big_udbl b_copy = (big_udbl) b;
    big_udbl carry_copy = (big_udbl) carry;
    big_udbl prev_copy = (big_udbl) prev;
    big_udbl multiplied = (big_udbl) prev_copy + a_copy * b_copy + carry_copy;

    big_uint limb = (big_uint) multiplied;
    big_uint carry_new = (big_uint) (multiplied >> 64);

    return (struct bigIntCalcMult) {.result = limb, .carry = carry_new};
}

int big_mul(bigint *X, const bigint *A, const bigint *B) {

    bigint A_copy;
    big_init(&A_copy);
    big_copy(&A_copy, A);

    bigint B_copy;
    big_init(&B_copy);
    big_copy(&B_copy, B);

    size_t new_num_limbs = A->num_limbs + B->num_limbs;
    X->data = realloc(X->data, sizeof(big_uint) * new_num_limbs);
    X->num_limbs = new_num_limbs;
    X->signum = (&A_copy)->signum * (&B_copy)->signum;

    for (size_t i=0; i < new_num_limbs; i++) {
        X->data[i] = (big_uint) 0;
    }

    big_uint c = 0;
    for (size_t i=0; i < (&A_copy)->num_limbs; i++) {
        c = 0;
        for (size_t j=0; j < (&B_copy)->num_limbs; j++) {
            struct bigIntCalcMult final = bigMult1x1((&B_copy)->data[j], (&A_copy)->data[i], c, X->data[i+j]);
            X->data[i+j] = final.result;
            c = final.carry;
        }
        X->data[i + (&B_copy)->num_limbs] += c;
    }
    while (X->data[X->num_limbs - 1] == 0 && X->num_limbs != 1) {
        X->data = (big_uint *) realloc(X->data, sizeof(big_uint) * (X->num_limbs - 1));
        X->num_limbs -= 1;
    }
    big_free(&A_copy);
    big_free(&B_copy);
    return 0;
}

int big_exp(bigint *A, size_t exp) {
    A->data = realloc(A->data, sizeof(big_uint) * (exp + 1));
    A->num_limbs = (exp + 1);
    A->data[A->num_limbs - 1] = 1;
    for (int i = A->num_limbs - 1; i > 0; i--) {
        A->data[i - 1] = 0;
    }
    return 0;
}

big_uint big_power_of_two(big_uint y_t) {
    big_uint base = (UINT64_MAX / 2) + 1;
    big_uint divided = base / y_t;

    big_uint value = 1;

    while (value < divided) {
        value = value << 1;
    }
    return value;
}

int big_get_power(big_uint lmb) {

    int value = 0;

    while (lmb != 0) {
        lmb = lmb >> 1;
        value += 1;
    }
    return value;
}

int big_shift_data(bigint *A, int shift) {
    unsigned int bit_shift = shift;
    if (bit_shift != 1) {

        for (size_t i = 0; i < A->num_limbs - 1; i++) {
            A->data[i] = (A->data[i + 1] << (64 - (bit_shift - 1))) | (A->data[i] >> (bit_shift - 1));
        }

        A->data[A->num_limbs - 1] = A->data[A->num_limbs - 1] >> (bit_shift - 1);

        while (A->data[A->num_limbs - 1] == 0 && A->num_limbs != 1) {
            A->data = (big_uint *) realloc(A->data, sizeof(big_uint) * (A->num_limbs - 1));
            A->num_limbs -= 1;
        }
    }
    return 0;
}

int big_div(bigint *Q, bigint *R, const bigint *A, const bigint *B) {

    if (B->num_limbs == 1 && B->data[0] == 0) {
        return ERR_BIGINT_DIVISION_BY_ZERO;
    }

    if (A->signum == 1 && B->signum == -1) {
        bigint B_pos;
        big_init(&B_pos);
        big_copy(&B_pos, B);
        (&B_pos)->signum = 1;
        big_div(Q, R, A, &B_pos);
        if (Q != NULL) {
            Q->signum = -1;
        }
        big_free(&B_pos);
        if (R != NULL) {
            big_add(R, R, B);
        }
        return 0;
    }

    if (A->signum == -1 && B->signum == 1) {
        bigint A_pos;
        big_init(&A_pos);
        big_copy(&A_pos, A);
        (&A_pos)->signum = 1;
        big_div(Q, R, &A_pos, B);
        if (Q != NULL) {
            Q->signum = -1;
        }
        big_free(&A_pos);
        if (R != NULL) {
            big_sub(R, B, R);
        }
        return 0;
    }

    if (big_cmp(A,B) == -1) {
        if (R != NULL) {
            big_copy(R, A);
        }
        if (Q != NULL) {
            Q->num_limbs = 1;
            Q->data = (big_uint *) realloc(Q->data, sizeof(big_uint));
            Q->data[0] = (big_uint) 0;
            Q->signum = 1;
        }
        return 0;
    }

    else if (big_cmp(A,B) == 0) {
        if (Q != NULL) {
            Q->num_limbs = 1;
            Q->data = (big_uint *) realloc(Q->data, sizeof(big_uint));
            Q->data[0] = (big_uint) 1;
            Q->signum = 1;
        }
        if (R != NULL) {
            R->num_limbs = 1;
            R->data = (big_uint *) realloc(R->data, sizeof(big_uint));
            R->data[0] = (big_uint) 0;
        }
        return 0;
    }

    bigint A_copy;
    big_init(&A_copy);

    bigint B_copy;
    big_init(&B_copy);

    bigint A_exp;
    big_init(&A_exp);
    (&A_exp)->signum = 1;

    big_uint lmb = big_power_of_two(B->data[B->num_limbs - 1]);
    bigint lmb_num;
    big_init(&lmb_num);
    big_set_nonzero(&lmb_num, lmb);

    big_mul(&A_copy, A, &lmb_num);
    big_mul(&B_copy, B, &lmb_num);

    big_free(&lmb_num);

    bigint Q_copy;
    big_init(&Q_copy);

    (&Q_copy)->num_limbs = (&A_copy)->num_limbs - (&B_copy)->num_limbs + 1;
    (&Q_copy)->data = (big_uint *) realloc((&Q_copy)->data, sizeof(big_uint) * (&Q_copy)->num_limbs);
    (&Q_copy)->signum = 1;

    for (size_t i=0; i < (&Q_copy)->num_limbs; i++) {
        (&Q_copy)->data[i] = (big_uint) 0;
    }

    bigint Base;
    big_init(&Base);
    (&Base)->signum = 1;
    big_exp(&Base, (((&A_copy)->num_limbs - 1) - ((&B_copy)->num_limbs - 1)));

    bigint exp_mult;
    big_init(&exp_mult);
    big_mul(&exp_mult, &B_copy, &Base);

    while (big_cmp(&A_copy, &exp_mult) >= 0) {
        big_sub(&A_copy, &A_copy, &exp_mult);
        (&Q_copy)->data[(&Q_copy)->num_limbs - 1] += 1;
    }

    size_t t = (&B_copy)->num_limbs - 1;

    for (size_t i=(&A_copy)->num_limbs - 1; i >= t + 1; i--) {
        if ((&A_copy)->data[i] == (&B_copy)->data[t]) {
            (&Q_copy)->data[i - t - 1] = UINT64_MAX;
        }
        else {
            big_udbl xb_xi_1 = (big_udbl) ((&A_copy)->data[i]) << 64 | (&A_copy)->data[i - 1];
            big_udbl y_t = (big_udbl) (&B_copy)->data[t];
            (&Q_copy)->data[i - t - 1] = xb_xi_1 / y_t;
        }

        bigint q_i_t_1;
        big_init(&q_i_t_1);

        bigint right_side;
        big_init(&right_side);
        (&right_side)->num_limbs = 3;
        (&right_side)->signum = 1;
        (&right_side)->data = realloc((&right_side)->data, sizeof(big_uint) * 3);
        
        if (i >= 2) {
            (&right_side)->data[0] = (&A_copy)->data[i - 2];
            (&right_side)->data[1] = (&A_copy)->data[i - 1];
            (&right_side)->data[2] = (&A_copy)->data[i];
        }
        else if (i >= 1) {
            (&right_side)->data[0] = 0;
            (&right_side)->data[1] = (&A_copy)->data[i - 1];
            (&right_side)->data[2] = (&A_copy)->data[i];
        }
        else {
            (&right_side)->data[0] = 0;
            (&right_side)->data[1] = 0;
            (&right_side)->data[2] = (&A_copy)->data[i];
        }

        bigint left_side;
        big_init(&left_side);
        (&left_side)->signum = 1;
        (&left_side)->num_limbs = 2;
        (&left_side)->data = realloc((&left_side)->data, sizeof(big_uint) * 2);
        if (t >= 1) {
            (&left_side)->data[0] = (&B_copy)->data[t - 1];
            (&left_side)->data[1] = (&B_copy)->data[t];
        }
        else {
            (&left_side)->data[0] = 0;
            (&left_side)->data[1] = (&B_copy)->data[t];
        }

        bigint multiplied;
        big_init(&multiplied);

        big_set_nonzero(&q_i_t_1, (&Q_copy)->data[i - t - 1]);

        big_mul(&multiplied, &q_i_t_1, &left_side);

        while (big_cmp(&multiplied, &right_side) == 1) {
            (&Q_copy)->data[i - t - 1] -= 1;
            (&q_i_t_1)->data[0] -= 1;
            big_mul(&multiplied, &q_i_t_1, &left_side);
        }

        big_free(&left_side);
        big_free(&right_side);
        big_free(&multiplied);

        bigint b_i_t_1;
        big_init(&b_i_t_1);

        big_exp(&b_i_t_1, i - t - 1);
        (&b_i_t_1)->signum = 1;

        bigint mul_result;
        big_init(&mul_result);
        big_mul(&mul_result, &B_copy, &b_i_t_1);

        big_set_nonzero(&q_i_t_1, (&Q_copy)->data[i - t - 1]);

        bigint mul_result2;
        big_init(&mul_result2);
        big_mul(&mul_result2, &mul_result, &q_i_t_1);

        bigint A_copy_new;
        big_init(&A_copy_new);
        big_sub(&A_copy_new, &A_copy, &mul_result2);
        big_copy(&A_copy, &A_copy_new);
        big_free(&A_copy_new);

        if ((&A_copy)->signum == -1) {
            big_add(&A_copy, &A_copy, &mul_result);
            (&Q_copy)->data[i - t - 1] -= 1;
        }
        
        big_free(&mul_result);

        if ((&A_copy)->num_limbs < i) {
            (&A_copy)->data = realloc((&A_copy)->data, sizeof(big_uint) * i);
            size_t index = i - 1;
            for (size_t j=0; j < i - (&A_copy)->num_limbs; j++) {
                (&A_copy)->data[index] = 0;
                index--;
            }
            (&A_copy)->num_limbs = i;
        }

        big_free(&q_i_t_1);
        big_free(&b_i_t_1);
        big_free(&mul_result2);

    }
    big_free(&A_exp);
    big_free(&Base);
    big_free(&exp_mult);

    int num_exp = big_get_power(lmb);
    big_shift_data(&A_copy, num_exp);

    if (Q != NULL) {
        big_copy(Q, &Q_copy);
    }
    big_free(&Q_copy);

    if (R != NULL) {
        while ((&A_copy)->data[(&A_copy)->num_limbs - 1] == 0 && (&A_copy)->num_limbs != 1) {
            (&A_copy)->data = (big_uint *) realloc((&A_copy)->data, sizeof(big_uint) * ((&A_copy)->num_limbs - 1));
            (&A_copy)->num_limbs -= 1;
        }
        big_copy(R, &A_copy);
    }
    big_free(&A_copy);
    big_free(&B_copy);

    return 0;
}

int big_gcd(bigint *G, const bigint *A, const bigint *B) {

    if (A->num_limbs == 1 && A->data[0] == 0) {
        big_copy(G, B);
        G->signum = 1;
        return 0;
    }
    
    else if (B->num_limbs == 1 && B->data[0] == 0) {
        big_copy(G, A);
        G->signum = 1;
        return 0;
    }

    bigint A_copy, B_copy;

    big_init(&A_copy);
    big_init(&B_copy);

    big_copy(&A_copy, A);
    big_copy(&B_copy, B);

    if (A->signum == 1 && B->signum == -1) {
        (&A_copy)->signum = 1;
        (&B_copy)->signum = 1;
        big_gcd(G, &A_copy, &B_copy);
        big_free(&A_copy);
        big_free(&B_copy);
        return 0;
    }

    else if (A->signum == -1 && B->signum == 1) {
        (&A_copy)->signum = 1;
        (&B_copy)->signum = 1;
        big_gcd(G, &A_copy, &B_copy);
        big_free(&A_copy);
        big_free(&B_copy);
        return 0;
    }

    else if (A->signum == -1 && B->signum == -1) {
        (&A_copy)->signum = 1;
        (&B_copy)->signum = 1;
        big_gcd(G, &A_copy, &B_copy);
        big_free(&A_copy);
        big_free(&B_copy);
        return 0;
    }

    bigint zero;
    big_init(&zero);
    (&zero)->signum = 1;
    (&zero)->num_limbs = 1;
    (&zero)->data = realloc((&zero)->data, sizeof(big_uint));
    (&zero)->data[0] = 0;
    
    size_t iter = 0;
    while (big_cmp(&B_copy, &zero) == 1) {
        bigint quotient;
        big_init(&quotient);
        bigint remainder;
        big_init(&remainder);
        big_div(&quotient, &remainder, &A_copy, &B_copy);
        big_copy(&A_copy, &B_copy);
        big_copy(&B_copy, &remainder);
        big_free(&quotient);
        big_free(&remainder);
        iter += 1;
    }
    big_free(&zero);
    big_copy(G, &A_copy);
    big_free(&A_copy);
    big_free(&B_copy);
    return 0;
}

int big_inv_mod(bigint *X, const bigint *A, const bigint *N) {
    if (N->num_limbs == 1 && (N->data[0] == 1 || N->data[0] == 0)) {
        return ERR_BIGINT_BAD_INPUT_DATA;
    }

    if (N->signum == -1) {
        return ERR_BIGINT_BAD_INPUT_DATA;
    }

    bigint gcd;
    big_init(&gcd);
    big_gcd(&gcd, A, N);
    if ((&gcd)->num_limbs != 1 && (&gcd)->data[0] != 1) {
        big_free(&gcd);
        return ERR_BIGINT_NOT_ACCEPTABLE;
    }
    else if ((&gcd)->num_limbs == 1 && (&gcd)->data[0] != 1) {
        big_free(&gcd);
        return ERR_BIGINT_NOT_ACCEPTABLE;
    }
    big_free(&gcd);

    bigint A_copy, N_copy;

    big_init(&A_copy);
    big_init(&N_copy);

    big_copy(&A_copy, A);
    big_copy(&N_copy, N);

    bigint x, y;
    big_init(&x);
    big_init(&y);
    big_set_nonzero(&x, 1);
    big_set_nonzero(&y, 0);

    bigint one;
    big_init(&one);
    (&one)->signum = 1;
    (&one)->num_limbs = 1;
    (&one)->data = realloc((&one)->data, sizeof(big_uint));
    (&one)->data[0] = 1;

    bigint q, r;
    big_init(&q);
    (&q)->signum = 1;
    big_init(&r);
    (&r)->signum = 1;

    bigint t;
    big_init(&t);

    while (big_cmp(&A_copy, &one) == 1) {
        big_div(&q, &r, &A_copy, &N_copy);
        big_copy(&t, &N_copy);
        big_copy(&N_copy, &r);
        big_copy(&A_copy, &t);
        big_copy(&t, &y);

        bigint new_y;
        big_init(&new_y);

        big_mul(&new_y, &q, &y);
        big_sub(&y, &x, &new_y);
        big_copy(&x, &t);
        big_free(&new_y);
    }

    if ((&x)->signum == -1) {
        big_add(&x, &x, N);
    }

    big_copy(X, &x);
    big_free(&one);
    big_free(&t);
    big_free(&q);
    big_free(&r);
    big_free(&x);
    big_free(&y);
    big_free(&A_copy);
    big_free(&N_copy);

    return 0;
}

int calculate_r(bigint *R, bigint *N) {
    size_t len = N->num_limbs;

    size_t num_limbs = len + 1;

    R->data = realloc(R->data, sizeof(big_uint) * num_limbs);

    for (size_t i=0; i < num_limbs; i++) {
        R->data[i] = 0;
    }

    R->data[num_limbs - 1] = 1;

    R->num_limbs = num_limbs;
    R->signum = 1;
}

int big_shift_by_base(bigint *divided) {
    for (size_t i=0; i < divided->num_limbs - 1; i++) {
        divided->data[i] = divided->data[i + 1];
    }
    divided->num_limbs -= 1;
    divided->data = realloc(divided->data, sizeof(big_uint) * divided->num_limbs);
}

int montgomery_multiplication(bigint *final, bigint *x, bigint *y, bigint *N, bigint *N_prime, bigint *R, bigint *b) {
    if (x->num_limbs > N->num_limbs) {
        big_div(NULL, x, x, N);
    }
    
    final->num_limbs = N->num_limbs + 1;
    final->data = realloc(final->data, sizeof(big_uint) * (final->num_limbs));
    for (size_t i=0; i < final->num_limbs; i++) {
        final->data[i] = 0;
    }
    final->signum = 1;

    bigint u_i;
    big_init(&u_i);

    bigint x_i;
    big_init(&x_i);

    bigint y_0;
    big_init(&y_0);
    big_set_nonzero(&y_0, y->data[0]);

    size_t n_iter = final->num_limbs - 1;
    for (size_t i = 0; i < n_iter; i++) {
        bigint tmp;
        big_init(&tmp);

        bigint a_0;
        big_init(&a_0);
        big_set_nonzero(&a_0, final->data[0]);
        if (i >= x->num_limbs) {
            big_set_nonzero(&x_i, 0);
        }
        else {
            big_set_nonzero(&x_i, x->data[i]);
        }

        big_mul(&tmp, &x_i, &y_0);
        bigint added_result;
        big_init(&added_result);
        big_add(&added_result, &a_0, &tmp);
        big_mul(&tmp, &added_result, N_prime);

        big_div(NULL, &u_i, &tmp, b);
        big_free(&added_result);

        bigint A_new;
        big_init(&A_new);

        bigint tmp2;
        big_init(&tmp2);

        bigint x_i_y;
        big_init(&x_i_y);
        big_mul(&x_i_y, &x_i, y);

        bigint u_i_m;
        big_init(&u_i_m);
        big_mul(&u_i_m, &u_i, N);

        big_add(&tmp2, &x_i_y, &u_i_m);
        bigint tmp3;
        big_init(&tmp3);
        big_add(&tmp3, &tmp2, final);

        big_shift_by_base(&tmp3);
        big_copy(final, &tmp3);

        big_free(&tmp);
        big_free(&a_0);
        big_free(&A_new);
        big_free(&tmp2);
        big_free(&x_i_y);
        big_free(&u_i_m);
        big_free(&tmp3);
    }
    big_free(&u_i);
    big_free(&x_i);
    big_free(&y_0);

    while (final->data[final->num_limbs - 1] == 0 && final->num_limbs != 1) {
        final->data = (big_uint *) realloc(final->data, sizeof(big_uint) * (final->num_limbs - 1));
        final->num_limbs -= 1;
    }

    if (big_cmp(final, N) >= 0) {
        big_sub(final, final, N);
    }

    return 0;
}

int divide_by_two(bigint *A) {
    unsigned int bit_shift = 2;

    for (size_t i = 0; i < A->num_limbs - 1; i++) {
        A->data[i] = (A->data[i + 1] << (64 - (bit_shift - 1))) | (A->data[i] >> (bit_shift - 1));
    }

    A->data[A->num_limbs - 1] = A->data[A->num_limbs - 1] >> (bit_shift - 1);

    while (A->data[A->num_limbs - 1] == 0 && A->num_limbs != 1) {
        A->data = (big_uint *) realloc(A->data, sizeof(big_uint) * (A->num_limbs - 1));
        A->num_limbs -= 1;
    }
    return 0;
}

int bigintToBinaryString(bigint *n, char *string) {

    int num_bits = sizeof(big_uint) * 8;
    char tmp[n->num_limbs * num_bits + 1];

    size_t curr_limb = 0;
    for (int k=n->num_limbs - 1; k >=0; k--) {
        big_uint num = n->data[k];
        for (int i = num_bits - 1; i >= 0; i--) {
            tmp[curr_limb * 64 + i] = (num & 1) + '0';
            num >>= 1;
        }
        curr_limb += 1;
    }
    tmp[n->num_limbs * num_bits] = '\0';
    size_t j = 0;
    while (tmp[j] == '0') {
        j++;
    }

    size_t written = 0;
    while(tmp[written + j] != '\0') {
        string[written] = tmp[written + j];
        written++;
    }
    string[written] = '\0';

    return 0;
}

int big_exp_mod(bigint *X, const bigint *A, const bigint *E, const bigint *N,
                bigint *_RR) {

    if (N->signum == -1 || N->data[0] % 2 == 0) {
        return ERR_BIGINT_BAD_INPUT_DATA;
    }
    if (E->signum == -1) {
        return ERR_BIGINT_BAD_INPUT_DATA;
    }
   
    bigint E_copy;
    big_init(&E_copy);
    big_copy(&E_copy, E);

    // Step 0
    bigint R;
    big_init(&R);
    calculate_r(&R, N);
    
    // Step 1

    bigint b;
    big_init(&b);
    (&b)->num_limbs = 2;
    (&b)->data = realloc((&b)->data, sizeof(big_uint) * 2);
    (&b)->data[0] = 0;
    (&b)->data[1] = 1;
    (&b)->signum = 1;

    bigint r_inv;
    big_init(&r_inv);
    big_inv_mod(&r_inv, N, &b);
    (&r_inv)->signum *= -1;
    if ((&r_inv)->signum == -1) {
        big_add(&r_inv, &r_inv, &b);
    }
    bigint n_prime;
    big_init(&n_prime);
    big_copy(&n_prime, &r_inv);
    big_free(&r_inv);

    // Step 2
    bigint x;
    big_init(&x);
    if (_RR == NULL) {
        bigint r_squared;
        big_init(&r_squared);
        big_mul(&r_squared, &R, &R);
        bigint _rr;
        big_init(&_rr);
        big_div(NULL, &_rr, &r_squared, N);
        montgomery_multiplication(&x, A, &_rr, N, &n_prime, &R, &b);
        big_free(&r_squared);
        big_free(&_rr);
    }
    else {
        montgomery_multiplication(&x, A, _RR, N, &n_prime, &R, &b);
    }

    bigint A_red;
    big_init(&A_red);
    big_div(NULL, &A_red, &R, N);

    // Step 4
    char e_bits[(&E_copy)->num_limbs * 64 + 1];
    bigintToBinaryString(&E_copy, e_bits);
    size_t i = 0;
    while (e_bits[i] != '\0') {
        bigint new_a;
        big_init(&new_a);
        montgomery_multiplication(&new_a, &A_red, &A_red, N, &n_prime, &R, &b);

        big_copy(&A_red, &new_a);

        if (e_bits[i] == '1') {
            montgomery_multiplication(&new_a, &A_red, &x, N, &n_prime, &R, &b);
            big_copy(&A_red, &new_a);
        }
        big_free(&new_a);
        i++;
    }
    bigint one;
    big_init(&one);
    (&one)->num_limbs = 1;
    (&one)->data = realloc((&one)->data, sizeof(big_uint));
    (&one)->data[0] = 1;
    (&one)->signum = 1;

    montgomery_multiplication(X, &A_red, &one, N, &n_prime, &R, &b);
    big_free(&E_copy);
    big_free(&R);
    big_free(&n_prime);
    big_free(&A_red);
    big_free(&x);
    big_free(&one);
    big_free(&b);

    return 0;
}

int generate_random_bigint(bigint *a, bigint *n) {
    srand(time(NULL));
    size_t num_limbs = n->num_limbs;
    a->num_limbs = num_limbs;
    a->data = realloc(a->data, sizeof(big_uint) * num_limbs);
    a->signum = 1;
    for (size_t i=0; i < num_limbs - 1; i++) {
        int r1 = rand();
        int r2 = rand();
        big_uint num = (big_uint) r1 << 62 | r2;
        int last = rand() % (3 - 0 + 1) + 0;
        num = last | num >> 2; 
        a->data[i] = num;
    }

    int r1 = rand();
    int r2 = rand();
    big_uint num = (big_uint) r1 << 62 | r2;
    while (num > a->data[a->num_limbs - 1]) {
        int r1 = rand();
        int r2 = rand();
        big_uint num = (big_uint) r1 << 62 | r2;
    }
    return 0;
}

bool miller_test(big_uint a, bigint *d, bigint *n, big_uint s) {
    bigint A;
    big_init(&A);
    big_set_nonzero(&A, a);

    if (big_cmp(&A, n) >= 0) {
        big_free(&A);
        return true;
    }

    bigint x;
    big_init(&x);
    big_exp_mod(&x, &A, d, n, NULL);

    bigint n_1;
    big_init(&n_1);
    big_copy(&n_1, n);

    bigint one;
    big_init(&one);
    big_set_nonzero(&one, 1);
    
    big_sub(&n_1, n, &one);

    bigint y;
    big_init(&y);
    big_copy(&y, &x);
    for (size_t i=0; i < s; i++) {
        bigint x_squared;
        big_init(&x_squared);
        big_mul(&x_squared, &x, &x);
        big_div(NULL, &y, &x_squared, n);

        if ((big_cmp(&y, &one) == 0) && (big_cmp(&x, &one) != 0) && (big_cmp(&x, &n_1) != 0)) {
            big_free(&A);
            big_free(&x);
            big_free(&n_1);
            big_free(&one);
            big_free(&y);
            return false;
        }
        big_copy(&x, &y);
        big_free(&x_squared);
    }

    if (big_cmp(&y, &one) != 0) {
        big_free(&A);
        big_free(&x);
        big_free(&n_1);
        big_free(&one);
        big_free(&y);
        return false;
    }

    big_free(&A);
    big_free(&x);
    big_free(&n_1);
    big_free(&one);
    big_free(&y);
    
    return true;
}

int big_is_prime(const bigint *X) {
    if (X->signum == -1 || (X->num_limbs == 1 && X->data[0] == 4)) {  
        return ERR_BIGINT_NOT_ACCEPTABLE;
    }
    if (X->num_limbs == 1 && X->data[0] <= 3) {
        return 0;
    }

    bigint d;
    big_init(&d);

    bigint one;
    big_init(&one);
    big_set_nonzero(&one, 1);

    big_sub(&d, X, &one);

    big_set_nonzero(&one, 2);

    big_uint s = 0;
    while ((&d)->data[0] % 2 == 0) {
        divide_by_two(&d);
        s += 1;
    }

    if (s == 0) {
        big_free(&d);
        big_free(&one);
        return ERR_BIGINT_NOT_ACCEPTABLE;
    }

    uint8_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

    for (size_t i=0; i < 12; i++) {
        big_uint a = (big_uint) bases[i];
        if (!miller_test(a, &d, X, s)) {
            big_free(&d);
            big_free(&one);
            return ERR_BIGINT_NOT_ACCEPTABLE;
        }
    }

    big_free(&d);
    big_free(&one);

    return 0;
}

int big_two_exponenent(bigint *lower, bigint *upper, size_t bits) {
    
    size_t num_limbs = (bits - 1) / 64;
    size_t shift = (bits - 1) % 64;

    lower->num_limbs = num_limbs + 1;
    lower->signum = 1;
    lower->data = realloc(lower->data, sizeof(big_uint) * lower->num_limbs);

    for (size_t i = 0; i < num_limbs; i++) {
        lower->data[i] = 0;
    }

    lower->data[lower->num_limbs - 1] = (big_uint) 1 << shift;

    upper->num_limbs = num_limbs + 1;
    upper->signum = 1;
    upper->data = realloc(upper->data, sizeof(big_uint) * upper->num_limbs);

    for (size_t i = 0; i < num_limbs; i++) {
        upper->data[i] = UINT64_MAX;
    }
    upper->data[num_limbs] = 0;
    for (size_t i = 0; i < shift + 1; i++) {
        upper->data[num_limbs] |= ((uint64_t)1 << i);
    }
    return 0;
}

int get_random_bigint(bigint *A, size_t size, big_uint lower, big_uint upper) {
    big_fill_random(A, size);
    if (lower == upper) {
        A->data[A->num_limbs - 1] = lower;
    }
    else {
        big_uint range = upper - lower + 1;
        big_uint highest = 0;

        for (int i = 0; i < 4; ++i) {
            highest = (highest << 15) | (rand() & 0x7FFF);
        }
        big_uint new_highest = lower + (highest % range);
        A->data[A->num_limbs - 1] = new_highest;
    }
    return 0;
}

int big_gen_prime(bigint *X, size_t nbits) {
    bigint lower;
    big_init(&lower);

    bigint upper;
    big_init(&upper);

    big_two_exponenent(&lower, &upper, nbits);

    bigint n;
    big_init(&n);

    size_t size = big_size(&lower);
    srand(time(NULL));
    while (true) {
        get_random_bigint(&n, size, (&lower)->data[(&lower)->num_limbs - 1], (&upper)->data[(&upper)->num_limbs - 1]);
        while (big_cmp(&lower, &n) != -1 || big_cmp(&n, &upper) != -1) {
            get_random_bigint(&n, size, (&lower)->data[(&lower)->num_limbs - 1], (&upper)->data[(&upper)->num_limbs - 1]);
        }
        (&n)->data[0] = (&n)->data[0] | 1;
        if (big_is_prime(&n) == 0) {
            big_copy(X, &n);
            big_free(&lower);
            big_free(&upper);
            big_free(&n);
            break;
        }
    }
    return 0;
}
