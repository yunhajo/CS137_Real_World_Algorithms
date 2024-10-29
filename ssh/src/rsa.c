#include "rsa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rsa_init(rsa_context *ctx) {
    ctx->ver = 0;
    big_init(&(ctx->N));
    big_init(&(ctx->E));

    big_init(&(ctx->D));
    big_init(&(ctx->P));
    big_init(&(ctx->Q));

    big_init(&(ctx->DP));
    big_init(&(ctx->DQ));
    big_init(&(ctx->QP));

    big_init(&(ctx->RN));

    big_init(&(ctx->RP));
    big_init(&(ctx->RQ));
}

void rsa_free(rsa_context *ctx) {
    big_free(&(ctx->N));
    big_free(&(ctx->E));

    big_free(&(ctx->D));
    big_free(&(ctx->P));
    big_free(&(ctx->Q));

    big_free(&(ctx->DP));
    big_free(&(ctx->DQ));
    big_free(&(ctx->QP));

    big_free(&(ctx->RN));

    big_free(&(ctx->RP));
    big_free(&(ctx->RQ));
}

int calculate_power_two(bigint *p, size_t bits) {
    size_t num_limbs = bits / 64;
    size_t shift = bits % 64;

    p->num_limbs = num_limbs + 1;
    p->data = realloc(p->data, sizeof(big_uint) * (num_limbs + 1));
    p->signum = 1;

    for (size_t i=0; i < num_limbs; i++) {
        p->data[i] = 0;
    }
    p->data[num_limbs] = (big_uint) 1 << shift;
    return 0;
}

int rsa_gen_key(rsa_context *ctx, size_t nbits, big_uint exponent) {
    bigint p;
    bigint q;
    big_init(&p);
    big_init(&q);

    bigint one;
    big_init(&one);
    big_set_nonzero(&one, 1);

    bigint abs;
    big_init(&abs);

    bigint powertwo;
    big_init(&powertwo);

    bigint multiplied;
    big_init(&multiplied);

    bigint p_1;
    big_init(&p_1);

    bigint q_1;
    big_init(&q_1);

    bigint exp;
    big_init(&exp);
    big_set_nonzero(&exp, exponent);

    bigint gcd;
    big_init(&gcd);

    bigint lcm;
    big_init(&lcm);

    bigint d;
    big_init(&d);

    bigint powertwo2;
    big_init(&powertwo2);

    for(;;) {
        big_gen_prime(&p, nbits/2);
        big_gen_prime(&q, nbits/2);

        while (big_cmp(&p, &q) == 0) {
            big_gen_prime(&q, nbits/2);
        }

        big_sub(&abs, &p, &q);
        (&abs)->signum = 1;

        size_t size = nbits/2;
        if (size >= 100) {
            size -= 100;
        }
        else {
            size = 0;
        }
        calculate_power_two(&powertwo, size);

        if (big_cmp(&abs, &powertwo) <= 0) {
            for (size_t i=0; i < (&p)->num_limbs; i++) {
                printf("p limb %zu %llu\n", i, (&p)->data[i]);
            }
            for (size_t i=0; i < (&q)->num_limbs; i++) {
                printf("q limb %zu %llu\n", i, (&q)->data[i]);
            }
            continue;
        }

        big_sub(&p_1, &p, &one);
        big_sub(&q_1, &q, &one);
        big_mul(&multiplied, &p_1, &q_1);

        big_gcd(&gcd, &exp, &multiplied);

        if ((&gcd)->num_limbs != 1 && (&gcd)->data[0] != 1) {
            continue;
        }

        big_div(&lcm, NULL, &multiplied, &gcd);

        big_inv_mod(&d, &exp, &lcm);

        size_t size2 = nbits/2;
        calculate_power_two(&powertwo2, size2);

        if (big_cmp(&d, &powertwo2) <= 0) {
            continue;
        }

        big_copy(&(ctx->P), &p);
        big_copy(&(ctx->Q), &q);
        big_copy(&(ctx->D), &d);
        big_copy(&(ctx->E), &exp);

        big_div(NULL, &(ctx->DP), &d, &p_1);
        big_div(NULL, &(ctx->DQ), &d, &q_1);
        big_mul(&(ctx->N), &p, &q);
        ctx->len = big_size(&(ctx->N));
        // printf("N is \n");
        // for (size_t i=0; i < (&(ctx->N))->num_limbs; i++) {
        //     printf("N limb %zu %llu\n", i, (&(ctx->N))->data[i]);
        // }

        
        bigint R;
        big_init(&R);
        calculate_r(&R, &(ctx->N));

        bigint r_squared;
        big_init(&r_squared);
        big_mul(&r_squared, &R, &R);
        bigint _rr;
        big_init(&_rr);
        big_div(NULL, &_rr, &r_squared, &(ctx->N));
        big_copy(&(ctx->RN), &_rr);

        big_div(NULL, &(ctx->RP), &r_squared, &(ctx->P));
        big_div(NULL, &(ctx->RQ), &r_squared, &(ctx->Q));
        big_inv_mod(&(ctx->QP), &q, &p);
        // for (size_t i=0; i < (&(ctx->QP))->num_limbs; i++) {
        //         printf("p limb %zu %llu\n", i, (&(ctx->QP))->data[i]);
        //     }
        
        
        big_free(&r_squared);
        big_free(&_rr);

        big_free(&p);
        big_free(&q);
        big_free(&one);
        big_free(&abs);
        big_free(&powertwo);
        big_free(&multiplied);
        big_free(&p_1);
        big_free(&q_1);
        big_free(&exp);
        big_free(&gcd);
        big_free(&lcm);
        big_free(&d);
        big_free(&powertwo2);
        big_free(&R);
        
        return 0;
    }

    return 0;
}

int write_uint32(uint8_t *buffer, uint32_t value) {
    buffer[0] = (uint8_t) (value >> 24);
    buffer[1] = (uint8_t) (value >> 16);
    buffer[2] = (uint8_t) (value >> 8);
    buffer[3] = value;
}

int rsa_write_public_key(const rsa_context *ctx, FILE *file) {
    const char *type = "ssh-rsa";
    uint32_t type_len = strlen(type);

    uint32_t e_len = big_size(&(ctx)->E);
    uint32_t n_len = big_size(&(ctx)->N);

    size_t blob_len = 4 + type_len + 4 + e_len + 4 + n_len;
    uint8_t blob[blob_len];
    uint8_t *ptr = blob;

    write_uint32(ptr, type_len);
    ptr += 4;
    memcpy(ptr, type, type_len); 
    ptr += type_len;
    write_uint32(ptr, e_len);
    ptr += 4;
    big_write_binary(&(ctx->E), ptr, e_len);
    ptr += e_len;
    write_uint32(ptr, n_len);
    ptr += 4;
    big_write_binary(&(ctx->N), ptr, n_len);

    size_t required = base64_size(blob_len);
    size_t olen;

    char dest[required];
    base64_encode(dest, required, &olen, blob, blob_len);
    fprintf(file, "ssh-rsa ");
    fprintf(file, "%s\n", dest);
    return 0;
}
